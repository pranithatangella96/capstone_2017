/*
 Name:		LED_strips_v2.ino
 Created:	4/11/2017 4:11:25 PM
 Author:	John
*/

/*CODE TAKEN FROM: https://learn.adafruit.com/multi-tasking-the-arduino-part-3/put-it-all-together-dot-dot-dot */


#include <Adafruit_NeoPixel.h>

//light variables
int button_light_inc = 8;
int button_light_dec = 9;
#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 60
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

struct state {

	struct LIGHTS {
		int on = 1;
		unsigned long time = 0;
		struct STATE {
			int active = 0;
			int interrupt = 0;
			int interrupt_reset = 0;
			int count = 0;
		} state;
		struct WIPE {
			int count = 0;
			unsigned long time = 0;
		} wipe;
		struct THEATER_CHASE {
			unsigned long time = 0;
			int count = 0;
			int cycle = 0;
		} theater_chase;
		struct RAINBOW {
			unsigned long time = 0;
			int count = 0;
		} rainbow;
		struct RAINBOW_CYCLE {
			unsigned long time = 0;
			int count = 0;
		} rainbow_cycle;
		struct THEATER_CHASE_RAINBOW {
			unsigned long time = 0;
			int count = 0;
			int cycle = 0;
		} theater_chase_rainbow;

	} lights;
} global_state;

void startShow(int i) {
	switch (i) {
	case 0: if (global_state.lights.on == 0)
		colorWipeOff(strip.Color(0, 0, 0), 1);
		break;
	case 1: {
		if (global_state.lights.on) {
			colorWipeOn(strip.Color(255, 0, 0), 50);  // Red
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 50);
		}
	}
			break;
	case 2: {
		if (global_state.lights.on) {
			colorWipeOn(strip.Color(0, 255, 0), 50);  // Green
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 50);
		}
	}
			break;
	case 3: {
		if (global_state.lights.on) {
			colorWipeOn(strip.Color(0, 0, 255), 50);  // Blue
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 50);
		}
	}
			break;
	case 4: {
		if (global_state.lights.on) {
			theaterChase(strip.Color(127, 127, 127), 75); // White
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 2);
		}
	}
			break;
	case 5: {
		if (global_state.lights.on) {
			theaterChase(strip.Color(127, 0, 0), 75); // Red
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 2);
		}
	}
			break;
	case 6: {
		if (global_state.lights.on) {
			theaterChase(strip.Color(0, 0, 127), 75); // Blue
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 2);
		}
	}
			break;
	case 7: {
		if (global_state.lights.on) {
			rainbow(5);
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 2);
		}
	}
			break;
	case 8: {
		if (global_state.lights.on) {
			rainbowCycle(5);
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 2);
		}
	}
			break;
	case 9: {
		if (global_state.lights.on) {
			theaterChaseRainbow(75);
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 2);
		}
	}
			break;
	default:
		colorWipeOff(strip.Color(0, 0, 0), 2);
	}

}
void initializeLights() {
	global_state.lights.wipe.time = millis();
	global_state.lights.theater_chase.time = millis();
	global_state.lights.rainbow.time = millis();
	global_state.lights.rainbow_cycle.time = millis();
	global_state.lights.theater_chase_rainbow.time = millis();
}

void resetLights() {
	global_state.lights.wipe.count = 0;
	global_state.lights.theater_chase.count = 0;
	global_state.lights.theater_chase.cycle = 0;
	global_state.lights.rainbow.count = 0;
	global_state.lights.rainbow_cycle.count = 0;
	global_state.lights.theater_chase_rainbow.count = 0;
	global_state.lights.theater_chase_rainbow.cycle = 0;
}

void colorWipeOn(uint32_t c, uint8_t wait) {

	if (global_state.lights.wipe.count < strip.numPixels()) {
		strip.setPixelColor(global_state.lights.wipe.count, c);
		if (millis() - global_state.lights.wipe.time > wait) {
			strip.show();
			global_state.lights.wipe.count++;
			global_state.lights.wipe.time = millis();
		}
	}
	else {
		resetLights();
		global_state.lights.on = 0;
	}
}

