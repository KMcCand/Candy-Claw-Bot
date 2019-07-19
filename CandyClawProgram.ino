#include <Servo.h>

#define CLAW_TOGGLE_INPUT 42
#define LEFT_MOTOR1 52 
#define LEFT_MOTOR2 53 
#define RIGHT_MOTOR1 50 
#define RIGHT_MOTOR2 51
#define CLAW_MOTOR1 26
#define CLAW_MOTOR2 27
#define LEFT_MOTOR_SPEED 6
#define RIGHT_MOTOR_SPEED 7
#define JOYSTICK_UP 22
#define JOYSTICK_DOWN 23
#define JOYSTICK_LEFT 24
#define JOYSTICK_RIGHT 25
#define MAX_FORWARD 48
#define MAX_BACKWARD 49
#define MAX_RIGHT 47
#define MAX_LEFT 46
#define CLAW_UP 45
#define CLAW_DOWN 44
#define CLAW_MAX_UP 41
#define CLAW_MAX_DOWN 40
#define DEBOUNCER_CONSTANT_MS 50

// for the countdown timer
#define SEGMENT_CLOCK 9
#define SEGMENT_LATCH 10
#define SEGMENT_DATA 11

bool clawIsOpen = true, alreadyMaxUp = false, alreadyMaxDown = false;
Servo clawServo;

// for the countdown timer
int countdown = 0;
int lastCountdownSeconds = 0;

unsigned long lastPressTime = 0;

void leftMotorForward() {
  digitalWrite (LEFT_MOTOR1, HIGH);
  digitalWrite (LEFT_MOTOR2, LOW);
}

void rightMotorBackward() {
  digitalWrite (RIGHT_MOTOR1, LOW);
  digitalWrite (RIGHT_MOTOR2, HIGH);
}

void leftMotorBackward() {
  digitalWrite (LEFT_MOTOR1, LOW);
  digitalWrite (LEFT_MOTOR2, HIGH);
}

void rightMotorForward() {
  digitalWrite (RIGHT_MOTOR1, HIGH);
  digitalWrite (RIGHT_MOTOR2, LOW);
}

void rightMotorStop() {
  digitalWrite (RIGHT_MOTOR1, LOW);
  digitalWrite (RIGHT_MOTOR2, LOW);
}

void leftMotorStop() {
  digitalWrite (LEFT_MOTOR1, LOW);
  digitalWrite (LEFT_MOTOR2, LOW);
}

void clawMotorForward() {
  digitalWrite (CLAW_MOTOR1, HIGH);
  digitalWrite (CLAW_MOTOR2, LOW);
}

void clawMotorBackward() {
  digitalWrite (CLAW_MOTOR1, LOW);
  digitalWrite (CLAW_MOTOR2, HIGH);
}

void clawMotorStop() {
  digitalWrite (CLAW_MOTOR1, LOW);
  digitalWrite (CLAW_MOTOR2, LOW);
}

void clawOpenClose() {
  
  if (clawIsOpen) {
    clawClose();
  }
  else {
    clawOpen();
  } 
}

void clawOpen() {
  clawServo.write(120);
  clawIsOpen = true;
}

