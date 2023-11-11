//Pumpkin Bot Controller V7
//Originally Devoloped from: https://www.thingiverse.com/thing:4905975/files
//Now with RC controller


//  --------                 --------
// |  D9    |               |  D7    |
// | joint9 |               | joint7 |
//  ----- --------     --------- -----
//       |  D8    |   |  D6    |
//       | joint8 |   | joint6 |
//        --------     --------
//       |  D2    |  |   D4    |
//       | joint2 |  |  joint4 |
//  ----- --------    --------- -----
// |  D3    |               |  D5    |
// | joint3 |               | joint5 |
//  --------                 --------
//                Front


//Define RC inputs
#define CH1 10
#define CH2 11
#define CH3 12

//setup serov objects and position vairables
#include <Servo.h>
Servo joint2;  
Servo joint3;  
Servo joint4;  
Servo joint5;  
Servo joint6;  
Servo joint7;  
Servo joint8;
Servo joint9;

//Shoulder
int home_joint2 = 120; //defualt 135
//Leg
int home_joint3 = 40; //default: 0

int home_joint4 = 70; //defualt: 45
int home_joint5 = 160; //defualt 180

int home_joint6 = 120; //defualt: 135
int home_joint7 = 40; //default: 0

int home_joint8 = 70; //defualt:45
int home_joint9 = 160; //defualt: 180

int zero = 90; //zero point for servos

String command; //for serial line control

int off = 0; //for startup command

// Read the number of a given RC channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the channel and return a boolean value
bool redSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

//other varibles for rc controller
int ch1Value, ch2Value, ch3Value;
int threashold = 40;


void setup() {

  //Assign RC inputs
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);

  //Assign pinout to servos
  joint2.attach(2);
  joint3.attach(3);
  joint4.attach(4);
  joint5.attach(5);
  joint6.attach(6);
  joint7.attach(7);
  joint8.attach(8);
  joint9.attach(9);

  //Start all zervos in home position
  joint2.write(home_joint2);
  joint3.write(home_joint3);
  joint4.write(home_joint4);
  joint5.write(home_joint5);
  joint6.write(home_joint6);
  joint7.write(home_joint7);
  joint8.write(home_joint8);
  joint9.write(home_joint9);
  
Serial.begin(9600); // Starts the serial communication

Serial.println("Commands:");
Serial.println("callibrate, sit, standhome, stand1-2-3, foward, back, left, right, twist");

//wait 2 seconnds
delay(2000);
}


void loop() {
  //Read RC input
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, 0);
  //for diognostics
  //Serial.println("Ch1: ");
  //Serial.println(ch1Value);
  //Serial.println(" Ch2: ");
  //Serial.println(ch2Value);

  if (ch2Value > threashold){
    forward(1);
    Serial.println("foward");
  }

  else if (ch2Value < -threashold){
    backward(1);
    Serial.println("backward");
  }

  else if (ch1Value > threashold){
    rightturn(1);
    Serial.println("turning right");
  }

  else if (ch1Value < -threashold){
    leftturn(1);
    Serial.println("turning left");
  }
  
  //throttle low = power down
  else if (ch3Value > -90){
    sithome();
    delay(2000);
    powerdown();
    delay(2000);
    Serial.println("Powered Down");
    off = 1;
  }

  //throttle high = power up
  else if (ch3Value < 90 & off == 1){
    powerup();
    delay(2000);
    standhome();
    delay(2000);
    Serial.println("Powered UP");
    off = 0;
  }
  

  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equals("callibrate")) {
      Serial.println("servos set to 90 degrees");
      callibrate();
      delay(2000);
    }

    else if (command.equals("sit")) {
      Serial.println("sitting");
      sithome();
      delay(2000);
    }

    else if (command.equals("standhome")) {
      Serial.println("servos at home position");
      standhome();
      delay(2000);
    }

    else if (command.equals("stand1")) {
      Serial.println("Stand up");
      stand1();
      delay(2000);
    }

    else if (command.equals("stand2")) {
      Serial.println("Stand up");
      stand2();
      delay(2000);
    }    

     else if (command.equals("stand3")) {
      Serial.println("Stand up");
      stand3();
      delay(2000);
    }   

    else if (command.equals("foward")) {
      //forward(1);
      forward(5);
      delay(2000);
    }

    else if (command.equals("back")) {
      backward(5);
      delay(2000);
    }    

    else if (command.equals("right")) {
      rightturn(5);
      delay(2000);
    }    

    else if (command.equals("left")) {
      leftturn(5);
      delay(2000);
    }  
    
    else if (command.equals("twist")) {
      twist();
      delay(2000);
    }

    else if(command.equals("powerdown")){
      sithome();
      delay(2000);
      powerdown();
      delay(2000);
    } 

    else if (command.equals("powerup")){
      powerup();
      delay(2000);
      standhome();
      delay(2000);
    }

  }


}

