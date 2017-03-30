#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//byte buffer_in[13]; 
//buffer for receiving position
//byte height_char;
//float height = 0;
//float height1 = 0;
float height_main = 0;
//up and down command strings
byte command_up[5] = {216, 216, 102, 2, 2}; 
byte command_down[5] = {216, 216, 102, 1, 1};

int serial_delay = 750;

//blinkScreen variables
int screen_state = 1;
unsigned long previous_millis = 0;
long blink_time = 500;

//defining buttons
int up_button = 8;
int down_button = 9;

int preset1_button = 6;
//int preset2_button
//int preset3_button
//int preset4_button

int preset_set = 7;

//preset values
float preset1 = 0;
float preset2 = 0;
float preset3 = 0;
float preset4 = 0;


float heightNow()
{
  byte buffer_in[13]; //buffer for receiving position
  byte height_char;
  float height = 0;
  float height_float = 0;
  
  if (Serial.available()) {
    // reading in height
    Serial.readBytes(buffer_in,13);
  }
  
  height_char = buffer_in[12];
  // seting the height from the input
  height_float = (float)height_char;
  height = 27.9 + ((height_float - 71.0)*0.4);
  // height correction
  if (height > 31.4){
    height = height - 0.1; 
  }
  if (height > 37.7){
    height = height - 0.1;
  }
  if (height > 44){
    height = height - 0.1;
  }
  return height;
}

void preset (float destination)
{
  float height_now = heightNow();

  if (destination == 0){
    return;
  }
  while(height_now != destination){
    height_now = heightNow();
    
    if ((height_now > destination)&&(destination > 27.9)){
      Serial.write(command_down, 5);
      delay(serial_delay);
    }
    else if((height_now < destination)&&(destination < 46.8)){
      Serial.write(command_up, 5);
      delay(serial_delay);
    }
    else;
  }  
  return;
}

void setPreset()
{
  int preset_set = 1; //to create loop for screen blinking
  
  while(preset_set){
    blinkScreen();
    
    if (preset1_button == LOW){
      preset1 = heightNow();
      preset_set=0;
    }
//    if (preset2_button == LOW){
//      preset2 = heightNow();
//      preset_set=0;
//    }
//    if (preset3_button == LOW){
//      preset3 = heightNow();
//      preset_set=0;
//    }
//    if (preset4_button == LOW){
//      preset4 = heightNow();
//      preset_set=0;
//    }
  }
  
  return;
}

//blinkScreen is a slightly modified chunk of code from the Flasher.Update() command from https://learn.adafruit.com/multi-tasking-the-arduino-part-1/all-together-now
void blinkScreen()
{
  unsigned long current_millis = millis();
  
  if((screen_state == 1) && (current_millis - previous_millis >= blink_time))
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

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize serial communications
  Serial.begin(9600);
  //setting buttons as pull up
  pinMode(up_button, INPUT);
  pinMode(down_button, INPUT);
  pinMode(preset1_button, INPUT);
  //pinMode(preset2_button, INPUT);
  //pinMode(preset3_button, INPUT);
  //pinMode(preset4_button, INPUT);
  pinMode(preset_set, INPUT);

  delay(1000);
}

void loop() {

  height_main = heightNow();
  
  //up and down by button
  if ((digitalRead(up_button)==LOW)&&(height_main < 46.8)) {
    Serial.write(command_up, 5);
    delay(serial_delay);
  }
  if ((digitalRead(down_button)==LOW)&&(height_main > 27.9)) {
    Serial.write(command_down, 5);
    delay(serial_delay);
  }
  
  //setting preset
  if (preset_set == LOW){
    delay(1000);
    if(preset_set == LOW){
      setPreset();
    }
  }

  //calling presets
  if (preset1_button == LOW){
    preset(preset1);
  }
//  if (preset2_button == LOW){
//    preset(preset2);
//  }
//  if (preset3_button == LOW){
//    preset(preset3);
//  }
//  if (preset4_button == LOW){
//    preset(preset4);
//  }

  //displaying value
  lcd.clear();
  lcd.print(heightNow());
}
