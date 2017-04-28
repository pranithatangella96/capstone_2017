/*
 Name:		Integrated_capstone_solution.ino
 Created:	4/13/2017 2:30:25 PM
 Author:	John
*/
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#define LEG_SERIAL_DELAY 895

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//variables for height now
byte buffer_in[13]; //buffer for receiving position
byte height_char;
float height = 0;
float height_float = 0;

//up and down command strings
byte command_up[13] = { 216, 216, 102, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
byte command_down[13] = { 216, 216, 102, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };

int serial_delay = 500;

//blinkScreen variables
int screen_state = 1;
unsigned long previous_millis = 0;
long blink_time = 500;

//defining buttons
int up_button = 8;
int down_button = 9;

int preset1_button = 6;
int preset2_button = 22;
int preset3_button = 23;
int preset4_button = 24;

int preset_set = 25;

//preset values
float preset1 = 0;
float preset2 = 0;
float preset3 = 0;
float preset4 = 0;

//timing for height controls
//unsigned long current_milleseconds = 0;
unsigned long wrote_up = 0;
unsigned long wrote_down = 0;

//fan variables
int fan = 26;
int fan_button = 27;
int vibe_fan = 28;
unsigned long fan_millis = 0;

//light variables
int button_light_inc = 30;
int button_light_dec = 31;
#define PIXEL_PIN    29    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 60
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
int vibe_light_inc = 32;
int vibe_light_dec = 33;

//music variables
int button_music_inc = 34;
int button_music_dec = 35;
int vibe_music_inc = 36;
int vibe_music_dec = 37;
int music_inc = 38;
int music_dec = 39;
int error_lib = 40;
int error_sd = 41;

//struct for all states of things
struct state {
	unsigned long fan = 0;
	struct VIBE {
		unsigned long fan = 0;
		unsigned long light_dec = 0;
		unsigned long light_inc = 0;
		unsigned long music_inc = 0;
		unsigned long music_dec = 0;
	} vibe;

	struct LIGHTS {
		int on = 1;
		struct STATE {
			int active = 0;
			int interrupt = 0;
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

float heightNow() {

	int i;

	if (Serial1.available()) {
		// reading in height
		Serial1.readBytes(buffer_in, 13);
	}
	//looking for the correct height values
	for (i = 1; i < 13; i++) {
		if ((buffer_in[i - 1] == buffer_in[i]) && (buffer_in[i] >= 71) && (buffer_in[i] <= 119)) {
			height_char = buffer_in[i];
			i = 12;
		}
	}

	// seting the height from the input
	height_float = (float)height_char;
	height = 27.9 + ((height_float - 71.0)*0.4);
	// height correction
	if (height > 31.4) {
		height = height - 0.1;
	}
	if (height > 37.7) {
		height = height - 0.1;
	}
	if (height > 44) {
		height = height - 0.1;
	}
	return height;
}

void preset(float destination) {
	float height_now = heightNow();
	if (destination == 0) {
		return;
	}
	while (height_now != destination) {
		height_now = heightNow();
		if ((height_now > destination) && (destination >= 27.9) && millis() - wrote_down > LEG_SERIAL_DELAY) {
			Serial1.write(command_down, 13);
			wrote_down = millis();
		}
		else if ((height_now < destination) && (destination <= 46.8) && millis() - wrote_up > LEG_SERIAL_DELAY) {
			Serial1.write(command_up, 13);
			wrote_up = millis();
		}
		else;
	}
	return;
}

void setPreset() {
	int preset_trig = 1; //to create loop for screen blinking

	while (preset_trig) {
	blinkScreen();

	if (digitalRead(preset1_button) == LOW) {
		preset1 = heightNow();
		preset_trig = 0;
	}
	else if (digitalRead(preset2_button) == LOW) {
		preset2 = heightNow();
		preset_trig=0;
	}
	else if (digitalRead(preset3_button) == LOW) {
		preset3 = heightNow();
		preset_trig=0;
	}
	else if (digitalRead(preset4_button) == LOW) {
		preset4 = heightNow();
		preset_trig=0;
	}
	}

	return;
}

//blinkScreen is a slightly modified chunk of code from the Flasher.Update() command from https://learn.adafruit.com/multi-tasking-the-arduino-part-1/all-together-now
void blinkScreen()
{
	unsigned long current_millis = millis();

	if ((screen_state == 1) && (current_millis - previous_millis >= blink_time))
	{
		screen_state = 0;
		previous_millis = current_millis;  // Remember the time
		lcd.noDisplay();
	}
	else if ((screen_state = 0) && (current_millis - previous_millis >= blink_time))
	{
		screen_state = 1;
		previous_millis = current_millis;   // Remember the time
		lcd.display();
	}
}

void startFan() {
	digitalWrite(fan, HIGH);
	global_state.fan = millis();
}

void startVibe(int pin) {
	switch (pin) {
	case (28): {//fan
		digitalWrite(vibe_fan, HIGH);
		global_state.vibe.fan = millis();
	}
			   break;
	case (32):{//light incrament
		digitalWrite(vibe_light_inc, HIGH);
		global_state.vibe.light_inc = millis();
	}
			  break;
	case (33): {//light decrament
		digitalWrite(vibe_light_dec, HIGH);
		global_state.vibe.light_dec = millis();
	}
			   break;
	case (36): {//music incrament
		digitalWrite(vibe_music_inc, HIGH);
		global_state.vibe.music_inc = millis();
	}
			   break;
	case (37): {//music decrament
		digitalWrite(vibe_music_dec, HIGH);
		global_state.vibe.music_dec = millis();
	}
			   break;
	}
}

void endFan() {
	digitalWrite(fan, LOW);
	global_state.fan = 0;
}

void endVibe(int pin) {
	switch (pin) {
	case (28): {//fan
		digitalWrite(vibe_fan, LOW);
		global_state.vibe.fan = 0;
	}
			   break;
	case (32): {//light incrament 
		digitalWrite(vibe_light_inc, LOW);
		global_state.vibe.light_inc = 0;
	}
			   break;
	case (33): {//light decrament
		digitalWrite(vibe_light_dec, LOW);
		global_state.vibe.light_dec = 0;
	}
			   break;
	case (36): {//music incrament
		digitalWrite(vibe_music_inc, LOW);
		global_state.vibe.music_inc = 0;
	}
			   break;
	case (37): {//music decrament
		digitalWrite(vibe_music_dec, LOW);
		global_state.vibe.music_dec = 0;
	}
			   break;
	}
}

void startShow(int i) {
	switch (i) {
	case 0: if(global_state.lights.on == 0)
		colorWipeOff(strip.Color(0, 0, 0), 5);
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
			colorWipeOff(strip.Color(0, 0, 0), 5);
		}
	}
			break;
	case 5: {
		if (global_state.lights.on) {
			theaterChase(strip.Color(127, 0, 0), 75); // Red
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 5);
		}
	}
	case 6: {
		if (global_state.lights.on) {
			theaterChase(strip.Color(0, 0, 127), 75); // Blue
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 5);
		}
	}
	case 7: {
		if (global_state.lights.on) {
			rainbow(5);
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 5);
		}
	}
			break;
	case 8: {
		if (global_state.lights.on) {
			rainbowCycle(5);
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 5);
		}
	}
			break;
	case 9: {
		if (global_state.lights.on) {
			theaterChaseRainbow(75);
		}
		else {
			colorWipeOff(strip.Color(0, 0, 0), 5);
		}
	}
			break;
	default:
		colorWipeOff(strip.Color(0, 0, 0), 5);

	}

}
void initializeLights() {
	global_state.lights.wipe.time = millis();
	global_state.lights.theater_chase.time = millis();
	global_state.lights.rainbow.time = millis();
	global_state.lights.rainbow_cycle.time = millis();
	global_state.lights.theater_chase_rainbow.time = millis();
}