void idle(){
  //for debuging:
  //delay(1000);
  //should make this varible controlled by throttle???
  delay(100);
  }

void callibrate(){
  joint2.write(zero);
  joint3.write(zero);
  joint4.write(zero);
  joint5.write(zero);
  joint6.write(zero);
  joint7.write(zero);
  joint8.write(zero);
  joint9.write(zero);
}

void standhome(){
  joint2.write(home_joint2);
  joint3.write(home_joint3);
  joint4.write(home_joint4);
  joint5.write(home_joint5);
  joint6.write(home_joint6);
  joint7.write(home_joint7);
  joint8.write(home_joint8);
  joint9.write(home_joint9);
}



void sithome(){
  joint2.write(home_joint2);
  joint3.write(home_joint3+90);
  joint4.write(home_joint4);
  joint5.write(home_joint5-90);
  joint6.write(home_joint6);
  joint7.write(home_joint7+90);
  joint8.write(home_joint8);
  joint9.write(home_joint9-90);
}



void stand1(){
  sithome();
  joint2.write(170);
  delay(300);
  joint2.write(home_joint2);
  delay(300);
  joint4.write(10);
  delay(300);
  joint4.write(home_joint4);
  delay(300);
  joint6.write(170);
  delay(300);
  joint6.write(home_joint6);
  delay(300);
  joint8.write(10);
  delay(300);
  joint8.write(home_joint8);
  delay(300);
  
  joint3.write(home_joint3);
  joint5.write(home_joint5);
  joint7.write(home_joint7);
  joint9.write(home_joint9);
}


void stand2(){
  sithome();
  joint2.write(175);
  joint4.write(5);
  joint6.write(175);
  joint8.write(5);
  delay(600);
    
  joint2.write(home_joint2);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  joint8.write(home_joint8);
  delay(600);
  
  joint3.write(home_joint3);
  joint5.write(home_joint5);
  joint7.write(home_joint7);
  joint9.write(home_joint9);
}


void stand3()
{ 
  sithome();
  int i;
  int j = 90;
  int k = 90;
  joint2.write(home_joint2);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  joint8.write(home_joint8);
  for(i = 90; i < 165; i++)
  {
    joint5.write(i);
    j = j - 1;
    joint3.write(j);
    delay(20);
  }
  
  for(i = 115; i < 165; i++)
  {
    joint9.write(i);
    k = k - 1;
    joint7.write(k);
    delay(20);
  }
}


void downaction(unsigned int step){
  while (step-- > 0){
  sithome();
  delay(100);
  standhome();
  delay(100);
  }
}


void wink(unsigned int step){
  standhome();
  joint9.write(home_joint9-30);
  
  while (step-- > 0){
  joint5.write(30);
  joint4.write(home_joint4 + 60);
  delay(300);
  joint4.write(home_joint4 - 60);
  delay(300);
  }
}


void twist(){
  joint3.write(home_joint3);
  joint5.write(home_joint5);
  joint7.write(home_joint7);
  joint9.write(home_joint9);
  
  for(int right=90;right<170;right++){
      joint2.write(right);
      joint6.write(right);
      joint4.write(right-90);
      joint8.write(right-90);
      delay(10);
  }

   for(int right=170;right>90;right--){
      joint2.write(right);
      joint6.write(right);
      joint4.write(right-90);
      joint8.write(right-90);
      delay(10);
  }

  //NEW TWIST WALK
  /*
  //Right Front and Back Left Legs LIFT
  joint3.write(home_joint3+90);
  joint7.write(home_joint7+90);
  idle();
  //Right Front and Back Left shoulders REACH
  joint4.write(zero);
  joint8.write(zero);
  //While Left Front and Back Right Shoulders ROW
  joint2.write(home_joint2);
  joint6.write(home_joint6);
  idle();

  //Right Front and Back Left Legs back DOWN
  joint3.write(home_joint3);
  joint7.write(home_joint7);
  idle();

  //Left Front and Back Right Leg LIFT
  joint5.write(home_joint5-90);
  joint9.write(home_joint9-90);
  idle();

  //Left Front and Back Right Shoulders REACH
  joint2.write(zero);
  joint6.write(zero);
  //While Right Front and Back Left Shoulder ROW
  joint4.write(home_joint4);
  joint8.write(home_joint8);
  idle();

  //Left Front and Back Right Leg back DOWM
  joint5.write(home_joint5);
  joint9.write(home_joint9);
  idle();

  */

}


