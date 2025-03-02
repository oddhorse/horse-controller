#include <Adafruit_USBD_CDC.h> // for Serial
#include <malloc.h>

size_t getFreeRAM() {
    struct mallinfo memInfo = mallinfo();
    return memInfo.fordblks; // Free memory in bytes
}