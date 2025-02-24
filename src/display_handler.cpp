#include "display_handler.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool displayLoop = false;


// 'oddhorse paren logo small', 16x16px
const unsigned char epd_bitmap_oddhorse_paren_logo_small [] PROGMEM = {
	0x10, 0x08, 0x20, 0x04, 0x60, 0xe6, 0x41, 0xb2, 0xc3, 0xe3, 0xc6, 0xb3, 0xc5, 0x33, 0xcc, 0xa3, 
	0xcc, 0x23, 0xcc, 0x43, 0x67, 0x82, 0x60, 0x06, 0x30, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char epd_bitmap_oddhorse_paren_logo_large [] PROGMEM = {
	0x03, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x60, 0x0e, 0x00, 0x00, 0x70, 0x1c, 0x00, 0x00, 0x38, 
	0x38, 0x00, 0x38, 0x1c, 0x38, 0x01, 0xfe, 0x1c, 0x78, 0x03, 0xe7, 0x1e, 0x70, 0x0f, 0x83, 0x0e, 
	0x70, 0x0f, 0x7c, 0x0e, 0xf0, 0x1e, 0xfe, 0x0f, 0xf0, 0x3d, 0xc6, 0x0f, 0xf0, 0x39, 0x86, 0x0f, 
	0xf0, 0x79, 0x86, 0x0f, 0xf0, 0x71, 0x86, 0x0f, 0xf0, 0x71, 0x8e, 0x0f, 0xf0, 0xe1, 0x8e, 0x0f, 
	0xf0, 0xe0, 0x0c, 0x0f, 0xf0, 0xe0, 0x1c, 0x0f, 0x70, 0x60, 0x38, 0x0e, 0x70, 0x60, 0x70, 0x0e, 
	0x78, 0x7b, 0xe0, 0x1e, 0x38, 0x3f, 0xc0, 0x1c, 0x3c, 0x0c, 0x00, 0x3c, 0x1c, 0x00, 0x00, 0x38, 
	0x0e, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 192)
const int epd_bitmap_allArray_LEN = 2;
const unsigned char* epd_bitmap_allArray[2] = {
	epd_bitmap_oddhorse_paren_logo_small,
    epd_bitmap_oddhorse_paren_logo_large
};


void setupDisplay() {
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    display.clearDisplay();
    display.drawBitmap(
      (display.width()  - 16 ) / 2,
      (display.height() - 16) / 2,
      epd_bitmap_oddhorse_paren_logo_small, 16, 16, 1);
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();

    startDisplayTask(awaitingConnectionLoop);
}

// Task handle (so we can delete it later)
TaskHandle_t displayTaskHandle = NULL;

#define CONN_LOOP_FRAMES 60
#define CONN_LOOP_HEIGHT_RANGE 10
void awaitingConnectionLoop(void *pvParameters) {
    int connLoopI = 0;
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text

    while(1) {
        display.clearDisplay();
        int16_t boundX, boundY;
        uint16_t textHeight, textWidth;
        display.getTextBounds("awaiting bluetooth...", 0, 0, &boundX, &boundY, &textWidth, &textHeight);
        float radians = (static_cast<float>(connLoopI) / CONN_LOOP_FRAMES) * 2 * PI;
        int heightAdjust = round(sin(radians) * CONN_LOOP_HEIGHT_RANGE);
        display.setCursor((display.width() - textWidth) / 2,
                        ((display.height() - textHeight) / 2) + heightAdjust);
        display.println(F("awaiting bluetooth..."));
        display.display();
        
        connLoopI++;
        if (connLoopI >= CONN_LOOP_FRAMES) connLoopI = 0;
        vTaskDelay(pdMS_TO_TICKS(33));
    }
}

void updateButtonDisplay(void *pvParameters) {
    while(1) {
        display.clearDisplay();

        // Iterate over all buttons in the vector
        for (size_t i = 0; i < buttons.size(); i++) {
            uint16_t color = buttons[i].isPressed() ? WHITE : BLACK;
            display.fillRect(i * 32, 0, 32, 32, color);
        }
        display.display();

        vTaskDelay(pdMS_TO_TICKS(33));
    }
    /*
    vTaskDelete(NULL);  // Deletes this task, so it won’t keep running
    displayTaskHandle = NULL;  // Reset handle
    */
}

void stopDisplayTask() {
    if (displayTaskHandle != NULL) {
      vTaskDelete(displayTaskHandle);  // Stop the task
      displayTaskHandle = NULL;        // Reset handle
      display.clearDisplay(); // clear display buffer
      display.display(); // update display
      Serial.println("display task stopped.");
    }
}

void startDisplayTask(TaskFunction_t task) {
    if (displayTaskHandle != NULL) stopDisplayTask();

    xTaskCreate(
    task,    // Function to run on this task
    "displayTask",             // Name of this task
    2048,                      // Stack size (bytes)
    NULL,                      // Parameter to pass
    1,                         // Task priority
    &displayTaskHandle        // Task handle
    );
    Serial.println("display task started.");
}