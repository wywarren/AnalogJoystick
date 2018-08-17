/* Analog Joystick with Calibration
 * Last Revision: 2018-08-17
 * By: Warren Wang @ Qoobit Productions
 * E-mail: warren@qoobit.com
 * --------------------------------
 * This is an example of setting up an analog joystick 
 * to interface with arduino and to calibrate the input
 * values between -1 and 1 with the bottom left of the
 * joystick being the origin. After calibration, the 
 * values are saved to EEPROM so that the next time the
 * controller is turned on, the values are retained.
 * 
 * Keep in mind even at rest position, the raw input
 * value is noisy. When using, make sure vector 
 * magnitude is beyond an arbitrary threshold before
 * firing events. Most raw input value deviations are
 * ~=1 (on a scale of 0-1023).
 * 
 * In this example, the switch press is the toggle for
 * calibration. Remember to remap the calibration mode
 * to your own use case so that you can reuse the
 * switch for other purposes.
 * 
 * CONNECTIONS (JOYSTICK -> ARDUINO)
 * ---------------------------------
 * 5V -> 5V
 * GND -> GND
 * X -> A0
 * Y -> A1
 * SW -> D2 (PIN 2)
 * 
*/
#include <EEPROM.h>

const int SW_pin = 2;
const int X_pin = 0;
const int Y_pin = 1;
const int center = 512;         // Raw input values are between 0 and 1023, choose midpoint as reset

int in_X, in_Y, switchState;    // Raw input values
int center_X, center_Y;         // Reference center position for calculations
int min_X, max_X, min_Y, max_Y; // Store MIN/MAX values during calibration
int current_X, current_Y;       // Adjusted raw values based on calibration center position
float x,y;                      // Final calibrated x and y values between -1 and 1

bool calibration = false;       // Calibration mode is ON/OFF

int prevSwitchState=1;          // Keep track of switch's state for toggling

void resetCalibration(){
  min_X = max_X = min_Y = max_Y = center_X = center_Y = center;
}
void loadCalibration(){
  int addr = 0;
  EEPROM.get(addr, center_X);
  addr+=sizeof(int);
  EEPROM.get(addr, center_Y);
  addr+=sizeof(int);
  EEPROM.get(addr, min_X);
  addr+=sizeof(int);
  EEPROM.get(addr, max_X);
  addr+=sizeof(int);
  EEPROM.get(addr, min_Y);
  addr+=sizeof(int);
  EEPROM.get(addr, max_Y);
  addr+=sizeof(int);

  Serial.println("LOADED FROM EEPROM");
  Serial.println("------------------");
  Serial.print("CENTER X: ");
  Serial.println(center_X);
  Serial.print("CENTER Y: ");
  Serial.println(center_Y);
  Serial.print("MIN X: ");
  Serial.println(min_X);
  Serial.print("MAX X: ");
  Serial.println(max_X);
  Serial.print("MIN Y: ");
  Serial.println(min_Y);
  Serial.print("MAX Y: ");
  Serial.println(max_Y);
}

void saveCalibration(){
  int addr = 0;
  EEPROM.put(addr, center_X);
  addr+=sizeof(int);
  EEPROM.put(addr, center_Y);
  addr+=sizeof(int);
  EEPROM.put(addr, min_X);
  addr+=sizeof(int);
  EEPROM.put(addr, max_X);
  addr+=sizeof(int);
  EEPROM.put(addr, min_Y);
  addr+=sizeof(int);
  EEPROM.put(addr, max_Y);
  addr+=sizeof(int);
  Serial.println("SAVED TO EEPROM");
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  calibration = false;
  resetCalibration();
  loadCalibration();
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  
}

void printCalibrationStatus(){
  Serial.print("INPUT: ");
  Serial.print(in_X);
  Serial.print(" ");
  Serial.print(in_Y);
  Serial.print(" CENTER: ");
  Serial.print(center_X);
  Serial.print(" ");
  Serial.print(center_Y);
  Serial.print(" MIN/MAX: ");
  Serial.print(min_X);
  Serial.print(" ");
  Serial.print(max_X);
  Serial.print(" ");
  Serial.print(min_Y);
  Serial.print(" ");
  Serial.print(max_Y);
  Serial.print("\n");
}

void printJoystickStatus(){
  Serial.print("Switch: ");
  if(switchState==0){
     Serial.print("DOWN");
  }
  else{
     Serial.print("UP");
  }
  Serial.print(" X-axis: ");
  Serial.print(x);
  Serial.print(" Y-axis: ");
  Serial.print(y);
  Serial.print("\n");
}

void loop() {
  // Read raw values from analog joystick
  switchState = digitalRead(SW_pin);
  in_X = analogRead(X_pin);
  in_Y = analogRead(Y_pin);
  
  // Toggle calibration on switch press down
  // NOTE : switchState 0 is the pressed down state
  if(switchState==0 && prevSwitchState != switchState){
    calibration = !calibration;
    if(calibration){
      resetCalibration();
    }
    else{
      //save to EEPROM
      saveCalibration();
    }
    // Set switch pressed position as the center origin
    center_X = in_X;
    center_Y = in_Y;
  }

  if(calibration){
    if(in_X<min_X) min_X=in_X;
    if(in_X>max_X) max_X=in_X;
    if(in_Y<min_Y) min_Y=in_Y;
    if(in_Y>max_Y) max_Y=in_Y;

    //printCalibrationStatus();
  }
  else{
    int current_X = in_X-center_X;
    int current_Y = in_Y-center_Y;
    
    //reintepolate between -1 and 1 depending on calibration - bottom left as -1,-1
    if(current_X<=0){
      x = (float)-current_X/(float)(min_X-center_X);
    }
    else{
      x = (float)current_X/(float)(max_X-center_X);
    }
    if(current_Y<=0){
      y = (float)current_Y/(float)(min_Y-center_Y);
    }
    else{
      y = (float)-current_Y/(float)(max_Y-center_Y);
    }
    
    //printJoystickStatus();
  }
  
  prevSwitchState = switchState;
  
  delay(10);
}
