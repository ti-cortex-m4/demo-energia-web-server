#include <Arduino.h>
#include <IPAddressHolder.h>


IPAddressHolder::IPAddressHolder(const char *address)
{
    ipAddress = INADDR_NONE;
    valid = false;

    uint8_t octets[4];
    uint16_t acc = 0; // Accumulator
    uint8_t dots = 0;

    while (*address)
    {
        char c = *address++;
        if (c >= '0' && c <= '9')
        {
            acc = acc * 10 + (c - '0');
            if (acc > 255) {
                // Value out of [0..255] range
                return;
            }
        }
        else if (c == '.')
        {
            if (dots == 3) {
                // Too much dots (there must be 3 dots)
                return;
            }
            octets[dots++] = acc;
            acc = 0;
        }
        else
        {
            // Invalid char
            return;
        }
    }

    if (dots != 3) {
        // Too few dots (there must be 3 dots)
        return;
    }

    octets[3] = acc;

    ipAddress = IPAddress(octets[0], octets[1], octets[2], octets[3]);
    valid = true;
}

IPAddressHolder::IPAddressHolder(const String &address)
{
    IPAddressHolder(address.c_str());
}
