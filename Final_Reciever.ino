//same libraries and object creation as your transmitter
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h> //initializing all the libraries needed in order to recieve and control the components on the CoLeBot

Servo myservo1; //added servo control to mysero1 variable and myservovariable2
Servo myservo2;

RF24 radio(9, 8);  // CE, CSN
const int ena1 = 5; //initializing the components needed to control the first dc motor
const int drive1a = 6;
const int drive2a = 7;

const int ena2 = 3;  //initializing the components needed to control the second dc motor
const int drive3a = 4;
const int drive4a = 2;

int ServoPin1 = A3; //assigning the correct pin with the servo 
int ServoPin2 = A2;

int ArmAngle = 90; //initializinig the initial arm angle
int ClawAngle = 90; //initializing the initial claw angle
int intensity = 0; 
int intensity2 = 0;
int increment = 1; //creating an increment to change the angle of the armAngle and clawAngle as we move the claw and arm
int Max = 140; 
int Min =60;

int spin_speed = 0;

// This is the data packet with the data we need to transmit from the controller to the reciever in order to control the robot
struct DataPacket {
  int X_right;
  int Y_right;
  int SW_right;
  int X_left;
  int Y_left;
  int SW_left;
};


DataPacket data; //this brings in the data from the datapacket
const byte address[6] = "77723"; //the radio code that we use to communicate between reciever and controller