void resetLights(){
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
				strip.setPixelColor(i + global_state.lights.theater_chase.count, 0);        //turn every third pixel off
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
	
	if (global_state.lights.rainbow_cycle.count < 256 * 5) { // 5 cycles of all colors on wheel
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
				strip.setPixelColor(i + global_state.lights.theater_chase_rainbow.count, Wheel((i + global_state.lights.theater_chase_rainbow.cycle) % 255));    //turn every third pixel on
			}
			if (millis() - global_state.lights.theater_chase_rainbow.time > wait) {
				strip.show();
				global_state.lights.theater_chase_rainbow.count++;
				global_state.lights.theater_chase_rainbow.time = millis();
			}
			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + global_state.lights.theater_chase_rainbow.count, 0);        //turn every third pixel off
			}
			if (global_state.lights.theater_chase.count >= 3) {
				global_state.lights.theater_chase_rainbow.count = 0;
				global_state.lights.theater_chase_rainbow.cycle++;
			}
		}
	}
	else {
		resetLights();
		global_state.lights.on = 0;
	}
}

void setup() {

	delay(1000);

	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	// initialize serial communications
	Serial1.begin(9600);

	strip.begin();
	strip.show(); // Initialize all pixels to 'off'

	//setting buttons as pull up
	pinMode(up_button, INPUT);
	pinMode(down_button, INPUT);
	pinMode(preset1_button, INPUT);
	pinMode(preset2_button, INPUT);
	pinMode(preset3_button, INPUT);
	pinMode(preset4_button, INPUT);
	pinMode(preset_set, INPUT);
	pinMode(fan_button, INPUT);
	pinMode(button_light_inc, INPUT);
	pinMode(button_light_dec, INPUT);
	pinMode(button_music_inc, INPUT);
	pinMode(button_light_dec, INPUT);
	pinMode(error_lib, INPUT);
	pinMode(error_sd, INPUT);
	pinMode(fan, OUTPUT);
	pinMode(music_inc, OUTPUT);
	pinMode(music_dec, OUTPUT);
	pinMode(vibe_fan, OUTPUT);
	pinMode(vibe_light_dec, OUTPUT);
	pinMode(vibe_light_inc, OUTPUT);
	pinMode(vibe_music_dec, OUTPUT);
	pinMode(vibe_music_inc, OUTPUT);

}

