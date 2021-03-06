#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   8   // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.

#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 60

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;
long current_millis;
long previous_millis;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    //delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      showType++;
      if (showType > 9)
        showType=0;
      startShow(showType);
    }
  }

  // Set the last button state to the old state.
  oldState = newState;
}

void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 50);// Black/off
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
    case 1: colorWipe(strip.Color(255, 0, 0), 50);  // Red
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
    case 2: colorWipe(strip.Color(0, 255, 0), 50);  // Green
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
    case 3: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
    case 4: theaterChase(strip.Color(127, 127, 127), 50); // White
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
    case 5: theaterChase(strip.Color(127,   0,   0), 50); // Red
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
    case 6: theaterChase(strip.Color(  0,   0, 127), 50); // Blue
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
    case 7: rainbow(1);
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
    case 8: rainbowCycle(1);
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
    case 9: theaterChaseRainbow(1);
      previous_millis = millis();
      while(current_millis - previous_millis <= 3000){
        current_millis = millis();
      }
            break;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
   // delay(3);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    //delay(5);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    //delay(5);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<4; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(5);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      //delay(5);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
