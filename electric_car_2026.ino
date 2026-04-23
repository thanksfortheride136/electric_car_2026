/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Autonomous Electric Vehicle 4/23/26
This is a simple electric vehicle design that uses Arduino Nano, 9V batteries for board power as 
well as 4AA batteries to power TT motors. The car avoids obstacles by using a ultrasonic sensor.
Use screw terminals to secure most wires to the board, as well as low profile jumpers for best organization.

Pinout:

Arduino Nano:
GND -> Shared GND
D2 -> GND
D3 -> Motor Controller IN2
D4 -> Motor Controller IN3
D5 -> Motor Controller IN4
D6 -> Ultrasonic Echo
D7 -> Ultrasonic Trig
D8 -> Motor Controller IN1
D10 -> Motor Controller FNB (ENB)
D11 -> Motor Controller (ENA)
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
int button_pin = 2;
int last_button = HIGH;
int current_button = HIGH;
int state = 0;

int left_motor_pin_1 =  8;
int left_motor_pin_2 = 3;
int right_motor_pin_1 = 4;
int right_motor_pin_2 = 5;
int ena = 11;
int fnb = 10;

void setup_pins(){
  pinMode(left_motor_pin_1, OUTPUT);
  pinMode(left_motor_pin_2, OUTPUT);
  pinMode(right_motor_pin_1, OUTPUT);
  pinMode(right_motor_pin_2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(fnb, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
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

void run_motors(){
  analogWrite(ena, 120);
  analogWrite(fnb, 120);
  digitalWrite(left_motor_pin_1, LOW);
  digitalWrite(left_motor_pin_2, HIGH);
  digitalWrite(right_motor_pin_1, HIGH);
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
void loop(){
  on_button();
  //logic will say IF no button is 1 call run motor function else call stop motor function
  if(state == 1){
    run_motors();
  }else{
    stop_motors();
  }
}
