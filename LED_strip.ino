/*
 Name:		LED_strip.ino
 Created:	4/11/2017 3:33:05 PM
 Author:	John
*/

#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN_inc   8   // Digital IO pin connected to the button.  This will be
// driven with a pull-up resistor so the switch should
// pull the pin to ground momentarily.  On a high -> low
// transition the button press logic will execute.
#define BUTTON_PIN_dec   9

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

/*struct state {
	unsigned long wipe = 0;
	//unsigned long vibe = 0;
} global_state;*/

void setup() {
	pinMode(BUTTON_PIN_inc, INPUT_PULLUP);
	pinMode(BUTTON_PIN_dec, INPUT_PULLUP);
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

void loop() {
	// Get current button state.
	bool StateInc = digitalRead(BUTTON_PIN_inc);
	bool StateDec = digitalRead(BUTTON_PIN_dec);
	// Check if state changed from high to low (button press).
	if (StateInc == LOW) {
		showType++;
		if (showType > 9)
			showType = 0;
		startShow(showType);
	}

	if (StateDec == LOW) {
		showType--;
		if (showType < 0)
			showType = 9;
		startShow(showType);
	}
}

void startShow(int i) {
	switch (i) {
	case 0: colorWipe(strip.Color(0, 0, 0), 5);
		break;
	case 1: colorWipe(strip.Color(255, 0, 0), 50);  // Red
		colorWipe(strip.Color(0, 0, 0), 50);
		break;
	case 2: colorWipe(strip.Color(0, 255, 0), 50);  // Green
		colorWipe(strip.Color(0, 0, 0), 50);
		break;
	case 3: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
		colorWipe(strip.Color(0, 0, 0), 50);
		break;
	case 4: theaterChase(strip.Color(127, 127, 127), 75); // White
		colorWipe(strip.Color(0, 0, 0), 5);
		break;
	case 5: theaterChase(strip.Color(127, 0, 0), 75); // Red
		colorWipe(strip.Color(0, 0, 0), 5);
		break;
	case 6: theaterChase(strip.Color(0, 0, 127), 75); // Blue
		colorWipe(strip.Color(0, 0, 0), 5);
		break;
	case 7: rainbow(5);
		colorWipe(strip.Color(0, 0, 0), 50);
		break;
	case 8: rainbowCycle(5);
		colorWipe(strip.Color(0, 0, 0), 50);
		break;
	case 9: theaterChaseRainbow(75);
		colorWipe(strip.Color(0, 0, 0), 5);
		break;
	}
	
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
	long wipe_time = millis();
	for (uint16_t i = 0; i<strip.numPixels(); i++) {
		strip.setPixelColor(i, c);
		if (millis() - wipe_time > wait) {
			strip.show();
			i++;
			wipe_time = millis();
		}
		i--;
	}
}

void rainbow(uint8_t wait) {
	uint16_t i, j;
	long bow_time = millis();
	for (j = 0; j<256; j++) {
		for (i = 0; i<strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel((i + j) & 255));
		}
		if (millis() - bow_time > wait) {
			strip.show();
			j++;
			bow_time = millis();
		}
		j--;
	}
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
	uint16_t i, j;
	long bow_cycle_time = millis();
	for (j = 0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
		for (i = 0; i< strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
		}
		if (millis() - bow_cycle_time > wait) {
			strip.show();
			j++;
			bow_cycle_time = millis();
		}
		j--;
	}
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
	for (int j = 0; j<10; j++) {  //do 10 cycles of chasing
		long chase_time = millis();
		for (int q = 0; q < 3; q++) {
			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + q, c);    //turn every third pixel on
			}
			if (millis() - chase_time > wait) {
				strip.show();
				q++;
				chase_time = millis();
			}
			q--;

			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + q, 0);        //turn every third pixel off
			}
		}
	}
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
	for (int j = 0; j < 256; j += 4) {     // cycle all 256 colors in the wheel
		long chase_time = millis();
		for (int q = 0; q < 3; q++) {
			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + q, Wheel((i + j) % 255));    //turn every third pixel on
			}
			if (millis() - chase_time > wait) {
				strip.show();
				q++;
				chase_time = millis();
			}
			q--;

			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + q, 0);        //turn every third pixel off
			}
		}
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}