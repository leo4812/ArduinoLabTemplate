/*
    Библиотека для амплитудного анализа звука на Arduino
    Документация: 
    GitHub: https://github.com/GyverLibs/VolAnalyzer
    Возможности:
    - Хитрый адаптивный алгоритм
    - Встроенные фильтры для плавного потока значений
    - Получение громкости в указанном диапазоне независимо от диапазона входного сигнала
    - Получение сигнала на резкие изменения звука
    - Работа в режиме виртуального анализатора (без привязки к АЦП МК)
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - более резкое падение при пропадании звука
    v1.2 - +совместимость. Вернул плавное падение звука
    v1.3 - упрощение алгоритма. Новый обработчик импульсов
    v1.4 - улучшение алгоритма
    v1.5 - сильное облегчение и улучшение алгоритма
    v1.6 - более резкая реакция на звук
    v1.7 - исключено деление на 0 в map
	v1.8 - теперь работает с 12 бит АЦП
*/

/*
    // тикер анализатора. Вернёт true при завершении текущего анализа. Вызывать почаще
    bool tick();                    // опрашивает указанный в setPin пин
    bool tick(int thisRead);        // берёт указанное значение

    // настройки анализа
    void setPin(int pin);           // указать пин АЦП
    void setDt(int dt);             // установить время между опросами АЦП, мкс (по умолч. 500)
    void setWindow(int window);     // установка ширины окна выборки (по умолч. 20)
    void setTrsh(int trsh);         // установить порог громкости в единицах raw АЦП (умолч 40)

    // амплитуда
    void setAmpliDt(int ampliDt);   // установить период фильтрации амплитудных огибающих, мс (умолч 150)
    void setAmpliK(byte k);         // установить коэффициент фильтрации амплитудных огибающих 0-31 (умолч 30)

    // громкость
    void setVolDt(int volDt);       // установить период фильтрации громкости (умолч 20)
    void setVolK(byte k);           // установить коэффициент фильтрации громкости 0-31 (умолч 25)
    void setVolMin(int scale);      // установить минимальную величину громкости (умолч 0)
    void setVolMax(int scale);      // установить максимальную величину громкости (умолч 100)

    // пульс
    void setPulseMax(int maxV);     // верхний порог срабатывания пульса (по шкале громкости)
    void setPulseMin(int minV);     // нижний порог перезагрузки пульса (по шкале громкости)
    void setPulseTimeout(int tout); // таймаут пульса, мс

    // получаем значения
    int getVol();                   // громкость в пределах setVolMin.. setVolMax
    bool pulse();                   // резкий скачок громкости
    int getMax();                   // текущее значение огибающей максимумов
    int getRaw();                   // значение сырого сигнала
*/

#ifndef _VolAnalyzer_h
#define _VolAnalyzer_h
#include <Arduino.h>

// ========================== FFilter ==========================
struct FFilter {
    bool compute(bool force = false) {
        if (force || millis() - tmr >= dt) {
            tmr = millis();
            uint8_t kk = (raw < fil) ? k : (k >> 1);    // вверх с коэффициентом /2
            fil = ((uint32_t)kk * fil + (32 - kk) * raw) >> 5;    // целочисленный фильтр 5 бит
            return 1;
        }
        return 0;
    }
    uint8_t k = 20;
    int dt = 0, fil = 0, raw = 0;
    uint32_t tmr = 0;
};

// ========================== VolAnalyzer ==========================
class VolAnalyzer {
public:
    // создать с указанием пина. Если не указывать - будет виртуальный анализатор
    VolAnalyzer (int pin = -1) {
        setVolDt(20);
        setVolK(25);
        setAmpliDt(80);
        setAmpliK(25);
        if (pin != -1) setPin(pin);
    }
    
