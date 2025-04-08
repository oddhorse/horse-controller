#ifndef util_h
#define util_h

#include <Arduino.h>

namespace Util
{
    // prints data in a teleplot-friendly format
    // eg. "">accel_x:340\n"
    template <typename T>
    void teleplot(const String &display, const T &value)
    {
        Serial.print(">");
        Serial.print(display);
        Serial.print(":");
        Serial.println(value);
    }
}

#endif // UTIL_H