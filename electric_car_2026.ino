/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Autonomous Electric Vehicle 4/23/26
This is a simple electric vehicle design that uses Arduino Nano, 9V batteries for board power as 
well as 4AA batteries to power TT motors. The car avoids obstacles by using a ultrasonic sensor.
Use screw terminals to secure most wires to the board, as well as low profile jumpers for best organization.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pinout:

Arduino Nano:
GND -> Shared GND
D2 -> Button Left Pin
D3 -> Ultrasonic Trig      
D4 -> Ultrasonic Echo
D5 -> 
D6 ->  
D7 ->  Motor Controller IN2
D8 -> Motor Controller IN1   
D9 -> Motor Controller IN4
D10 -> Motor Controller (ENA)
D11 ->  Motor Controller FNB (ENB)
D12 -> Motor Controller IN3
5V -> Ultrasonic VCC
Vin -> 9V Battery Screw Terminal V

9V Battery:
GND -> Shared GND

AA Battery:
V (Via Screw Terminal) -> Motor Controller +12V
GND -> Shared GND

Motor Controller:
GND -> Shared GND

Ultrasonic Sensor:
GND -> Shared GND

Button:
Signal Pin bottom left -> D2
Bottom Right Pin -> Shared GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include <Servo.h>

int button_pin = 2;
int last_button = HIGH;
int current_button = HIGH;
int state = 0;

int left_motor_pin_1 =  8;
int left_motor_pin_2 = 7;

int right_motor_pin_1 = 9;
int right_motor_pin_2 = 12;

int ena = 11;
int fnb = 10;

int trig_pin = 3;
int echo_pin = 4;
float duration, distance;

Servo sweep_servo;
int sweep_servo_pin = 6;

int servo_pos = 20;
int servo_direction = 1;
unsigned long last_servo_move = 0;
int servo_move_interval = 15;

void setup_pins(){
  sweep_servo.attach(sweep_servo_pin);
  pinMode(left_motor_pin_1, OUTPUT);
  pinMode(left_motor_pin_2, OUTPUT);
  pinMode(right_motor_pin_1, OUTPUT);
  pinMode(right_motor_pin_2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(fnb, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void setup(){
  Serial.begin(9600);
  setup_pins();
}

void on_button() {
  current_button = digitalRead(button_pin);
  if (last_button == HIGH && current_button == LOW) {
    state = !state;
    Serial.print("State: ");
    Serial.println(state);
    delay(200);  //prevents multiple toggles per press
  }
  last_button = current_button;
}

float read_distance(){
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);
  distance = duration / 148.0; //converts to inches dividing duration by speed of sound (.0125 in/ micro second)
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
  return distance;
}


void run_motors(){
  analogWrite(ena, 120);
  analogWrite(fnb, 120);
  digitalWrite(left_motor_pin_1, LOW);
  digitalWrite(left_motor_pin_2, HIGH);
  digitalWrite(right_motor_pin_1, HIGH);
  digitalWrite(right_motor_pin_2, LOW);
}

void backup_motors(int duration){
  analogWrite(ena, 120);
  analogWrite(fnb, 120);
  digitalWrite(left_motor_pin_1, HIGH);
  digitalWrite(left_motor_pin_2, LOW);
  digitalWrite(right_motor_pin_1, LOW);
  digitalWrite(right_motor_pin_2, HIGH);
  delay(duration);
  analogWrite(ena, 0);
  analogWrite(fnb, 0);
  digitalWrite(left_motor_pin_1, LOW);
  digitalWrite(left_motor_pin_2, LOW);
  digitalWrite(right_motor_pin_1, LOW);
  digitalWrite(right_motor_pin_2, LOW);
}

void stop_motors(){
  analogWrite(ena, 0);
  analogWrite(fnb, 0);
  digitalWrite(left_motor_pin_1, LOW);
  digitalWrite(left_motor_pin_2, LOW);
  digitalWrite(right_motor_pin_1, LOW);
  digitalWrite(right_motor_pin_2, LOW);
}

void turn_motors(int time){
  analogWrite(ena, 120);
  analogWrite(fnb, 120);
  digitalWrite(left_motor_pin_1, LOW);
  digitalWrite(left_motor_pin_2, HIGH);
  digitalWrite(right_motor_pin_1, LOW);
  digitalWrite(right_motor_pin_2, HIGH);
  delay(time);
 
}
//NEEDS TO BE CORRECTED TO USE WITH A BLUE MICRO SERVO THAT 180 DEGREES.
void sweep_servo_motor(){
  
  if (millis() - last_servo_move >= servo_move_interval) {
    last_servo_move = millis();

    servo_pos += servo_direction;

    if (servo_pos >= 140 || servo_pos <= 20) {
      servo_direction *= -1;
    }

    sweep_servo.write(servo_pos);
  }
}

void loop(){
//   sweep_servo_motor(); //this cannot stay here ,results in looping
//   float car_distance = read_distance();
//   on_button();
//   if (state == 0) {
//     stop_motors();
//   } else if (car_distance > 10.0) {
//       run_motors();
//   } else {
//       stop_motors();
//       backup_motors(1000);
//       turn_motors(300);
// }
  run_motors();
}
