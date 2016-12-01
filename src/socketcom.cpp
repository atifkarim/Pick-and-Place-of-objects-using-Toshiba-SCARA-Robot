#include <ros/ros.h>
#include"std_msgs/Int32.h"
#include"client_communication/connection_msg.h"
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<iostream>
#include <unistd.h>
#define PORT  1001
//#define PORT  11311




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




int main(int argc, char **argv)
{
  // Set up ROS.
  ros::init(argc, argv, "socketcom");
  ros::NodeHandle n;
  ros::Publisher socketId_pub = n.advertise<client_communication::connection_msg>("/socket_flage_id", 1000);
  ros::Rate loop_rate(10);

  int iId_Socket,iPort = PORT;
  bool bConnected;
  const char* c_Host = "192.168.0.124";

  iId_Socket = socket(AF_INET, SOCK_STREAM, 0);

  bConnected = ClientConnect(c_Host,iPort,iId_Socket);

  while (ros::ok())
  {

    // This is a message object. You stuff it with data, and then publish it.

    client_communication::connection_msg msg;
    std::stringstream sIdpublish;

    if(bConnected == true)
    {
      sIdpublish << "connected";
    }
    else
      sIdpublish << "Notconnected";


    msg.connectionFlage = sIdpublish.str();
    msg.socketId = iId_Socket;
    ROS_INFO("%s", msg.connectionFlage.c_str());
    ROS_INFO("%d",msg.socketId);

    // The publish() function is how you send messages. The parameter
    // is the message object. The type of this object must agree with the type
    // given as a template parameter to the advertise<>() call, as was done
    // in the constructor above.

    socketId_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();

  }
}
