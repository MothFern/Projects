//adding required libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>

//initializing variables
int X_right_pin = A3; //right joystick, x direction pin 
int Y_right_pin = A4; //right joystick, y direction pin
int SW_right_pin = A5; //right joystick button pin
int X_left_pin = A0; //left joystick x direction pin
int Y_left_pin = A1; //left joystick y direction pin
int SW_left_pin = A2; //left joystick button pin

int X_right_val; //open variable for right joystick x direction to read/assign
int Y_right_val; //open variable for right joystick y direction to read/assign 
int SW_right_state; //open variable for right joystick button to read/assign 
int X_left_val; //open variable for left joystick x direction to read/assign
int Y_left_val; //open variable for left joystick y direction to read/assign 
int SW_left_state; //open variable for left joystick button to read/assign

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); //LCD screen interface pins
unsigned long startTime = 0;  // initial startTime timing variable starting at 0
unsigned long duration = 0;  // initial duration (elapsed time) timing variable starting at 0
bool timing = 0; // boolean state variable to indicate whether stopwatch is actively timing or not 

//create the RF24 object
RF24 radio(9, 8); // CE, CSN

//initialize data packet variables for transmitting
struct DataPacket{
  int X_right; //right joystick x 
  int Y_right; //right joystick y
  int SW_right; //right joystick button
  int X_left; //left joystick x
  int Y_left; //left joystick y
  int SW_left; // left joystick button
};

DataPacket data; //variable to hold data to be transmitted

const byte address[6]="77723"; //address for tx and rx

void setup() {

  radio.begin(); //RF begin radio communication
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);//set transmission address to 5-digit number above
  radio.stopListening(); // assign this antenna as transmitter
  Serial.begin(9600); 

//assign pin functions
  pinMode(X_right_pin, INPUT); 
  pinMode(Y_right_pin, INPUT);
  pinMode(X_left_pin, INPUT);
  pinMode(Y_left_pin, INPUT);

  pinMode(SW_right_pin, INPUT_PULLUP);
  pinMode(SW_left_pin, INPUT_PULLUP);

//first LCD display message
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("POWER ON Go BBL!");
  lcd.clear();
}

void loop() {

  //read inputs 
  X_right_val = analogRead(X_right_pin);
  Y_right_val = analogRead(Y_right_pin);
  X_left_val = analogRead(X_left_pin);
  Y_left_val = analogRead(Y_left_pin);

  SW_right_state = digitalRead(SW_right_pin);
  SW_left_state = digitalRead(SW_left_pin);

  //assign inputs to data packet
  data.X_right=X_right_val;
  data.Y_right=Y_right_val;
  data.SW_right=SW_right_state;
  data.X_left=X_left_val;
  data.Y_left=Y_left_val;
  data.SW_left=SW_left_state;

  //transmit data packet
  radio.write(&data, sizeof(DataPacket));
  delay(20);

//LCD DISPLAY CODE

//TIMER, if right joystick button pushed once, timing begins. if pushed twice, timing stops
  if (digitalRead(SW_right_pin) == LOW && timing== 0) {
    timing=1;
    startTime=(millis());
    duration=((millis())-startTime);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TIMER:");
    lcd.setCursor(0,1);
    lcd.print("0.000");
    delay(200);
  } else if (timing == 1) {
    duration = ((millis())-startTime);
    unsigned long seconds=((millis())-startTime)/1000;
    unsigned long milliseconds = duration-(seconds*1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TIMER:");
    lcd.setCursor(0,1);
    lcd.print(seconds);
    lcd.print(".");
    lcd.print(milliseconds);
  }
  if (digitalRead(SW_right_pin)== LOW && timing== 1){
    timing=0;
    duration = ((millis())-startTime);
    unsigned long seconds=((millis())-startTime)/1000;
    unsigned long milliseconds = duration-(seconds*1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FINAL:");
    lcd.setCursor(0,1);
    lcd.print(seconds);
    lcd.print(".");
    lcd.print(milliseconds);
    delay(3000);
    lcd.clear();
  }else{
  }  

//display commands

  if (SW_left_state== LOW){
    lcd.setCursor(7,0);
    lcd.print("Dance");
    lcd.setCursor(7,1);
    lcd.print("Initiated");
    delay(2000);
  } 
  if (X_right_val>650){
    lcd.setCursor(7,0);
    lcd.print("TurnRight  ");
  }
    else if (X_right_val<400){
    lcd.setCursor(7,0);
    lcd.print("TurnLeft  ");
  }
    else if (Y_right_val>650){
    lcd.setCursor(7,0);
    lcd.print("Forwards  ");
  }
    else if (Y_right_val<400){
    lcd.setCursor(7,0);
    lcd.print("Backwards  ");
  } 
    else {
    lcd.setCursor(7,0);
    lcd.print("              ");
  }
    if (X_left_val<400){
    lcd.setCursor(7,1);
    lcd.print("ClawClose   ");
  }
    else if (X_left_val>650){

    lcd.setCursor(7,1);
    lcd.print("ClawOpen    ");
  }
    else if (Y_left_val<400){

    lcd.setCursor(7,1);
    lcd.print("ArmDown    ");
  }
    else if (Y_left_val>650){

    lcd.setCursor(7,1);
    lcd.print("ArmUp    ");
  } else {
    lcd.setCursor(7,1);
    lcd.print("              ");
  }

//PRINT STATEMENTS TO SERIAL MONITOR FOR TROUBLESHOOTING
  // Serial.println("-----------------");
  // Serial.print("X right: ");
  // Serial.println(X_right_val);
  // Serial.print("Y right: ");
  // Serial.println(Y_right_val);
  // Serial.print("X left: ");
  // Serial.println(X_left_val);
  // Serial.print("Y left: ");
  // Serial.println(Y_left_val);
  // Serial.print("SW right: ");
  // Serial.println(SW_right_state);
  // Serial.print("SW left: ");
  // Serial.println(SW_left_state);
  // delay(500);
  delay(50);

}