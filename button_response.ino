#define SEC5_MSEC 5000

int vibe_pin = 11;
int light_pin = 12;
int button = 10;
int button_state = 0;
int vibe_state = LOW;
long interval = 500;
unsigned long prev_millis;
unsigned long current_millis;

/*
 * Global state will have 0 if things aren't running and the time
 * that we started them if they are running
 * 
 * This is meant to keep track of the state of our machine and should
 * be available to the entire program.
 */
struct state {
	unsigned long light = 0;
	unsigned long vibe = 0;
} global_state;

//void vibe() {
//	current_millis = millis();
//
//	if ((vibe_state == HIGH) && (current_millis - prev_millis >= interval))
//	{
//		vibe_state = LOW;  // Turn it off
//		digitalWrite(vibe_pin, vibe_state);  // Update the actual LED
//	}
//	
//}

void start_light() {
	digitalWrite(light_pin, HIGH);
	global_state.light = millis();
}

void start_vibe() {
	digitalWrite(vibe_pin, HIGH);
	global_state.vibe = millis();
}

void end_light() {
	digitalWrite(light_pin, LOW);
	global_state.light = 0;
}

void end_vibe() {
	digitalWrite(vibe_pin, LOW);
	global_state.vibe = 0;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(vibe_pin, OUTPUT);
  pinMode(light_pin, OUTPUT);
  pinMode(button, INPUT);
}
void loop() {
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

}
