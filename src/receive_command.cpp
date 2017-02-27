
/**********************************************************************************************************************************

  Description:This program is used for communication with the robot(TH650) Controller.
  Auther     :Hirenbhai Patel & Avinash Jain

 **********************************************************************************************************************************/

#include <ros/ros.h>
#include"std_msgs/String.h"
#include"robot_communication/connection_msg.h"
#include"robot_communication/current_pos_robot.h"
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<iostream>
#include <unistd.h>
#include<QString>
#include<QStringList>
#define PORT  1001

using namespace std;
int m_socket;

//Function for making the socket communication with particular protocol
bool ClientConnect(const char* host, int port, int iIdSocket)

{
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(host);
  if(connect(iIdSocket, (sockaddr *) &addr, sizeof(sockaddr)) <0){
    return false;
  }else{
    return true;
  }
}


void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  char ReadString[500];
  char c_Send[30];
  bool b_Send;
  double x,y,z,c;
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<robot_communication::current_pos_robot>("/robot_current_pos",1000); // creat publisher which publish a data on a topic "/robot_current_pos"
  robot_communication::current_pos_robot robot_pose_msg;
  QString toSplitString;

  strcpy(c_Send,msg->data.c_str());
  b_Send = write(m_socket,c_Send,strlen(c_Send)); // send the data to the robot controller
  sleep(1);

  recv(m_socket,ReadString,500,0); // read the data from robot controller
  toSplitString = QString::fromUtf8(ReadString);
  QStringList ReceiveList = toSplitString.split(" "); //convert data in QStringlist
  ReceiveList.removeFirst(); //remove "LESE" from the data
  for(int i = 0; i < ReceiveList.size(); i++)
  {
    if(ReceiveList.value(i).toDouble() == 0)
    {
      ReceiveList.removeAt(i); //Remove any empty spaces
      i = i - 1;
    }
  }

  //assign the values
  x = ReceiveList.value(0).toDouble();
  y = ReceiveList.value(1).toDouble();
  z = ReceiveList.value(2).toDouble();
  c = ReceiveList.value(3).toDouble();

  cout<<"x ="<<x<<endl;
  cout<<"y ="<<y<<endl;
  cout<<"z ="<<z<<endl;
  cout<<"c ="<<c<<endl;

  robot_pose_msg.PosX=x;
  robot_pose_msg.PosY=y;
  robot_pose_msg.PosZ=z;
  robot_pose_msg.PosC=c;
  pub.publish(robot_pose_msg); //publish the current position of the robot on a topic "/robot_current_pos"
}

int main(int argc, char **argv)
{
  // Set up ROS.
  ros::init(argc, argv, "receive_command"); //create a node name "receive_command"
  ros::NodeHandle n; //create a object for node handle
  int iPort = PORT;
  bool bConnected;
  const char* c_Host = "192.168.0.124"; //ip address of the robot controller

  m_socket = socket(AF_INET, SOCK_STREAM, 0); //create a socket
  bConnected = ClientConnect(c_Host,iPort,m_socket); //do connection with the socket

  if(bConnected == true)
  {
    cout << "connected";
  }
  else
    cout << "Notconnected";

  ros::Subscriber sub = n.subscribe("/send_string", 1000, chatterCallback); // subscribe a topic "/send_string"

  ros::spin();
  close(m_socket); //close the socket

  return 0;
}



