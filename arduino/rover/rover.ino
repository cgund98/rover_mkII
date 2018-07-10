#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Encoder.h>
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <geometry_msgs/Twist.h>

ros::NodeHandle nh;

int enA = 10;
int in1 = 9;
int in2 = 8;

int enB = 5;
int in3 = 7;
int in4 = 6;

Encoder leftEncoder(2, 11);
Encoder rightEncoder(3, 12);

// ROS Publishers
std_msgs::UInt16 lwheelData;
std_msgs::UInt16 rwheelData;
ros::Publisher lwheel("lwheel", &lwheelData);
ros::Publisher rwheel("rwheel", &rwheelData);

void setup()
{
  Serial.begin(57600);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // ROS Node Init
  nh.initNode();
  nh.advertise(lwheel);
  nh.advertise(rwheel);

}

long positionLeft  = 0;
long positionRight = 0;

boolean stopMotors = true;

void loop()
{
  long newLeft, newRight;
  newLeft = leftEncoder.read();
  newRight = rightEncoder.read();
  if (newLeft != positionLeft || newRight != positionRight) {
    // Serial.print("Left = ");
    // Serial.print(newLeft);
    // Serial.print(", Right = ");
    // Serial.print(newRight);
    // Serial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  // // if a character is sent from the serial monitor,
  // // reset both back to zero.
  // if (Serial.available()) {
  //   Serial.read();
  //   Serial.println("Reset both knobs to zero");
  //   leftEncoder.write(0);
  //   rightEncoder.write(0);
  // }
  //
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 100);
  analogWrite(enB, 180);

  lwheelData.data = positionLeft;
  rwheelData.data = positionRight;
  lwheel.publish(&lwheelData);
  rwheel.publish(&rwheelData);

  nh.spinOnce();

  if (stopMotors) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }

  delay(20);


}