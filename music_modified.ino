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
#define sd_error 0

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

int song=0;
  
void setup() {
  pinMode(INC_PIN,INPUT_PULLUP);
  pinMode(DEC_PIN,INPUT_PULLUP);
  pinMode(1,OUTPUT);
  pinMode(0,OUTPUT);
  //attachInterrupt digitalPinToInterrupt(3);
//attachInterrupt digitalPinToInterrupt(2);
  delay(1000);
if (! musicPlayer.begin()) { // initialise the music player
     digitalWrite(music_error,HIGH);
     while (1);
  }
 // Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
   // Serial.println(F("SD failed, or not present"));
   digitalWrite(sd_error,HIGH);
    while (1);  // don't do anything more
  }
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);
  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  musicPlayer.playFullFile("track001.mp3");
}
void loop(){
  }

void increment(){
  //stop playing
  //increment to next song
  song++;
  if(song>6)
  song=0;
  }
  void decrement(){
    song--;   
    if(song<0)
    song=6;
    }
    void startsong(int i)
    {
      switch(i){
               case 0:musicPlayer.playFullFile("none.mp3");
               break;
               case 1: musicPlayer.playFullFile("track001.mp3");
               break;
                case 2:musicPlayer.playFullFile("track002.mp3");
                break;
                case 3:musicPlayer.playFullFile("track003.mp3");
                break;
                case 4:musicPlayer.playFullFile("track004.mp3");
                break;
                case 5: musicPlayer.playFullFile("track005.mp3");
                break;
                case 6:musicPlayer.playFullFile("track006.mp3");
                break;          
  
      }
    }

