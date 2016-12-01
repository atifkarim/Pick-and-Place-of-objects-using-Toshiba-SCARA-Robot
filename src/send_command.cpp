#include <ros/ros.h>
#include"std_msgs/String.h"


int main(int argc, char **argv)
{
  // Set up ROS.
  ros::init(argc, argv, "send_command");
  ros::NodeHandle n;
  ros::Publisher send_command_pub = n.advertise<std_msgs::String>("/send_string", 1000);
  ros::Rate loop_rate(10);

  char c_Send[30];
  std_msgs::String msg;

  strcpy(c_Send,"1,400,500,110,0,0,0,20,1\r\n");
  msg.data = c_Send;

  while (ros::ok())
  {
    send_command_pub.publish(msg);
    ros::spinOnce();
    //ROS_INFO("%s",msg.data);
    loop_rate.sleep();

  }

  return 0;


}
