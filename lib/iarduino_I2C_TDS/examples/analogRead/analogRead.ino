// ПРИМЕР ПОЛУЧЕНИЯ АНАЛОГОВЫХ ДАННЫХ:            // * Строки со звёздочкой являются необязательными.
                                                  //
const uint8_t pin_Vm = A0;                        //   Определяем вывод к которому подключён аналоговый выход модуля.
                                                  //
const float   Vcc    = 5.0f;                      //   Напряжение питания Arduino.
const float   Vccm   = 3.3f;                      //   Напряжение питания ОУ модуля (внутрисхемно используется 3,3В).
const float   Ka     = 1000.0f;                   //   Множитель степенной функции (определяется калибровкой модуля). Можно получить функцией getKa().
const float   Kb     = -5.0f;                     //   Степень   степенной функции (определяется калибровкой модуля). Можно получить функцией getKb().
const float   Kt     = 0.02f;                     //   Температурный коэффициент (зависит от состава жидкости).
const float   Kp     = 0.5f;                      //   Коэффициент пересчёта (зависит от состава жидкости).
const float   Kf     = 0.85f;                     //   Коэффициент передачи ФВЧ+ФНЧ модуля (зависит от частоты переменного тока используемого для проведения измерений).
const float   T      = 25.0f;                     //   Опорная температура в °C.
                                                  //
void setup(){                                     //
     delay(500);                                  // * Ждём завершение переходных процессов связанных с подачей питания.
     Serial.begin(9600);                          //   Инициируем работу с шиной UART для передачи данных в монитор последовательного порта на скорости 9600 бит/сек.
}                                                 //
                                                  //
void loop(){                                      //
     float t   = 25.0f;                           //   Указываем текущую температуру жидкости в °C.
     float Vm  = analogRead(pin_Vm)*Vcc/1023;     //   Получаем напряжение на выходе модуля в Вольтах.
     float S   = Ka * pow((Vccm-Kf*Vm)/2,Kb);     //   Получаем удельную электропроводность жидкости.
     float EC  = S / (1+Kt*(t-T));                //   Приводим удельную электропроводность жидкости к опорной температуре T.
     float TDS = EC * Kp;                         //   Получаем количество растворённых твёрдых веществ в жидкости.
//   Выводим полученные данные:                   //
     Serial.print((String)"S="+S+"мкСм/см, ");    //
     Serial.print((String)"EC="+EC+"мкСм/см, ");  //
     Serial.print((String)"TDS="+TDS+"мг/л\r\n"); //
     delay(1000);                                 //
}                                                 //
