#include "ros/ros.h"
#include "std_msgs/String.h"

#include"robot_communication/test_coordinate.h"
#include"robot_communication/current_pos_robot.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv,"coordinate_publisher");
  ros::NodeHandle nh;
  double x,y,z,c;

  x=23.4;
  y=80.3;
  z=23.2;
  c=100.1;

  ros::Publisher chatter_pub = nh.advertise<robot_communication::test_coordinate>("coordinate", 1000);
  robot_communication::test_coordinate data;
  data.PosX=x;
  data.PosY=y;
  data.PosZ=z;
  data.PosC=c;

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    //std_msgs::String msg;
    //msg.data = "hello world";

    chatter_pub.publish(data);
    ROS_INFO("Coordinate published");

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
