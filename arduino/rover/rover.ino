#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Encoder.h>
#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
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

// ROS Callbacks
void lmotor_cb( const std_msgs::Float32& cmd_msg){
    float cmd;
    if (cmd_msg.data < 0) {
        cmd = -cmd_msg.data;
        setLeftBackward();
    } else {
        cmd = cmd_msg.data;
        setLeftForward();
    }
    analogWrite(enA, cmd);
}
void rmotor_cb( const std_msgs::Float32& cmd_msg){
    float cmd;
    if (cmd_msg.data < 0) {
        cmd = -cmd_msg.data;
        setRightBackward();
    } else {
        cmd = cmd_msg.data;
        setRightForward();
    }
    analogWrite(enB, cmd);
}

// ROS Subscribers
ros::Subscriber<std_msgs::Float32> lmotor_sub("lmotor_cmd", lmotor_cb);
ros::Subscriber<std_msgs::Float32> rmotor_sub("rmotor_cmd", rmotor_cb);

// ROS Publishers
std_msgs::Int16 lwheelData;
std_msgs::Int16 rwheelData;
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
  nh.subscribe(lmotor_sub);
  nh.subscribe(rmotor_sub);

}

long positionLeft  = 0;
long positionRight = 0;

boolean stopMotors = false;

void setLeftForward() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
}

void setRightForward() {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void setLeftBackward() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
}

void setRightBackward() {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

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