void loop() {
	//Height control
	//current_milleseconds = millis();
	//displaying value
	lcd.clear();
	lcd.print(String(heightNow(), 1));
	//up and down by button
	if ((digitalRead(up_button) == LOW) && (heightNow() < 46.8) && millis() - wrote_up > LEG_SERIAL_DELAY) {
		Serial1.write(command_up, 13);
		wrote_up = millis();
	}
	if ((digitalRead(down_button) == LOW) && (heightNow() > 27.9) && millis() - wrote_down > LEG_SERIAL_DELAY) {
		Serial1.write(command_down, 13);
		wrote_down = millis();
	}

	//setting preset
	if (digitalRead(preset_set) == LOW) {
		setPreset();
	}

	//calling presets
	if (digitalRead(preset1_button) == LOW) {
		preset(preset1);
	}
	if (digitalRead(preset2_button) == LOW) {
		preset(preset2);
	}
	if (digitalRead(preset3_button) == LOW) {
		preset(preset3);
	}
	if (digitalRead(preset4_button) == LOW) {
		preset(preset4);
	}

	//Fan control
	if (digitalRead(fan_button) == LOW) {
		startFan();
		startVibe(vibe_fan);
	}
		
	if (global_state.fan != 0 && millis() - global_state.fan > 5000)
		endFan();
	if (global_state.vibe.fan != 0 && millis() - global_state.vibe.fan > 500)
		endVibe(vibe_fan);

	//light control
	if (digitalRead(button_light_inc) == LOW) {
		startVibe(vibe_light_inc);
		if (global_state.lights.state.active) {
			global_state.lights.state.interrupt = 1;
		}
		global_state.lights.state.count++;
		if (global_state.lights.state.count > 9)
			global_state.lights.state.count = 0;
		global_state.lights.state.active = global_state.lights.state.count;
	}

	if (digitalRead(button_light_dec) == LOW) {
		startVibe(vibe_light_dec);
		if (global_state.lights.state.active) {
			global_state.lights.state.interrupt = 1;
		}
		global_state.lights.state.count--;
		if (global_state.lights.state.count < 0)
			global_state.lights.state.count = 9;
		global_state.lights.state.active = global_state.lights.state.count;
	}

	if (global_state.lights.state.interrupt) {
		initializeLights();
		startShow(0);
	}
	else {
		initializeLights();
		startShow(global_state.lights.state.active);
	}

	if (global_state.vibe.light_inc != 0 && millis() - global_state.vibe.light_inc > 500)
		endVibe(vibe_light_inc);
	if (global_state.vibe.light_dec != 0 && millis() - global_state.vibe.light_dec > 500)
		endVibe(vibe_light_dec);

	//music control
	// this sets up for the interrupts happen on LOW signal
	digitalWrite(music_inc, HIGH);
	digitalWrite(music_dec, HIGH);

	if (digitalRead(button_music_inc) == LOW) {
		digitalWrite(music_inc, LOW);
		startVibe(vibe_music_inc);
	}
	if (digitalRead(button_music_dec) == LOW) {
		digitalWrite(music_dec, LOW);
		startVibe(vibe_music_dec);
	}

	if (global_state.vibe.music_inc != 0 && millis() - global_state.vibe.music_inc > 500)
		endVibe(vibe_music_inc);
	if (global_state.vibe.music_dec != 0 && millis() - global_state.vibe.music_dec > 500)
		endVibe(vibe_music_dec);
}
