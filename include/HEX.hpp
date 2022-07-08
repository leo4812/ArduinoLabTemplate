String HexString2ASCIIString(String hexstring)
{
    String temp = "", sub = "", result;
    char buf[3];
    for (unsigned int i = 0; i < hexstring.length(); i += 2)

    {
        sub = hexstring.substring(i, i + 2);
        sub.toCharArray(buf, 3);
        char b = (char)strtol(buf, 0, 16);
        if (b == '\0')
            break;
        temp += b;
    }
    return temp;
}

String buffToHex(uint8_t *buffer, uint8_t length)
{
    // String result = "";
    String hexstring = "";

    for (int i = 0; i < length; i++)

    {
        if (buffer[i] < 0x10)

        {
            hexstring += '0';
        }

        hexstring += String(buffer[i], HEX);
    }

    // result = HexString2ASCIIString(hexstring);
    return hexstring;
}