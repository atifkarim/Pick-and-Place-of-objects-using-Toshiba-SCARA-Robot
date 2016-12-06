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
  ros::Publisher pub = n.advertise<robot_communication::current_pos_robot>("/robot_current_pos",1000);
  robot_communication::current_pos_robot robot_pose_msg;
  QString toSplitString;

  strcpy(c_Send,msg->data.c_str());
  b_Send = write(m_socket,c_Send,strlen(c_Send));
  sleep(1);

  recv(m_socket,ReadString,500,0);
  toSplitString = QString::fromUtf8(ReadString);
  QStringList ReceiveList = toSplitString.split(" "); //Konvertierung in QStringlist
  ReceiveList.removeFirst(); //entferne "LESE"
  for(int i = 0; i < ReceiveList.size(); i++)
  {
    if(ReceiveList.value(i).toDouble() == 0)
    {
      ReceiveList.removeAt(i); //entferne Leerstellen
      i = i - 1;				//verschiebe die Schleife bei L�schung
    }
  }
  x = ReceiveList.value(0).toDouble(); //Werte werden Zugeordnet
  y = ReceiveList.value(1).toDouble();
  z = ReceiveList.value(2).toDouble();
  c = ReceiveList.value(3).toDouble();


  robot_pose_msg.PosX=x;
  robot_pose_msg.PosY=y;
  robot_pose_msg.PosZ=z;
  robot_pose_msg.PosC=c;
  pub.publish(robot_pose_msg);
}

int main(int argc, char **argv)
{
  // Set up ROS.
  ros::init(argc, argv, "receive_command");
  ros::NodeHandle n;
  int iPort = PORT;
  bool bConnected;
  const char* c_Host = "192.168.0.124";

  m_socket = socket(AF_INET, SOCK_STREAM, 0);
  bConnected = ClientConnect(c_Host,iPort,m_socket);

  if(bConnected == true)
  {
    cout << "connected";
  }
  else
    cout << "Notconnected";

  ros::Subscriber sub = n.subscribe("/send_string", 1000, chatterCallback);

  ros::spin();
  close(m_socket);

  return 0;
}