void setup() {
  radio.begin(); //This is the radio code to start listening for the controller and recieve the data to control roboto components 
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0,address);
  radio.startListening();
  Serial.begin(9600);

  myservo1.attach(A2); //attacahes the servo to it's pin
  myservo2.attach(A3); 

  myservo1.write(90);
  delay(20);
  myservo2.write(90);
  delay(20);

  pinMode(ena1,OUTPUT); //allows the arduino to know to make the pin related to these as ouputs for motor 1
  pinMode(drive1a,OUTPUT);
  pinMode(drive2a,OUTPUT);

  pinMode(ena2,OUTPUT); //allows the arduino to know to make the pin related to these as ouputs for motor 2 
  pinMode(drive3a,OUTPUT); 
  pinMode(drive4a,OUTPUT);

  
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(DataPacket));

    if (ClawAngle < 0 || ClawAngle > 180 || ArmAngle < 0 || ArmAngle > 180) { //this allows for the ClawAngle to not go past 0 or 180 degrees
      if (ClawAngle < 0) {
        ClawAngle = 0;
        myservo1.write(ClawAngle);
      } else if (ClawAngle > 180) {
        ClawAngle = 180;
        myservo1.write(ClawAngle);
      } else if (ArmAngle < 0) {
        ArmAngle = 0;
        myservo2.write(ArmAngle);
      } else if (ArmAngle > 180) {
        ArmAngle = 180;
        myservo2.write(ArmAngle);
      }
    } else if(data.X_left > 650 && data.Y_left < 650 && data.Y_left > 400){
      //increment = map(data.X_left,600,1023,1,15);
      ClawAngle = constrain(ClawAngle,60,122); 
      myservo1.write(ClawAngle + increment); //increments the ArmAngle so we know when we hit 130 degrees
      ClawAngle = ClawAngle + increment; 
      Serial.println(ClawAngle);
      delay(20);
    }
    else if(data.X_left < 400 && data.Y_left < 650 && data.Y_left > 400){
      //increment = map(data.X_left,650,1023,1,15);
      ClawAngle = constrain(ClawAngle,60,122); //contrain function to make sure that the Arm Angle doesn't overheat the servo motor
      myservo1.write(ClawAngle - increment); //increments the ArmAngle so we know when we hit 130 degrees
      ClawAngle = ClawAngle - increment;
      Serial.println(ClawAngle);
      delay(20);
    }
     else if(data.Y_left > 650 && data.X_left < 650 && data.X_left > 400){
      //increment = map(data.Y_left,650,1023,1,15);
      ArmAngle = constrain(ArmAngle,60,140); //contrain function to make sure that the claw angle doesn't overheat when clasping the I-beam
      myservo2.write(ArmAngle + increment); //increments the ArmAngle so we know when we hit 40 or 150 degrees
      ArmAngle = ArmAngle + increment;
      Serial.println(ArmAngle);
      delay(20);
    }
    else if(data.Y_left < 400 && data.X_left < 650 && data.X_left > 400){
      //increment = map(data.Y_left,650,1023,1,15);
      ArmAngle = constrain(ArmAngle,60,140); //contrain function to make sure that the claw angle doesn't overheat when clasping the I-beam
      myservo2.write(ArmAngle - increment); //increments the ArmAngle so we know when we hit 40 or 150 degrees
      ArmAngle = ArmAngle - increment; 
      Serial.print(ArmAngle);
      delay(20);
    }else {
      //Serial.print("Inactive");
    }
    /*
    Serial.print (", ");
    Serial.print (data.X_left);
    Serial.print (", ");
    Serial.print (data.Y_left);
    Serial.print (", ");
    Serial.print (ClawAngle);
    Serial.print (", ");
    Serial.println (ArmAngle);
    */

    if(data.Y_right > 650 && data.X_right < 650 && data.X_right > 400){ //Takes the data from the data packet and uses it to drive the motors right
      intensity = map(data.Y_right,650,1023,127,255);
      analogWrite(ena1,255);
      digitalWrite(drive1a,LOW);
      digitalWrite(drive2a,HIGH);
      intensity2 = map(data.Y_right,400,0,127,255);
      analogWrite(ena2,255);
      digitalWrite(drive3a,HIGH);
      digitalWrite(drive4a,LOW);
      //Serial.print("Turn Right");
    }
    else if(data.Y_right < 400 && data.X_right < 650 && data.X_right > 400){ //Takes the data from the data packet and uses it to drive the motors left
      intensity = map(data.Y_right,400,0,50,255);
      analogWrite(ena1,intensity);
      digitalWrite(drive1a,HIGH);
      digitalWrite(drive2a,LOW);
      intensity2 = map(data.Y_right,650,1023,50,255);
      analogWrite(ena2,intensity);
      digitalWrite(drive3a,LOW);
      digitalWrite(drive4a,HIGH);
      //Serial.print("Turn Left");
    }
     else if(data.X_right > 650 && data.Y_right < 650 && data.Y_right > 400){ //Takes the data from the data packet and uses it to drive the motors forwards
      intensity2 = map(data.X_right,650,1023,127,255);
      analogWrite(ena1,intensity);
      digitalWrite(drive1a,LOW);
      digitalWrite(drive2a,HIGH);
      intensity2 = map(data.X_right,650,1023,127,255);
      analogWrite(ena2,intensity);
      digitalWrite(drive3a,LOW);
      digitalWrite(drive4a,HIGH);
      //Serial.print("Forwards");
    }
    else if(data.X_right < 400 && data.Y_right < 650 && data.Y_right > 400){ //Takes the data from the data packet and uses it to drive the motors backwards
      intensity = map(data.X_right,400,0,127,255);
      analogWrite(ena1,intensity);
      digitalWrite(drive1a,HIGH);
      digitalWrite(drive2a,LOW);
      intensity = map(data.X_right,400,0,127,255);
      analogWrite(ena2,intensity);
      digitalWrite(drive3a,HIGH);
      digitalWrite(drive4a,LOW);
      //Serial.print("Backwards");
    }else {
      analogWrite(ena1,0); //ensures that when no input from the reciever is being done nothing happens to the robot
      analogWrite(ena2,0);
      digitalWrite(drive1a,LOW);
      digitalWrite(drive2a,LOW);
      digitalWrite(drive3a,LOW);
      digitalWrite(drive4a,LOW);
      //Serial.print("Inactive");
    }
    delay(50);
    // Serial.print (", ");
    //Serial.print (data.X_right);
    //Serial.print (", ");
    //Serial.println (data.Y_right);
    }
  //radio.write(&data, sizeof(DataPacket));
  //radio.write(&data, sizeof(DataPacket));
}