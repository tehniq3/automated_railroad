/////////////////////////////////////////////////////
//
// Simple back and forth train control by Arduino with IR sensor
//
// ryuchihoon@gmail.com
// http://ardutrain.blogspot.com/2013/06/back-and-forth-train-control-by-arduino.html
// video: https://youtu.be/fNeRCVgfMRw
//
// ver.1.0 by niq_ro (Nicu Florica) - changed for usual IR module (HIGH - free, LOW - obstacle) and control a L298 module
// ver.1.b - PWM start from cca. 40% (105/255) not from 0
//
// train speed setups
//
// NOTE: Some kato trams are believed that their coreless motors are
//       fragile to low speed PWM'ed input.
//       I use normal(full) speed value as 255.
//
const int SPEED_MAX = 255;  // maximum PWM
const int SPEED_MIN = 105;  // minimum PWM for start 
const int SPEED_DELTA = 3;
//const int SPEED_MAX = 90;
//const int SPEED_DELTA = 8;


//
// pin conntection setups
//
const int IRD_A = 7; // detector a
const int IRD_B = 6; // detector b
const int LED_RED = 13;
const int SPEED_PWM = 3;
const int DIR = 12;
const int DIR2 = 11;

const int STAY_TIME = 4 * 1000;  // pause for change direction

const int CONTROL_INIT = 0;
const int CONTROL_DECEL = 1;
const int CONTROL_STOPPED = 2;
const int CONTROL_RUNNING = 3;
boolean ignore_detector_a = false;
boolean ignore_detector_b = false;
unsigned long time_to_leave = 0;
int train_control = CONTROL_INIT;
int train_speed = 0;
int train_dir = HIGH;
int train_dir2 = LOW;  // negate train_dir

void setup() {
  pinMode(IRD_A, INPUT);
  pinMode(IRD_B, INPUT);
  digitalWrite(IRD_A, HIGH);
  digitalWrite(IRD_B, HIGH);
  pinMode(LED_RED, OUTPUT);
  pinMode(SPEED_PWM, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(DIR2, OUTPUT);
}

void loop() {
    int ir_detected = HIGH;  // free
  if(!ignore_detector_a) 
  {
    ir_detected = digitalRead(IRD_A);
     if(ir_detected == LOW) {
      ignore_detector_a = true;
      ignore_detector_b = false;
    }
  } else if(!ignore_detector_b) 
  {
    ir_detected = digitalRead(IRD_B);
    if(ir_detected == LOW) {
      ignore_detector_b = true;
      ignore_detector_a = false;
    }
  }

  if(ir_detected == LOW) 
  {
    train_control = CONTROL_DECEL;
    digitalWrite(LED_RED, HIGH);
  }

  if(train_control != CONTROL_DECEL) 
  {
    digitalWrite(LED_RED, LOW);
  }

  if(train_control == CONTROL_INIT) 
  {
    train_speed = SPEED_MIN;
    train_control = CONTROL_RUNNING;
    digitalWrite(DIR, train_dir);
    train_dir2 = (train_dir+1)%2;
    digitalWrite(DIR2, train_dir2);
  }
 
  speed_control();

  delay(10);
} // end main loop


void speed_control() {
  switch(train_control) {
  case CONTROL_DECEL:
    train_speed -= SPEED_DELTA;
    if(train_speed <= 0) {
      train_speed = 0;
      train_control = CONTROL_STOPPED;
      time_to_leave = millis() + STAY_TIME;
    }
    break;
  case CONTROL_STOPPED:
      digitalWrite(DIR, 0);
      digitalWrite(DIR2, 0);
    if(millis() > time_to_leave) {
      train_dir = HIGH - train_dir;
      train_dir2 = (train_dir+1)%2;
      digitalWrite(DIR, train_dir);
      digitalWrite(DIR2, train_dir2);
      train_control = CONTROL_RUNNING;
    }
    break;
  case CONTROL_RUNNING:
    train_speed += SPEED_DELTA;
    if(train_speed >= SPEED_MAX) {
      train_speed = SPEED_MAX;
    }
    break;
  }
  if(train_speed == 0) {
    digitalWrite(SPEED_PWM, LOW);
  } else if(train_speed == 255) {
    digitalWrite(SPEED_PWM, HIGH);
  } else {
    analogWrite(SPEED_PWM, train_speed);
  }
}

// EOF