    // =========================== TICK ===========================
    // опрашивать как можно чаще. Может принимать значение, если это виртуальный анализатор
    // вернёт true при окончании анализа выборки
    bool tick(int read = -1) {
        volF.compute();                     // сглаживание громкости
        if (ampF.compute()) _ampli = 0;     // сглаживание амплитуды, сброс максимума

        // таймер выборки
        if (_dt == 0 || micros() - _tmrDt >= _dt) {
            if (_dt) _tmrDt = micros();
            if (read == -1) read = analogRead(_pin);
            _max = max(_max, read);         // поиск макс за выборку
            _min = min(_min, read);         // поиск мин за выборку

            if (++count >= _window) {       // выборка завершена
                _raw = _max - _min;         // сырая громкость
                _ampli = max(_ampli, _raw); // амплитудная огибающая
                ampF.raw = _ampli;          // передаём в фильтр

                if (_raw > ampF.fil) ampF.compute(true);    // форсируем фильтр
                
                if (_raw > _trsh && ampF.fil > _trsh) {     // если звук громкий + в map не будет 0
                    // от порога _trsh до сглаженной амплитуды в (_volMin, _volMax)
                    volF.raw = map(_raw, _trsh, ampF.fil, _volMin, _volMax);
                    volF.raw = constrain(volF.raw, _volMin, _volMax);
                    volF.compute(true);    // форсируем фильтр
                } else volF.raw = 0;
                
                // обработка пульса
                if (!_pulseState) {
                    if (volF.raw <= _pulseMin && millis() - _tmrPulse >= _pulseTout) _pulseState = 1;
                } else {
                    if (volF.raw > _pulseMax) {
                        _pulseState = 0;
                        _pulse = 1;
                        _tmrPulse = millis();
                    }
                }
                _max = count = 0;
                _min = 30000;
                return true;        // выборка завершена
            }
        }
        return false;
    }
    
    // ========================== SETTINGS ==========================
    // указать пин АЦП
    void setPin(int pin) {
        _pin = pin;
        pinMode(_pin, INPUT);
    }
    
    // установить время между опросами АЦП (мкс) (по умолч. 500) 
    void setDt(int dt) {
        _dt = dt;
    }
    
    // установка ширины окна выборки (по умолч. 20)
    void setWindow(int window) {
        _window = window;
    }
    
    // установить порог громкости в единицах АЦП (умолч 40)
    void setTrsh(int trsh) {
        _trsh = trsh;
    }
    
    // ========================== VOLUME ==========================
    // установить период фильтрации громкости (умолч 20)
    void setVolDt(int dt) {
        volF.dt = dt;
    }
    
    // установить коэффициент плавности громкости 0-31 (умолч 25)
    void setVolK(byte vk) {
        volF.k = vk;
    }
    
    // получить громкость в пределах setVolMin.. setVolMax
    int getVol() {
        return volF.fil;
    }
    
    // установить минимальную величину громкости (умолч 0)
    void setVolMin(int scale) {
        _volMin = scale;
    }
    
    // установить максимальную величину громкости (умолч 100)
    void setVolMax(int scale) {
        _volMax = scale;
    }
    
    // ========================= AMPLITUDE =========================
    // установить период фильтрации амплитудных огибающих
    void setAmpliDt(int dt) {
        ampF.dt = dt;
    }
    
    // установить коэффициент плавности амплитуды 0-31 (умолч 31)
    void setAmpliK(byte rk) {
        ampF.k = rk;
    }
    
    // получить текущее значение огибающей минимумов (с v1.5 - 0)
    int getMin() {
        return 0;
    }

    // получить текущее значение огибающей максимумов
    int getMax() {
        return ampF.fil;
    }
    
    // =========================== PULSE ===========================
    // верхний порог срабатывания пульса (по шкале громкости)
    void setPulseMax(int maxV) {
        _pulseMax = maxV;
    }
    
    // нижний порог перезагрузки пульса (по шкале громкости)
    void setPulseMin(int minV) {
        _pulseMin = minV;
    }
    
    // таймаут пульса, мс
    void setPulseTimeout(int tout) {
        _pulseTout = tout;
    }
    
    // резкий скачок громкости (true)
    bool pulse() {
        if (_pulse) {
            _pulse = false;
            return true;
        }
        return false;
    }
    
    // ========================== RAW DATA ===========================
    // получить значение сырого сигнала за выборку
    int getRaw() {
        return _raw;
    }
    
    // ========================= DEPRECATED =========================
    void setPeriod(int v) {}            // установить период между выборками
    int getRawMax() { return _raw; }    // получить максимальное значение сырого сигнала за выборку
    bool getPulse() { return pulse(); }
    void setPulseTrsh(int trsh) { setPulseMax(trsh); }

private:
    int _dt = 500;      // 500 мкс между сэмплами достаточно для музыки
    int _window = 20;   // при таком размере окна получаем длительность оцифровки 10 мс
    int _trsh = 40;
    int _volMin = 0, _volMax = 100;
    
    uint32_t _tmrPulse = 0, _tmrDt = 0;
    int _pin, count = 0;
    int _min = 30000, _max = 0, _ampli = 0, _raw = 0;
    
    int _pulseMax = 80, _pulseMin = 20, _pulseTout = 100;
    bool _pulse = 0, _pulseState = 0;

    FFilter volF, ampF;
};
#endif