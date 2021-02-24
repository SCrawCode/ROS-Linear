//A Simple Proportional Controller Node To Command TurtleBot Pathing

#include"ros/ros.h"
#include"geometry_msgs/Twist.h"
#include"geometry_msgs/Pose2D.h"
#include"turtlesim/Pose.h"
#include <cstdlib>
#include <iostream>
using namespace std;

//Global Variables
geometry_msgs::Twist cmdVel;
geometry_msgs::Pose2D current;
geometry_msgs::Pose2D desired;

//Changes goal to any value from 0-11
const double GOAL = 3;

//Coefficient (Gain) for linear velocity
const double K1 = 1;

//Distance tolerance
const double distanceTolerance = .1;

//Setup Function
void misc_setup()
{
  desired.x = GOAL;
  cmdVel.linear.x = 0;
  cmdVel.linear.y = 0;
  cmdVel.linear.z = 0;
  cmdVel.angular.x = 0;
  cmdVel.angular.y = 0;
  cmdVel.angular.z = 0;
}

//Callback Function to update current location 
void update_pose(const turtlesim::PoseConstPtr&currentPose)
{
  current.x = currentPose->x;
  current.y = currentPose->y;
  current.theta = currentPose->theta;
}

//Helper Functions
double getDistanceError()
{
  return desired.x - current.x;
}

//Controller Functions
void set_velocity()
{
  if (abs(getDistanceError()) > distanceTolerance)
    {
     cmdVel.linear.x = K1 * getDistanceError();
    }
  else
    {
     cout << "Here's JOHNNY!\n";
     cmdVel.linear.x = 0;
    }
}

//Main function
int main(int argc, char **argv)
{
//9.1
 misc_setup();

//9.2 Register node "go_to_x" with roscore & get Nodehandle
 ros::init(argc, argv, "simple_pc");
 ros::NodeHandle node;

//9.3 Subscribe to topic and set callback
 ros::Subscriber subCurrentPose = node.subscribe("turtle1/pose", 0, update_pose);

//9.4 Register node as publisher
 ros::Publisher pubVelocity = node.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 0);

//9.5.1 Loop Frequency
 ros::Rate loop_rate(10);  //10 cycles/second

//9.5 Execute loop till master disconnection
 while (ros::ok)
  {
   //9.5.2 Call callbacks waiting to be called
    ros::spinOnce();

   //9.5.3 Call controller after callbacks are done
    set_velocity();

   //9.5.4 Publish Messages
    pubVelocity.publish(cmdVel);

   //9.5.5 Output Messages
    cout << "Goal x: " << desired.x << endl
    << "Current x: " << current.x << endl
    << "Dis Error: " << getDistanceError() << endl
    << "cmd_vel: " << cmdVel.linear.x << endl;

   //Set the frequency for 10Hz
    loop_rate.sleep();
  }

//9.6 No Hardware to shutdown
  return 0;
}
