#include <ros/ros.h>
#include"std_msgs/String.h"
#include<vector>
#include<string.h>


//std::string DoubleToRobotString(double Funct, double X, double Y, double Z, double C, double T, double Shoudler, double Speed, double G)
//{
//  std::vector<double> Input;
//  Input.push_back(Funct);
//  Input.push_back(X);
//  Input.push_back(Y);
//  Input.push_back(Z);
//  Input.push_back(C);
//  Input.push_back(T);
//  Input.push_back(Shoudler);
//  Input.push_back(Speed);
//  Input.push_back(G);

//  std::string RobotString;
//  for(int i = 0; i < 9; i++){
//    if(i == 8){
//      RobotString = RobotString+QString::number(Input[i], 'g', 6)+"\r\n";
//      break;
//    }
//    RobotString = RobotString+QString::number(Input[i], 'g', 6)+",";
//  }
//  return RobotString;
//}

//char *StringToCharP(std::string InputString){
//  std::string str = InputString.toStdString();
//  char *cstr = new char[str.length() + 1];
//  strcpy(cstr, str.c_str());
//  return cstr;
//}


int main(int argc, char **argv)
{
  // Set up ROS.
  ros::init(argc, argv, "send_command");
  ros::NodeHandle n;
  ros::Publisher send_command_pub = n.advertise<std_msgs::String>("/send_string", 1000);
  ros::Rate loop_rate(10);

  char c_Send[30];
  std_msgs::String msg;

  strcpy(c_Send,"3,0,0,0,0,0,0,0,0\r\n");
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
