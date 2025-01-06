#include "lights_handler.h"

    float heartbeatStrength = HEARTBEAT_STRENGTH_DISCONNECTED;
    float heartbeatSpeed = HEARTBEAT_SPEED_DISCONNECTED;

// Internal state for updateDotstar
static unsigned long lastPress = 0; // Timestamp for the last button press

// create dotstar controller
Adafruit_DotStar dotstar(1, DOTSTAR_DATA_PIN, DOTSTAR_CLOCK_PIN);

void setupLights() {
    dotstar.begin();
    dotstar.setBrightness(32); // Adjust brightness as needed
    dotstar.clear();

    HwPWM0.addPin(LED_BUILTIN);
    HwPWM0.begin();
    HwPWM0.setResolution(15);
    HwPWM0.setClockDiv(PWM_PRESCALER_PRESCALER_DIV_2); // 8MHz
}

void updateLED() {
  static float heartbeatPeriod = 0.0; // Angle (or period) for sine wave
  static unsigned long lastIter = 0; // timestamp for elapsed time
  if ((millis() - lastIter) >= 10) { // if 10ms have passed:
    float brightness = (sin(heartbeatPeriod) + 1) / 2 * heartbeatStrength; // Normalize sine wave to 0-1 range
    const int maxValue = bit(15) - 1; // 15-bit PWM max value
    int pwmValue = brightness * maxValue; // Scale to PWM max value
    // Write PWM value to the LED
    HwPWM0.writePin(LED_BUILTIN, pwmValue);
    // Increment angle for the next loop iteration
    static float incrementBase = TWO_PI / 100; // multiply by hz for proper increments
    heartbeatPeriod += heartbeatSpeed * incrementBase; // Adjust this for speed of pulsing
    if (heartbeatPeriod > TWO_PI) heartbeatPeriod -= TWO_PI;
    // save timestamp for elapsed time calc
    lastIter = millis();
  }
}

void updateDotstar() {
  unsigned long elapsed = millis() - lastPress;
  static float release = 100; // release time for light in ms
  static float exp = 0.5; // exponent for falloff
  static unsigned long lastIter = 0; // timestamp for elapsed time

  if (elapsed == 0) { //immediate response
    dotstar.setBrightness(255);
    dotstar.setPixelColor(0, dotstar.gamma32(dotstar.ColorHSV(random(65536)))); // random color each hit
    dotstar.show();
  } else if ((millis() - lastIter) >= 10) { // update every 10ms
    float brightness = -pow((elapsed/release), exp) + 1.0; // translate elapsed ms to 1-0 range
    if (brightness < 0.0) brightness = 0.0;
    int brightUnit = brightness * 255; // Scale to 0-255
    dotstar.setBrightness(brightUnit);
    dotstar.show();
    lastIter = millis(); // update timestamp
  }
}

// Update the last press time (to be called when needed)
void updateLastButtonPress() {
    lastPress = millis();
}


void flashConnected() {
    heartbeatStrength = HEARTBEAT_STRENGTH_CONNECTED;
    heartbeatSpeed = HEARTBEAT_SPEED_CONNECTED;
}

void flashDisconnected() {
    heartbeatStrength = HEARTBEAT_STRENGTH_DISCONNECTED;
    heartbeatSpeed = HEARTBEAT_SPEED_DISCONNECTED;
}