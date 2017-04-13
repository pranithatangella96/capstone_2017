#include <pcmRF.h>
#include<SPI.h>
#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin 53  
#define SD_ChipSelectPin 10  
#include <TMRpcm.h>          
TMRpcm tmrpcm;   // create an object for use in this sketch
char mychar;
void setup(){

  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno

  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  
  tmrpcm.setVolume(7);//the sound file "music" will play each time the arduino powers up, or is reset
  tmrpcm.loop(1);
  
  Serial.println("BEFORE PLAY");
  Serial.println(tmrpcm.isPlaying());
  tmrpcm.play("4.wav");
  Serial.println(tmrpcm.isPlaying());
  Serial.println("after PLAY");
  
  
}

void loop(){
  
  
 /* if(Serial.available()){   
    mychar = Serial.read();
  }
  if(mychar == 'k'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play("1");
    } else if(mychar == 'a'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play("2");
    } else if(mychar == 'l'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play("3");
    } else if(mychar == 'p'){
      tmrpcm.play("4");
    }
    else if(mychar == 'h'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play("5");
    }
    else if(mychar == 'v'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play("6");
    }*/
}
