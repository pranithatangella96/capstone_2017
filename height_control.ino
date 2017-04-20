#include <LiquidCrystal.h>
#define SEC5_MSEC 5000

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//variables for height now
byte buffer_in[13]; //buffer for receiving position
byte height_char;
float height = 0;
float height_float = 0;
//variables for button response
int vibe_pin = 22;
int light_pin = 23;
int music_pin=24;
int fan_pin=25;
int button = 26;
int button_state = 0;
int vibe_state = LOW;
long interval = 500;
unsigned long prev_millis;
unsigned long current_millis;

float height_print;
//up and down command strings
byte command_up[13] = { 216, 216, 102, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0};
byte command_down[13] = { 216, 216, 102, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};

int serial_delay = 500;

//blinkScreen variables
//int screen_state = 1;
//unsigned long previous_millis = 0;
//long blink_time = 500;

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
struct state {
  unsigned long light = 0;
  unsigned long vibe = 0;
  unsigned long music = 0;
  unsigned long fan =0;
} global_state;

//void vibe() {
//  current_millis = millis();
//
//  if ((vibe_state == HIGH) && (current_millis - prev_millis >= interval))
//  {
//    vibe_state = LOW;  // Turn it off
//    digitalWrite(vibe_pin, vibe_state);  // Update the actual LED
//  }
//  
//}

void start_light() {
  digitalWrite(light_pin, HIGH);
  global_state.light = millis();
}
void start_music()
{
  digitalWrite(music_pin,HIGH);
  global_state.music=millis();
}
void start_fan()
{
  digitalWrite(fan_pin,HIGH);
  global_state.fan=millis();
}

void start_vibe() {
  digitalWrite(vibe_pin, HIGH);
  global_state.vibe = millis();
}

void end_light() {
  digitalWrite(light_pin, LOW);
  global_state.light = 0;
}
void end_music(){
  digitalWrite(music_pin,LOW);
  global_state.music=0;
}
void end_fan(){
  digitalWrite(fan_pin,LOW);
  global_state.fan=0;
}
void end_vibe() {
  digitalWrite(vibe_pin, LOW);
  global_state.vibe = 0;
}

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
  while ((height_now >= destination+.3)||(height_now <= destination-.3)) {
    height_now = heightNow();

    if ((height_now >= destination-.3) && (destination >= 27.9)) {
      Serial1.write(command_down, 13);
      
    }
    else if ((height_now <= destination+.3) && (destination <= 46.8)) {
      Serial1.write(command_up, 13);
      
    }
    else;
  }
  return;
}

void setPreset() {
  //int preset_trig = 1; //to create loop for screen blinking

  //while (preset_trig) {
    //blinkScreen();

    if (digitalRead(preset1_button) == LOW) {
      preset1 = heightNow();
      //preset_trig = 0;
    }
    //    if (digitalRead(preset2_button) == LOW){
    //      preset2 = heightNow();
    //      preset_trig=0;
    //    }
    //    if (digitalRead(preset3_button) == LOW){
    //      preset3 = heightNow();
    //      preset_trig=0;
    //    }
    //    if (digitalRead(preset4_button) == LOW){
    //      preset4 = heightNow();
    //      preset_trig=0;
    //    }
  //}

  return;
}

//blinkScreen is a slightly modified chunk of code from the Flasher.Update() command from https://learn.adafruit.com/multi-tasking-the-arduino-part-1/all-together-now
//void blinkScreen()
//{
//  unsigned long current_millis = millis();
//
//  if ((screen_state == 1) && (current_millis - previous_millis >= blink_time))
//  {
//    screen_state = 0;
//    previous_millis = current_millis;  // Remember the time
//    lcd.noDisplay();
//  }
//  else if ((screen_state = 0) && (current_millis - previous_millis >= blink_time))
//  {
//    screen_state = 1;
//    previous_millis = current_millis;   // Remember the time
//    lcd.display();
//  }
//}

void setup() {

  delay(1000);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize serial communications
  Serial1.begin(9600);
  //setting buttons as pull up
  pinMode(up_button, INPUT);
  pinMode(down_button, INPUT);
  pinMode(preset1_button, INPUT);
  //pinMode(preset2_button, INPUT);
  //pinMode(preset3_button, INPUT);
  //pinMode(preset4_button, INPUT);
  pinMode(preset_set, INPUT);
   pinMode(vibe_pin, OUTPUT);
  pinMode(light_pin, OUTPUT);
  pinMode(music_pin,OUTPUT);
  pinMode(button, INPUT);


}

void loop() {

  //displaying value
  height_print = heightNow();
  lcd.clear();
  lcd.print(height_print);

  //up and down by button
  if ((digitalRead(up_button) == LOW) && (height < 46.8)) {
    Serial1.write(command_up, 13);
    delay(serial_delay);
  }
  if ((digitalRead(down_button) == LOW) && (height > 27.9)) {
    Serial1.write(command_down, 13);
    delay(serial_delay);
  }
  
  //setting preset
  if (digitalRead(preset_set) == LOW) {
    setPreset();
  }

  //calling presets
  if (digitalRead(preset1_button) == LOW) {
    preset(preset1);
  }
  //  if (digitalRead(preset2_button) == LOW){
  //    preset(preset2);
  //  }
  //  if (digitalRead(preset3_button) == LOW){
  //    preset(preset3);
  //  }
  //  if (digitalRead(preset4_button) == LOW){
  //    preset(preset4);
  //  }
  // put your main code here, to run repeatedly:

  
  // read our inputs here, as of now we only have button, should be consecutive if's not elseifs  
  if (digitalRead(button) == LOW){
    start_light();
    start_vibe();
  }
  // after reading inputs, see if we have to turn off any outputs
  current_millis = millis();
  if ( global_state.light != 0 && current_millis - global_state.light > 3000 )
    end_light();
  if ( global_state.vibe != 0 && current_millis - global_state.vibe > SEC5_MSEC )
    end_vibe();
else if(digitalRead(music_pin)==LOW){
  start_music();
  start_vibe();
}
current_millis= millis();
if(global_state.music !=0&& current_millis-global_state.music>5000)
end_music();
if ( global_state.vibe != 0 && current_millis - global_state.vibe > SEC5_MSEC )
    end_vibe();
    
else if(digitalRead(fan_pin)==LOW){
  start_fan();
  start_vibe();
}
current_millis=millis();
if(global_state.fan !=0&& current_millis - global_state.fan>6000)
end_fan();
if(global_state.vibe!=0&&current_millis -global_state.vibe>500)
end_vibe();
}