void colorWipeOff(uint32_t c, uint8_t wait) {

	if (global_state.lights.wipe.count < strip.numPixels()) {
		strip.setPixelColor(global_state.lights.wipe.count, c);
		if (millis() - global_state.lights.wipe.time > wait) {
			strip.show();
			global_state.lights.wipe.count++;
			global_state.lights.wipe.time = millis();
		}
	}
	else {
		resetLights();
		global_state.lights.on = 1;
		global_state.lights.state.active = 0;
		global_state.lights.state.interrupt = 0;
	}
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
	if (global_state.lights.theater_chase.cycle < 10) {  //do 10 cycles of chasing
		if (global_state.lights.theater_chase.count< 3) {
			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + global_state.lights.theater_chase.count, c);    //turn every third pixel on
			}
			if (millis() - global_state.lights.theater_chase.time > wait) {
				strip.show();
				global_state.lights.theater_chase.count++;
				global_state.lights.theater_chase.time = millis();
			}
			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + global_state.lights.theater_chase.count+1, 0);        //turn every third pixel off
			}
			if (global_state.lights.theater_chase.count >= 3) {
				global_state.lights.theater_chase.count = 0;
				global_state.lights.theater_chase.cycle++;
			}
		}
	}
	else {
		resetLights();
		global_state.lights.on = 0;
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

void rainbow(uint8_t wait) {
	if (global_state.lights.rainbow.count < 256) {
		for (uint16_t i = 0; i < strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel((i + global_state.lights.rainbow.count) & 255));
		}
		if (millis() - global_state.lights.rainbow.time > wait) {
			strip.show();
			global_state.lights.rainbow.count++;
			global_state.lights.rainbow.time = millis();
		}
	}
	else {
		resetLights();
		global_state.lights.on = 0;
	}
}

void rainbowCycle(uint8_t wait) {

	if (global_state.lights.rainbow_cycle.count < 256) { // 1 cycle of all colors on wheel
		for (uint16_t i = 0; i< strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + global_state.lights.rainbow_cycle.count) & 255));
		}
		if (millis() - global_state.lights.rainbow_cycle.time> wait) {
			strip.show();
			global_state.lights.rainbow_cycle.count++;
			global_state.lights.rainbow_cycle.time = millis();
		}
	}
	else {
		resetLights();
		global_state.lights.on = 0;
	}
}

void theaterChaseRainbow(uint8_t wait) {
	if (global_state.lights.theater_chase_rainbow.cycle < 256) {     // cycle all 256 colors in the wheel
		if (global_state.lights.theater_chase_rainbow.count < 3) {
			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + global_state.lights.theater_chase_rainbow.count, Wheel((i + global_state.lights.theater_chase_rainbow.cycle) & 255));    //turn every third pixel on
			}
			if (millis() - global_state.lights.theater_chase_rainbow.time > wait) {
				strip.show();
				global_state.lights.theater_chase_rainbow.count++;
				global_state.lights.theater_chase_rainbow.time = millis();
			}
			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + global_state.lights.theater_chase_rainbow.count + 1, 0);        //turn every third pixel off
			}
			if (global_state.lights.theater_chase_rainbow.count >= 3) {
				global_state.lights.theater_chase_rainbow.count = 0;
				global_state.lights.theater_chase_rainbow.cycle += 4;
			}
		}
	}
	else {
		resetLights();
		global_state.lights.on = 0;
	}
}

void timer() {
	global_state.lights.time = millis();
}

void timerReset() {
	global_state.lights.time = 0;
}

void setup() {
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
	pinMode(button_light_inc, INPUT);
	pinMode(button_light_dec, INPUT);
	digitalWrite(button_light_dec, LOW);
	digitalWrite(button_light_inc, LOW);
}

void loop() {

	if (digitalRead(button_light_inc) == HIGH && global_state.lights.time == 0) {
		if (global_state.lights.state.active) {
			global_state.lights.state.interrupt = 1;
			global_state.lights.state.interrupt_reset = 1; 
			global_state.lights.state.count--;
		}
		timer();
		global_state.lights.state.count++;
		if (global_state.lights.state.count > 9)
			global_state.lights.state.count = 0;
		global_state.lights.state.active = global_state.lights.state.count;
		initializeLights();
	}
	if (digitalRead(button_light_dec) == HIGH && global_state.lights.time == 0) {
		if (global_state.lights.state.active) {
			global_state.lights.state.interrupt = 1;
			global_state.lights.state.interrupt_reset = 1;
			global_state.lights.state.count++;
		}
		timer();
		global_state.lights.state.count--;
		if (global_state.lights.state.count < 0)
			global_state.lights.state.count = 9;
		global_state.lights.state.active = global_state.lights.state.count;
		initializeLights();
	}

	if (global_state.lights.state.interrupt) {
		if (global_state.lights.state.interrupt_reset) {
			resetLights();
			global_state.lights.state.interrupt_reset = 0;
			global_state.lights.on = 0;
		}
		startShow(0);
	}
	else {
		startShow(global_state.lights.state.active);
	}
	if (global_state.lights.time != 0 && millis() - global_state.lights.time > 500)
		timerReset();

}
