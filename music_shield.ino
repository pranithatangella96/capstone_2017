/*
 Name:		music_shield.ino
 Created:	4/25/2017 3:23:01 PM
 Author:	John
*/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
// define the pins used
#define CLK 13       // SPI Clock, shared with SD card
#define MISO 12      // Input data, from VS1053/SD card
#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)
// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin
#define INC_PIN   2
#define DEC_PIN   3
#define music_error 1
#define sd_error 5

Adafruit_VS1053_FilePlayer musicPlayer =
// create shield-example object!
Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void setup() {
	pinMode(INC_PIN, INPUT);
	//pinMode(DEC_PIN, INPUT);
	digitalWrite(INC_PIN, LOW);
	pinMode(1, OUTPUT);
	pinMode(5, OUTPUT);
	//attachInterrupt(digitalPinToInterrupt(3), increment, LOW);
	//attachInterrupt(digitalPinToInterrupt(2), increment, LOW);
	if (!musicPlayer.begin()) { // initialise the music player
		digitalWrite(music_error, HIGH);
		while (1);
	}
	// Serial.println(F("VS1053 found"));

	if (!SD.begin(CARDCS)) {
		// Serial.println(F("SD failed, or not present"));
		digitalWrite(sd_error, HIGH);
		while (1);  // don't do anything more
	}
	// Set volume for left, right channels. lower numbers == louder volume!
	musicPlayer.setVolume(1, 1);
	// If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
	// audio playing
	musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
	//musicPlayer.playFullFile("track001.mp3");
	musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
}
int song = 0;
int state = 0;
unsigned long time = 0;

void timer() {
	time = millis();
}

void timerReset() {
	time = 0;
}
void loop() {
	if (state) {
		startSong(song);
	}
	if (musicPlayer.playingMusic) {
		state = 0;
	}
	if (digitalRead(INC_PIN) == HIGH && time == 0) {
		timer();
		increment();
	}
	if (time != 0 && millis() - time > 500)
		timerReset();
}

void increment() {
	musicPlayer.stopPlaying();
	song++;
	if (song > 6)
		song = 0;
	state = 1;
}

//void decrement() {
//	musicPlayer.stopPlaying();
//	song--;
//	if (song<0)
//		song = 6;
//	song = song;
//	state = 1;
//}

void startSong(int i)
{
	switch (i) {
	case 0:musicPlayer.startPlayingFile("none.mp3");
		break;
	case 1:musicPlayer.startPlayingFile("track001.mp3");
		break;
	case 2:musicPlayer.startPlayingFile("track002.mp3");
		break;
	case 3:musicPlayer.startPlayingFile("track003.mp3");
		break;
	case 4:musicPlayer.startPlayingFile("track004.mp3");
		break;
	case 5:musicPlayer.startPlayingFile("track005.mp3"); 
		break;
	case 6:musicPlayer.startPlayingFile("track006.mp3");
		break;
	}
}