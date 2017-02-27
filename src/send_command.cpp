
/**********************************************************************************************************************************

  Description:This program is used for sending the string data to receive_command node.
  Auther     :Hirenbhai Patel & Avinash Jain

 **********************************************************************************************************************************/

#include <ros/ros.h>
#include"std_msgs/String.h"
#include<vector>
#include<string.h>
#include"pick_and_place/object_coordinate.h"

void object_to_string(const pick_and_place::object_coordinate msg1)
{

}

int main(int argc, char **argv)
{
  // Set up ROS.
  ros::init(argc, argv, "send_command"); //create a node name "send_command"
  ros::NodeHandle n;
  ros::Publisher send_command_pub = n.advertise<std_msgs::String>("/send_string", 1000);// create publisher which publish a data on a topic "/send_string"
  ros::Subscriber receive_object = n.subscribe<pick_and_place::object_coordinate>("/object_current_pos",10, object_to_string);
  ros::Rate loop_rate(10);

  char c_Send[30];
  std_msgs::String msg;

  strcpy(c_Send,"3,0,0,0,0,0,0,0,0\r\n");
  msg.data = c_Send;

  while (ros::ok())
  {
    send_command_pub.publish(msg); //publish the data on a topic "/send_string"
    ros::spinOnce();
    //ROS_INFO("%s",msg.data);
    loop_rate.sleep();

  }
  return 0;
}
