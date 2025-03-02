#include "display_handler.h"
#include "control_handler.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool displayLoop = false;

// 'oddhorse paren logo small', 16x14px
const unsigned char epd_bitmap_oddhorse_paren_logo_small[] PROGMEM = {
	0x30, 0x0c, 0x71, 0xee, 0x63, 0x16, 0xe6, 0x67, 0xc6, 0xf3, 0xcd, 0xb3, 0xcd, 0x33, 0xcd, 0x33,
	0xcc, 0x63, 0xcc, 0xe3, 0xef, 0xc7, 0x67, 0x86, 0x70, 0x0e, 0x30, 0x0c};

const unsigned char epd_bitmap_oddhorse_paren_logo_large[] PROGMEM = {
	0x03, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x60, 0x0e, 0x00, 0x00, 0x70, 0x1c, 0x00, 0x00, 0x38,
	0x38, 0x00, 0x38, 0x1c, 0x38, 0x01, 0xfe, 0x1c, 0x78, 0x03, 0xe7, 0x1e, 0x70, 0x0f, 0x83, 0x0e,
	0x70, 0x0f, 0x7c, 0x0e, 0xf0, 0x1e, 0xfe, 0x0f, 0xf0, 0x3d, 0xc6, 0x0f, 0xf0, 0x39, 0x86, 0x0f,
	0xf0, 0x79, 0x86, 0x0f, 0xf0, 0x71, 0x86, 0x0f, 0xf0, 0x71, 0x8e, 0x0f, 0xf0, 0xe1, 0x8e, 0x0f,
	0xf0, 0xe0, 0x0c, 0x0f, 0xf0, 0xe0, 0x1c, 0x0f, 0x70, 0x60, 0x38, 0x0e, 0x70, 0x60, 0x70, 0x0e,
	0x78, 0x7b, 0xe0, 0x1e, 0x38, 0x3f, 0xc0, 0x1c, 0x3c, 0x0c, 0x00, 0x3c, 0x1c, 0x00, 0x00, 0x38,
	0x0e, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 'hand thin', 35x27px
const unsigned char epd_bitmap_hand_thin[] PROGMEM = {
	0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x84, 0xf0, 0x00, 0x00,
	0x1d, 0x05, 0x08, 0x00, 0x00, 0x63, 0x06, 0x08, 0x00, 0x00, 0x43, 0x06, 0x08, 0x00, 0x00, 0x41,
	0x06, 0x08, 0x00, 0x00, 0x41, 0x06, 0x08, 0x00, 0x00, 0x41, 0x06, 0x08, 0x00, 0x00, 0x41, 0x06,
	0x0f, 0xc0, 0x00, 0x41, 0x06, 0x0c, 0x20, 0x00, 0x41, 0x06, 0x08, 0x20, 0x00, 0x41, 0x06, 0x08,
	0x20, 0x00, 0x41, 0x06, 0x08, 0x20, 0x00, 0x41, 0x06, 0x18, 0x20, 0x7c, 0x41, 0x06, 0x18, 0x20,
	0x82, 0x41, 0x06, 0x10, 0x40, 0x83, 0x40, 0x00, 0x10, 0x40, 0x80, 0xc0, 0x00, 0x10, 0x40, 0x80,
	0xc0, 0x00, 0x00, 0x40, 0x40, 0x60, 0x00, 0x00, 0x40, 0x40, 0x60, 0x00, 0x00, 0x40, 0x40, 0x00,
	0x00, 0x00, 0x80, 0x20, 0x00, 0x00, 0x00, 0x80, 0x20, 0x00, 0x00, 0x00, 0x80, 0x10, 0x00, 0x00,
	0x00, 0x80, 0x08, 0x00, 0x00, 0x00, 0x80};

// 'music note', 10x9px
const unsigned char epd_bitmap_music_icon[] PROGMEM = {
	0x01, 0xc0, 0x1f, 0xc0, 0x1c, 0x40, 0x10, 0x40, 0x10, 0x40, 0x11, 0xc0, 0x73, 0xc0, 0xf1, 0x80,
	0x60, 0x00};

// 'cc icon', 10x9px
const unsigned char epd_bitmap_cc_icon[] PROGMEM = {
	0x39, 0xc0, 0x7b, 0xc0, 0x63, 0x00, 0x63, 0x00, 0x63, 0x00, 0x63, 0x00, 0x63, 0x00, 0x7b, 0xc0,
	0x39, 0xc0};

// Array of all bitmaps for convenience.
const int epd_bitmap_allArray_LEN = 3;
const unsigned char *epd_bitmap_allArray[epd_bitmap_allArray_LEN] = {
	epd_bitmap_oddhorse_paren_logo_small,
	epd_bitmap_oddhorse_paren_logo_large,
	epd_bitmap_hand_thin};

void getTextDims(String str, uint16_t *w, uint16_t *h)
{
	int16_t boundX, boundY;
	uint16_t wget, hget;
	display.getTextBounds(str, 0, 0, &boundX, &boundY, &wget, &hget);
	*w = wget;
	*h = hget;
}

void setupDisplay()
{
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
	{
		Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			;
	}
	display.clearDisplay();
	// display.fillScreen(WHITE);
	display.drawBitmap(
		(display.width() - 16) / 2,
		(display.height() - 14) / 2,
		epd_bitmap_oddhorse_paren_logo_small, 16, 14, WHITE);
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
void awaitingConnectionLoop(void *pvParameters)
{
	int connLoopI = 0;
	display.setTextSize(1);		 // Normal 1:1 pixel scale
	display.setTextColor(WHITE); // Draw white text

	while (1)
	{
		display.clearDisplay();
		uint16_t textHeight, textWidth;
		getTextDims("awaiting bluetooth...", &textWidth, &textHeight);
		float radians = (static_cast<float>(connLoopI) / CONN_LOOP_FRAMES) * 2 * PI;
		int heightAdjust = round(sin(radians) * CONN_LOOP_HEIGHT_RANGE);
		display.setCursor((display.width() - textWidth) / 2,
						  ((display.height() - textHeight) / 2) + heightAdjust);
		display.println(F("awaiting bluetooth..."));
		display.display();

		connLoopI++;
		if (connLoopI >= CONN_LOOP_FRAMES)
			connLoopI = 0;
		vTaskDelay(pdMS_TO_TICKS(33));
	}
}

#define OVERLAY_BORDER_SIZE 4
#define OVERLAY_SHADOW_SIZE 1
#define OVERLAY_SHADOW_SIZE_OFFSET 2

/**
 * @brief overlays the value of the rotary encoder
 */
void overlayValue()
{
	long elapsed = millis() - timeOfLastChange;
	if (elapsed < 2000)
	{
		uint16_t textHeight, textWidth;
		String displayString = "encoder: " + String(encoderPosition);
		getTextDims(displayString, &textWidth, &textHeight);
		int rectBounds[4] = {
			((display.width() - textWidth) / 2) - OVERLAY_BORDER_SIZE,
			((display.height() - textHeight) / 2) - OVERLAY_BORDER_SIZE,
			(textWidth + OVERLAY_BORDER_SIZE * 2),
			(textHeight + OVERLAY_BORDER_SIZE * 2)};
		display.fillRect(
			rectBounds[0] - OVERLAY_SHADOW_SIZE,
			rectBounds[1] - OVERLAY_SHADOW_SIZE,
			rectBounds[2] + OVERLAY_SHADOW_SIZE * 2 + OVERLAY_SHADOW_SIZE_OFFSET,
			rectBounds[3] + OVERLAY_SHADOW_SIZE * 2 + OVERLAY_SHADOW_SIZE_OFFSET,
			BLACK);
		display.drawRect(rectBounds[0], rectBounds[1], rectBounds[2], rectBounds[3], WHITE);
		display.setCursor(
			(display.width() - textWidth) / 2,
			((display.height() - textHeight) / 2));
		display.println(displayString);
	}
}

void updateButtonDisplay(void *pvParameters)
{
	while (1)
	{
		display.clearDisplay();
		display.drawFastHLine(0, 0, 41, WHITE);							// middle
		display.drawFastHLine(0, 16, 36, WHITE);						// index
		display.drawFastHLine(84, 0, 44, WHITE);						// ring
		display.drawFastHLine(85, 16, 43, WHITE);						// pinky
		display.drawLine(41, 1, 55, 5, WHITE);							// middle
		display.drawLine(37, 15, 48, 12, WHITE);						// index
		display.drawLine(73, 6, 83, 1, WHITE);							// ring
		display.drawFastHLine(79, 15, 6, WHITE);						// pinky
		display.drawBitmap(42, 5, epd_bitmap_hand_thin, 35, 27, WHITE); // hand

		// Iterate over all buttons in the vector
		for (size_t i = 0; i < buttons.size(); i++)
		{
			const unsigned char *icon;
			if (buttons[i].buttonType == MIDI_BUTTON_TYPE_NOTE)
			{
				icon = epd_bitmap_music_icon;
			}
			else if (buttons[i].buttonType == MIDI_BUTTON_TYPE_CC)
			{
				icon = epd_bitmap_cc_icon;
			}
			if (strcmp(buttons[i].name, "index") == 0)
			{
				if (buttons[i].isPressed())
				{
					display.fillRect(52, 9, 5, 4, WHITE);
				}
				display.setCursor(1, 18);
				display.println(buttons[i].currentValue);
				display.drawBitmap(28, 3, icon, 10, 9, WHITE);
			}
			if (strcmp(buttons[i].name, "middle") == 0)
			{
				if (buttons[i].isPressed())
				{
					display.fillRect(58, 6, 5, 4, WHITE);
				}
				display.setCursor(1, 3);
				display.println(buttons[i].currentValue);
				display.drawBitmap(28, 19, icon, 10, 9, WHITE);
			}
			if (strcmp(buttons[i].name, "ring") == 0)
			{
				if (buttons[i].isPressed())
				{
					display.fillRect(65, 8, 5, 4, WHITE);
				}
				uint16_t txth, txtw;
				getTextDims(String(buttons[i].currentValue), &txtw, &txth);
				display.setCursor(
					display.width() - txtw,
					3);
				display.println(buttons[i].currentValue);
				display.drawBitmap(89, 3, icon, 10, 9, WHITE);
			}
			if (strcmp(buttons[i].name, "pinky") == 0)
			{
				if (buttons[i].isPressed())
				{
					display.fillRect(71, 15, 5, 3, WHITE);
					display.drawPixel(74, 18, WHITE);
					display.drawPixel(75, 18, WHITE);
				}
				uint16_t txth, txtw;
				getTextDims(String(buttons[i].currentValue), &txtw, &txth);
				display.setCursor(
					(display.width() - txtw),
					18);
				display.println(buttons[i].currentValue);
				display.drawBitmap(89, 19, icon, 10, 9, WHITE);
			}
		}

		// display.setCursor(1, 2);
		// uint16_t txth, txtw;
		// getTextDims("127", &txtw, &txth);
		// display.println("127");

		overlayValue();

		display.display();

		vTaskDelay(pdMS_TO_TICKS(33));
	}
	/*
	vTaskDelete(NULL);  // Deletes this task, so it won’t keep running
	displayTaskHandle = NULL;  // Reset handle
	*/
}

void stopDisplayTask()
{
	if (displayTaskHandle != NULL)
	{
		vTaskDelete(displayTaskHandle); // Stop the task
		displayTaskHandle = NULL;		// Reset handle
		display.clearDisplay();			// clear display buffer
		display.display();				// update display
		Serial.println("display task stopped.");
	}
}

void startDisplayTask(TaskFunction_t task)
{
	if (displayTaskHandle != NULL)
		stopDisplayTask();

	xTaskCreate(
		task,			   // Function to run on this task
		"displayTask",	   // Name of this task
		2048,			   // Stack size (bytes)
		NULL,			   // Parameter to pass
		1,				   // Task priority
		&displayTaskHandle // Task handle
	);
	Serial.println("display task started.");
}