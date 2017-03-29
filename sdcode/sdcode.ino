#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 10  
#include <TMRpcm.h>          
TMRpcm tmrpcm;   // create an object for use in this sketch
char mychar;
int light =3;
void setup(){

  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc

  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  tmrpcm.play((char*)("1.wav")); //the sound file "music" will play each time the arduino powers up, or is reset
  pinMode(light, OUTPUT);
}

void loop(){  
  if(SD.exists("1.wav")){
    digitalWrite(light, HIGH);
  }
  if(Serial.available()){   
    mychar = Serial.read();

    if(mychar == 'o'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play((char*)("1.wav"));
    } else if(mychar == 'r'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play((char*)("2.wav"));
    } else if(mychar == 'q'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play((char*)("3.wav"));
    } else if(mychar == 'p'){
      tmrpcm.play((char*)("4.wav"));
    }
    else if(mychar == 'w'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play((char*)("5.wav"));
    }
    else if(mychar == 't'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play((char*)("6.wav"));
    }
    
  }

}