void forward(unsigned int step){
  while (step-- > 0){
  //Right Front and Back Left Legs lift 
  joint3.write(home_joint3+90);
  joint7.write(home_joint7+90);
  idle();
  //Right Shoulders (front and back) seperate (reach/row)
  joint2.write(home_joint2+30);
  joint8.write(home_joint8-30);
  //Left Shoulders (front and back) go to home
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  idle();
  //Right Front and Back Left Legs back to home
  joint3.write(home_joint3);
  joint7.write(home_joint7);
  idle();

  //Left Front and Back Right Leg Lift
  joint5.write(home_joint5-90);
  joint9.write(home_joint9-90);
  idle();
  //Right shoulders (front and back) go to home
  joint2.write(home_joint2);
  joint8.write(home_joint8);
  //Left shoulders (front and back) seperate?
  joint4.write(home_joint4-30);
  joint6.write(home_joint6+30);
  idle();
  //Left Front and Back Right Leg back to home
  joint5.write(home_joint5);
  joint9.write(home_joint9);
  idle();


  }
}


void backward(unsigned int step){
  while (step-- > 0){
  joint3.write(home_joint3+90);
  joint7.write(home_joint7+90);
  delay(100);
  joint2.write(home_joint2);
  joint8.write(home_joint8);
  joint4.write(home_joint4-30);
  joint6.write(home_joint6+30);
  delay(100);
  joint3.write(home_joint3);
  joint7.write(home_joint7);
  idle();
  
  joint5.write(home_joint5-90);
  joint9.write(home_joint9-90);
  delay(100);
  joint2.write(home_joint2+30);
  joint8.write(home_joint8-30);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  delay(100);
  joint5.write(home_joint5);
  joint9.write(home_joint9);
  idle();
  }
}


void rightturn(unsigned int step){
  while (step-- > 0){
  joint5.write(home_joint5-90);
  joint9.write(home_joint9-90);
  delay(100);
  joint2.write(home_joint2+30);
  joint8.write(home_joint8-30);
  joint4.write(home_joint4-30);
  joint6.write(home_joint6+30);
  delay(100);
  joint5.write(home_joint5);
  joint9.write(home_joint9);
  idle();
  
  joint3.write(home_joint3+90);
  joint7.write(home_joint7+90);
  delay(100);
  joint2.write(home_joint2);
  joint8.write(home_joint8);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  delay(100);
  joint3.write(home_joint3);
  joint7.write(home_joint7);
  idle();
  }
}


void leftturn(unsigned int step){
  while (step-- > 0){
  joint3.write(home_joint3+90);
  joint7.write(home_joint7+90);
  delay(100);
  joint2.write(home_joint2+30);
  joint8.write(home_joint8-30);
  joint4.write(home_joint4-30);
  joint6.write(home_joint6+30);
  delay(100);
  joint3.write(home_joint3);
  joint7.write(home_joint7);
  idle();
  
  joint5.write(home_joint5-90);
  joint9.write(home_joint9-90);
  delay(100);
  joint2.write(home_joint2);
  joint8.write(home_joint8);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  delay(100);
  joint5.write(home_joint5);
  joint9.write(home_joint9);
  idle();
  }
}

void powerdown(){
    //Assign pinout to servos
  joint2.detach();
  joint3.detach();
  joint4.detach();
  joint5.detach();
  joint6.detach();
  joint7.detach();
  joint8.detach();
  joint9.detach();
}

void powerup(){
    //Assign pinout to servos
  joint2.attach(2);
  joint3.attach(3);
  joint4.attach(4);
  joint5.attach(5);
  joint6.attach(6);
  joint7.attach(7);
  joint8.attach(8);
  joint9.attach(9);
}

