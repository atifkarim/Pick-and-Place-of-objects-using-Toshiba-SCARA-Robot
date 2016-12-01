#include <ros/ros.h>
#include "std_msgs/String.h"



void chatterCallback(const std_msgs::String::ConstPtr& msg)
  {
    ROS_INFO("I heard: [%s]", msg->data.c_str());
  }


int main(int argc, char **argv)
{
  // Set up ROS.
  ros::init(argc, argv, "receiver");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
  ros::spin();
  return 0;
  //ROS_INFO("Hello world!");
}