void clawClose() {
  clawServo.write(45);
  clawIsOpen = false;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(JOYSTICK_UP, INPUT_PULLUP);
  pinMode(JOYSTICK_DOWN, INPUT_PULLUP); 
  pinMode(JOYSTICK_LEFT, INPUT_PULLUP);
  pinMode(JOYSTICK_RIGHT, INPUT_PULLUP);
  pinMode(LEFT_MOTOR1, OUTPUT);
  pinMode(LEFT_MOTOR2, OUTPUT);
  pinMode(LEFT_MOTOR_SPEED, OUTPUT);
  pinMode(RIGHT_MOTOR1, OUTPUT);
  pinMode(RIGHT_MOTOR2, OUTPUT);
  pinMode(RIGHT_MOTOR_SPEED, OUTPUT);
  pinMode(MAX_FORWARD, INPUT_PULLUP);
  pinMode(MAX_BACKWARD, INPUT_PULLUP);
  pinMode(MAX_LEFT, INPUT_PULLUP);
  pinMode(MAX_RIGHT, INPUT_PULLUP);
  pinMode(CLAW_UP, INPUT_PULLUP);
  pinMode(CLAW_DOWN, INPUT_PULLUP);
  pinMode(CLAW_MOTOR1, OUTPUT);
  pinMode(CLAW_MOTOR2, OUTPUT);
  pinMode(CLAW_TOGGLE_INPUT, INPUT_PULLUP);
  clawServo.attach(43);
  pinMode(CLAW_MAX_UP, INPUT_PULLUP);
  pinMode(CLAW_MAX_DOWN, INPUT_PULLUP);

  // for the countdown timer
  Serial.begin(9600);
  Serial.println("Large Digit Driver Example");

  pinMode(SEGMENT_CLOCK, OUTPUT);
  pinMode(SEGMENT_DATA, OUTPUT);
  pinMode(SEGMENT_LATCH, OUTPUT);

  digitalWrite(SEGMENT_CLOCK, LOW);
  digitalWrite(SEGMENT_DATA, LOW);
  digitalWrite(SEGMENT_LATCH, LOW);
}


void loop() {
  // put your main code here, to run repeatedly:
  analogWrite (LEFT_MOTOR_SPEED, 255);
  analogWrite (RIGHT_MOTOR_SPEED, 255);

  bool buttonBeingPressed = false;
  clawOpen();
  
  while (true) {
    if (! digitalRead (JOYSTICK_UP) && ! digitalRead (MAX_FORWARD)) {
      if (! digitalRead (JOYSTICK_RIGHT) && ! digitalRead (MAX_RIGHT)) {
        //leftMotorForward(); 
        //rightMotorStop();
        leftMotorStop(); 
        rightMotorForward();
      }
      else if (! digitalRead (JOYSTICK_LEFT) && ! digitalRead (MAX_LEFT)) {
        //leftMotorStop(); 
        //rightMotorForward();
        leftMotorBackward(); 
        rightMotorStop();
      }
      else {
        leftMotorBackward(); 
        rightMotorForward();
      }
    }
    else if (! digitalRead (JOYSTICK_DOWN) && ! digitalRead (MAX_BACKWARD)) {
      if (! digitalRead (JOYSTICK_RIGHT) && ! digitalRead (MAX_RIGHT)) {
        //leftMotorStop(); 
        //rightMotorBackward();
        leftMotorForward(); 
        rightMotorStop();
      }
      else if (! digitalRead (JOYSTICK_LEFT) && ! digitalRead (MAX_LEFT)) {
        //leftMotorBackward(); 
        //rightMotorStop();
        leftMotorStop(); 
        rightMotorBackward();
      }
      else {
        leftMotorForward();
        rightMotorBackward();
      }
    }
    else if (! digitalRead (JOYSTICK_LEFT) && ! digitalRead (MAX_LEFT)) { 
      leftMotorBackward();
      rightMotorBackward();
    }
    else if (! digitalRead (JOYSTICK_RIGHT) && ! digitalRead (MAX_RIGHT)) {
      leftMotorForward();
      rightMotorForward();
    }
    else {
      leftMotorStop();
      rightMotorStop();
    }
    
    if (! digitalRead (CLAW_UP)) {
      if (! digitalRead (CLAW_DOWN)) {
        clawMotorStop();
      }
      else if (! digitalRead (CLAW_MAX_UP)) {
        if (! alreadyMaxUp) {
          clawMotorForward();
          alreadyMaxDown = false;
        }
        
      }
      else {
        clawMotorStop();
        alreadyMaxUp = true;
      }
    }
    else if (! digitalRead (CLAW_DOWN)) {
       if (digitalRead (CLAW_MAX_DOWN)) {
         if (! alreadyMaxDown) {
           clawMotorBackward();
           alreadyMaxUp = false;
         }
       }
       else {
         clawMotorStop();
         alreadyMaxDown = true;
       }
    }
    else {
      clawMotorStop();
    }


    unsigned long latestPressTime = millis();
    if (digitalRead (CLAW_TOGGLE_INPUT) == false) {
      if (buttonBeingPressed == false) {
        buttonBeingPressed = true;
        
        if (latestPressTime - lastPressTime > DEBOUNCER_CONSTANT_MS) {
          clawOpenClose();
          lastPressTime = latestPressTime;
        }
      }
    }
    else {
      if (buttonBeingPressed) {
        if (latestPressTime - lastPressTime > DEBOUNCER_CONSTANT_MS) {
          buttonBeingPressed = false;
          lastPressTime = latestPressTime;
        }
      }
    }
    int seconds = millis()/1000;
    if (! digitalRead(CLAW_UP) && ! digitalRead(CLAW_DOWN) && ! digitalRead(CLAW_TOGGLE_INPUT)) {
      Serial.println("Restart Countdown!");
      countdown = 60;
      showNumber(60);
      lastCountdownSeconds = seconds;
    }
    else if (seconds != lastCountdownSeconds) {
      countdown--;
      if (countdown < -4) {
        countdown = 60;
        showNumber(60);
      }
      else if (countdown == 0) {
        showNumber(0);

         // go all the way up first to ensure that it doesn't accidentally bump into candy chute and drop users candy
         if (! alreadyMaxUp) {
            while (! digitalRead (CLAW_MAX_UP)) {
              clawMotorForward();
            }
         }
         
         clawMotorStop();
         
         while (! digitalRead (MAX_RIGHT) || ! digitalRead (MAX_FORWARD)) {
          rightMotorForward();
            if (! digitalRead (MAX_RIGHT) && ! digitalRead (MAX_FORWARD)) {
              // this is the diagonal case
              leftMotorStop();
            }
            else if (! digitalRead (MAX_RIGHT)) {
              leftMotorForward();
            }
            else if (! digitalRead (MAX_FORWARD)) {
              leftMotorBackward();
            }
         }


         leftMotorStop();
         rightMotorBackward();

         delay(1000);
         
         rightMotorStop();

         delay(1000);
         clawOpen();
         
      }
      else if (countdown < 0) {
        showNumber(0);
      }
      else {
        showNumber(countdown);
      }
      lastCountdownSeconds = seconds;
    }
  }
}

