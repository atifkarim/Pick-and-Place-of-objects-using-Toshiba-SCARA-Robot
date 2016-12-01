#include <ros/ros.h>
#include"std_msgs/String.h"
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


std::string ReceiveString()
{
  int recv_size;
  std::vector<std::string> antwort;
  while(1){
    char server_reply[100];
    recv_size = recv(m_socket, server_reply, 100, 0);
    server_reply[recv_size]= '\0';
    std::string reply_string(server_reply);
    antwort.push_back(reply_string);
    if(server_reply[recv_size-1] == '*'){
      break;
    }
  }
  return antwort[antwort.size()-1];
}


bool IsAlive()
{
  char active[30];
  strcpy(active,"4,0,0,0,0,0,0,0,0\r");
  write(m_socket,active,strlen(active));
  std::string antwort = ReceiveString();
  if(antwort != ""){
    return true;
  }
  else{
    return false;
  }
}

void chatterCallback(const std_msgs::String::ConstPtr& msg)
  {
    char ReadString[500];
    char c_Send[30];
    bool b_Send;
    strcpy(c_Send,msg->data.c_str());
    b_Send = write(m_socket,c_Send,strlen(c_Send));
    sleep(2);
//    for(int iii =0;iii < 10 ; iii++)
//    {
//          recv(m_socket,ReadString,500,0);
//          for(int i=0;i<=strlen(ReadString);i++)
//          {
//             cout<<ReadString[i]<<endl;
//          }
//    }

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



