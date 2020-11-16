#include <Servo.h>

/////////////////////////////
// Configurable parameters //
/////////////////////////////

// Arduino pin assignment
#define PIN_LED 9      // [1234] LED를 GPIO 9번 핀에 연결

#define PIN_SERVO 10  //[3030]SERVO를 10번 핀에 연결
#define PIN_IR A0     //[3037]적외선 센서를 A0에 연결

// Framework setting
#define _DIST_TARGET 255            // [3040] 레일플레이트의 중간지점(목표지점=25.5cm)
#define _DIST_MIN 100                    //[3035] 측정 최소 거리
#define _DIST_MAX 410   //[3036] 측정 가능한 최대 거리

// Distance sensor
#define _DIST_ALPHA 0.0   //[3023] EMA 가중치

// Servo range 
#define _DUTY_MIN 1000                 //[3028] 서보 각도 최소값
#define _DUTY_NEU 1450    //[3038] 레일 수평 서보 펄스폭
#define _DUTY_MAX 2000    //[3031] 서보 최대값

// Servo speed control
#define _SERVO_ANGLE 30   //[3030] servo angle limit 실제 서보의 동작크기
#define _SERVO_SPEED 30            // [3040] 서보의 각속도(초당 각도 변화량)

// Event periods
#define _INTERVAL_DIST 20   //[3039]적외선 센서 측정 간격
#define _INTERVAL_SERVO 20         //[3046]서보갱신간격
#define _INTERVAL_SERIAL 100       //[3030]시리얼 플로터 갱신간격

// PID parameters
#define _KP 0.0     //[3039] 비례 제어의 상수 값

//////////////////////
// global variables //
//////////////////////

// Servo instance     //[3046]서보간격
Servo myservo;                      //[3030]servo

// Distance sensor
float dist_target; // location to send the ball
float dist_raw, dist_ema;     //[3034]적외선센서로 측정한 거리값과 ema필터를 적용한 거리값

// Event periods
unsigned long last_sampling_time_dist, last_sampling_time_servo, last_sampling_time_serial;   //[3039] interval간격으로 동작 시행을 위한 정수 값
bool event_dist, event_servo, event_serial; 

// Servo speed control
int duty_chg_per_interval;  //[3039] interval 당 servo의 돌아가는 정도
int duty_target, duty_curr;      //[3030]servo 목표 위치, servo 현재 위치

// PID variables
float error_curr, error_prev, control, pterm, dterm, iterm; 
// [3042] 현재 오차값, 이전 오차값, ? , p,i,d 값


void setup() {
// initialize GPIO pins for LED and attach servo 
pinMode(PIN_LED,OUTPUT);           //[3030]LED를 연결
myservo.attach(PIN_SERVO);  //[3039]servo를 연결

// initialize global variables
dist_min = _DIST_MIN;                      //[3030] 측정값의 최소값
dist_

// move servo to neutral position
myservo.writeMicroseconds(_DUTY_NEU);//[3030]서보를 레일이 수평이 되는 값으로 초기화

// initialize serial port
Serial.begin(57600);  //[3039]

// convert angle speed into duty change per interval.
  duty_chg_per_interval =(float)(_DUTY_MAX - _DUTY_MIN) * (_SERVO_SPEED / _SERVO_ANGLE) * ( INTERVAL_SERVO / 1000.0); //[3039] 
  
/*
_SERVO_ANGLE/_SERVO_SPEED=
(_DUTY_MAX - _DUTY_MIN_)* INTERVAL_SERVO/duty_chg_per_interval = 
interval 반복횟수*INTERVAL_SERVO=
_SERVO_ANGLE만큼 돌아가는데 걸리는 시간 
*/
 
 
}
  

void loop() {
/////////////////////
// Event generator //
/////////////////////


////////////////////
// Event handlers //
////////////////////

  if(event_dist) {
     event_dist = false;    // [3037]
  // get a distance reading from the distance sensor
     dist_ema = ir_distance_filtered();   // [3037]

  // PID control logic
    error_curr = 
    pterm = 
    control = 

  // duty_target = f(duty_neutral, control)
    duty_target = 

  // keep duty_target value within the range of [_DUTY_MIN, _DUTY_MAX]

  }
  
  if(event_servo) {

    // adjust duty_curr toward duty_target by duty_chg_per_interval 
     if(duty_target > duty_curr) {
        duty_curr += duty_chg_per_interval;
        if(duty_curr > duty_target) duty_curr = duty_target;
      }
      else {
        duty_curr -= duty_chg_per_interval;
        if(duty_curr < duty_target) duty_curr = duty_target;
  }//[3034] servo현재 위치가 servo목표위치에 도달하기전이면 도달하기전까지 duty_chg_per_interval를 증감시킴. 만약 servo 현재 위치가 servo 목표위치를 벗어난다면 servo현재위치를 servo목표위치로 고정

    // update servo position

  }
  
  if(event_serial) {
    event_serial = false;               // [3030]
    Serial.print("Min:0,Low:200,dist_target:255,dist_ir:");
    Serial.print(dist_raw);
    Serial.print(",pterm:");
    Serial.print(pterm);
    Serial.print(",duty_target:");
    Serial.print(duty_target);
    Serial.print(",duty_curr:");
    Serial.print(duty_curr);
    Serial.println(",High:310,Max:2000");
  }
}

float ir_distance(void){ // return value unit: mm
  float val; //[3031] 
  float volt = float(analogRead(PIN_IR)); //[3031]
  val = ((6762.0/(volt-9.0))-4.0) * 10.0; //[3031]
  return val; //[3031]
}

float ir_distance_filtered(void){ // return value unit: mm
  return ir_distance(); // for now, just use ir_distance() without noise filter.
}