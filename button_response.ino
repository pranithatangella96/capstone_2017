int vibe_pin = 11;
int button = 10;
int button_state = 0;
int vibe_state = LOW;
long interval = 500;
unsigned long prev_millis;
unsigned long current_millis;

void setup() {
  // put your setup code here, to run once:
  pinMode(vibe_pin, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  if (digitalRead(button) == LOW){
	  button_state = 1;
  }
  prev_millis = millis();
  while (button_state) {
	  current_millis = millis();
	  if (current_millis - prev_millis >= interval) {
		  vibe_state = LOW;
		  button_state = 0;
	  }
	  else {
		  vibe_state = HIGH;
	  }
	  digitalWrite(vibe_pin, vibe_state);
  }
  
}
