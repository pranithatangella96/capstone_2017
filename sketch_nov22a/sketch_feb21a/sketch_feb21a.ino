#include<liquidcrystal.h>
liquidcrystal lcd(12,11,5,4,3,2);
void setup() {
  // put your setup code here, to run once:
lcd.begin(16,2);
lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
lcd.print("printing text");
delay(3000);
lcd.clear();
lcd.setCursor(0,1);
lcd.print("setting cursor");
delay(3000);
lcd.clear();
lcd.print("blink cursor");
lcd.blink();
delay(3000);
lcd.print("uline cursor");
lcd.cursor();
delay(3000);
lcd.clear();
lcd.print("no cursor");
lcd.noCursor();
delay(3000);
lcd.clear();
lcd.print("no display");
delay(1000);
led.noDisplay();
lcd.clear();
lcd.print("display on");
delay(3000);
lcd.display();
delay(300);
}