// for the countdown timer
void showNumber(int number)
{
  Serial.print("number: ");
  Serial.println(number);

  for (byte x = 0 ; x < 2 ; x++)
  {
    int remainder = number % 10;

    postNumber(remainder, false);

    number /= 10;
  }

  //Latch the current segment data
  digitalWrite(SEGMENT_LATCH, LOW);
  digitalWrite(SEGMENT_LATCH, HIGH); //Register moves storage register on the rising edge of RCK
}

// for the countdown timer
void postNumber(byte number, boolean decimal)
{
  //    -  A
  //   / / F/B
  //    -  G
  //   / / E/C
  //    -. D/DP

#define a  1<<0
#define b  1<<6
#define c  1<<5
#define d  1<<4
#define e  1<<3
#define f  1<<1
#define g  1<<2
#define dp 1<<7

  byte segments;

  switch (number)
  {
    case 1: segments = b | c; break;
    case 2: segments = a | b | d | e | g; break;
    case 3: segments = a | b | c | d | g; break;
    case 4: segments = f | g | b | c; break;
    case 5: segments = a | f | g | c | d; break;
    case 6: segments = a | f | g | e | c | d; break;
    case 7: segments = a | b | c; break;
    case 8: segments = a | b | c | d | e | f | g; break;
    case 9: segments = a | b | c | d | f | g; break;
    case 0: segments = a | b | c | d | e | f; break;
    case ' ': segments = 0; break;
    case 'c': segments = g | e | d; break;
    case '-': segments = g; break;
  }

  if (decimal) segments |= dp;

  //Clock these bits out to the drivers
  for (byte x = 0 ; x < 8 ; x++)
  {
    digitalWrite(SEGMENT_CLOCK, LOW);
    digitalWrite(SEGMENT_DATA, segments & 1 << (7 - x));
    digitalWrite(SEGMENT_CLOCK, HIGH); //Data transfers to the register on the rising edge of SRCK
  }
}
