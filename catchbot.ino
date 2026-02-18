//0.04m per 200 steps, 0.0002m per 1 step

//import libraries
#include <AccelStepper.h> 
#include <math.h>

//defines all the variables/makes sure they exist
const int sensor1 = 8; //first sensor
const int sensor2 = 7; //second sensor
unsigned long time1, time2, steps; //variables for first trigger, second trigger, and the amount of steps
bool trip = false;//sets trip to false
float deltaTime, velocity, landingTime, deltaX; //
float sensorDistance = 0.045;//distance between sensors
float gravity = 9.8;//acceleration of gravity
float tableHeight = 0.73; //(table height) change
AccelStepper step(AccelStepper::DRIVER, 2, 5); //set stepper pins
float stepsPerMeter = 5000;//the steps/meter




void setup() {
  // put your setup code here, to run once:
  //set stepper constants
  step.setSpeed(20000);
  step.setCurrentPosition(0);
  step.setMaxSpeed(20000); //stepper speed
  step.setAcceleration(100000); //stepper acceleration

//set ir sensors to input
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  //start serial monitor
  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:
  //renames ir readings
  int read1 = digitalRead(sensor1); 
  int read2 = digitalRead(sensor2);


//if the first sensor senses an object and has not been tripped, record the time and set trip to true
  if(read1 == LOW && !trip){ 
    time1 = millis();
    trip = true;
    // Serial.println("1");
  }

//if the second sensor senses an object and has been tripped, record the time and set trip to false
  if(read2 == LOW && trip){
    time2 = millis();
    trip = false;
    // Serial.println("2");

//math yay
  deltaTime = (time2 - time1) / 1000.000;//calculates change in time and changes milliseconds to seconds
  velocity = sensorDistance / deltaTime;//calculates the variable velocity as the distance/the change in time
  landingTime = (velocity + (sqrt((velocity * velocity) + (2 * gravity * tableHeight)))) / gravity; //calculates the variable landingTime which is the amount of time until it hits the ground (sob)
  deltaX = velocity * landingTime;//how far in the sensorDistance axis the ball will travel
  steps = deltaX * stepsPerMeter;//tells us the amount of steps it will take for the motor to get to the location to catch the ball

//prints the velocity on the sensorDistance axis in m/s and k/h
  // Serial.print(velocity);
  // Serial.println(" m/s");
  // Serial.print(velocity * 3.6);
  // Serial.println(" k/h");
 
 //prints the landing point in meters away
  // Serial.print("landing point: ");//prints “landing point: ”
  // Serial.print(deltaX);//prints the predicted landing point on the sensorDistance axis
  // Serial.println("m away");//prints “m away”

//moves motor to the predicted landing point
    step.moveTo(steps);
    while (step.distanceToGo() != 0) {
      step.run();
    }
    delay(5000);

//moves motor back to 0 after 5 seconds
    stepper.moveTo(0);  
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }


    }
  }





