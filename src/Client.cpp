#include <ros/ros.h>
//#include"std_msgs/Int32.h"
#include"std_msgs/String.h"
#include"robot_communication/connection_msg.h"
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

/*void chatterCallback(const robot_communication::connection_msg::ConstPtr& msg)
  {
    ROS_INFO("I heard: [%s] and [%d]", msg->connectionFlage.c_str(),msg->socketId);
    m_socket=msg->socketId;
    char c_Send[30];
    char ReadString[500];
    bool b_Send;
    int i;
    strcpy(c_Send,"1,400,400,110,0,0,0,20,1\r");


      b_Send = write(m_socket,c_Send,strlen(c_Send));
      sleep(2);
      if (b_Send)
      {
        cout<<"sent1";
      }
      else
        cout<<"not sent";


    sleep(1);
    for(int iii =0;iii < 10 ; iii++)
    {
          recv(m_socket,ReadString,500,0);
          for(i=0;i<=strlen(ReadString);i++)
          {
              //cout<<"no"<<endl;
              cout<<ReadString[i]<<endl;
          }
    }

  }*/

int main(int argc, char **argv)
{
  // Set up ROS.
  ros::init(argc, argv, "Client");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  //ros::Subscriber sub = n.subscribe("/socket_flage_id", 1000, chatterCallback);
  ros::Rate loop_rate(10);
  int i=0,a;

    char c_Send[30];
    char c_Send_One[30];
    char c_Send_Two[30];
    strcpy(c_Send,"1,400,500,110,0,0,0,20,1\r\n");
    strcpy(c_Send_One,"1,400,450,110,0,0,0,20,1\r\n");
    strcpy(c_Send_Two,"1,400,350,110,0,0,0,20,1\r\n");
    //socket->connectToHost("192.168.0.124", 1001);
    cout<<"Yes"<<endl;
    cout<<strlen(c_Send)<<endl;

    int iId_Socket,iPort = PORT;
    bool bConnected;
    const char* c_Host = "192.168.0.124";

    iId_Socket = socket(AF_INET, SOCK_STREAM, 0);
    m_socket = iId_Socket;
    cout<<m_socket<<endl;

    bConnected = ClientConnect(c_Host,iPort,iId_Socket);

    if(bConnected == true)
    {
      cout << "connected";
    }
    else
      cout << "Notconnected";


      char ReadString[500];

  cout<<"Enter number"<<endl;
  cin>>a;

  if(a==1)
  {

        bool b_Send = write(m_socket,c_Send,strlen(c_Send));
        sleep(2);
        if (b_Send)
        {
          cout<<"sent1";
        }
        else
          cout<<"not sent";

        bool b_Send_One = write(m_socket,c_Send_One,strlen(c_Send_One));
        sleep(2);
        if (b_Send_One)
        {
          cout<<"sent2";
        }
        else
          cout<<"not sent";

        bool b_Send_Two = write(m_socket,c_Send_Two,strlen(c_Send_Two));
        sleep(2
              );
        if (b_Send_Two)
        {
          cout<<"sent3";
        }
        else
          cout<<"not sent";
  }

      sleep(1);
      for(int iii =0;iii < 10 ; iii++)
      {
            recv(m_socket,ReadString,500,0);
            for(i=0;i<=strlen(ReadString);i++)
      {
        //cout<<"no"<<endl;
        cout<<ReadString[i]<<endl;
      }
      }


    close(m_socket);
  ros::spin();
  close(m_socket);
  return 0;
}


/*int i=0,a;

  char c_Send[30];
  char c_Send_One[30];
  char c_Send_Two[30];
  strcpy(c_Send,"1,400,500,110,0,0,0,20,1\r\n");
  strcpy(c_Send_One,"1,400,450,110,0,0,0,20,1\r\n");
  strcpy(c_Send_Two,"1,400,350,110,0,0,0,20,1\r\n");
  //socket->connectToHost("192.168.0.124", 1001);
  cout<<"Yes"<<endl;
  cout<<strlen(c_Send)<<endl;

  int iId_Socket,iPort = PORT;
  bool bConnected;
  const char* c_Host = "192.168.0.124";

  iId_Socket = socket(AF_INET, SOCK_STREAM, 0);
  m_socket = iId_Socket;

  bConnected = ClientConnect(c_Host,iPort,iId_Socket);

  if(bConnected == true)
  {
    cout << "connected";
  }
  else
    cout << "Notconnected";


    char ReadString[500];

cout<<"Enter number"<<endl;
cin>>a;

if(a==1)
{

      bool b_Send = write(m_socket,c_Send,strlen(c_Send));
      sleep(2);
      if (b_Send)
      {
        cout<<"sent1";
      }
      else
        cout<<"not sent";

      bool b_Send_One = write(m_socket,c_Send_One,strlen(c_Send_One));
      sleep(2);
      if (b_Send_One)
      {
        cout<<"sent2";
      }
      else
        cout<<"not sent";

      bool b_Send_Two = write(m_socket,c_Send_Two,strlen(c_Send_Two));
      sleep(2
            );
      if (b_Send_Two)
      {
        cout<<"sent3";
      }
      else
        cout<<"not sent";
}

    sleep(1);
    for(int iii =0;iii < 10 ; iii++)
    {
          recv(m_socket,ReadString,500,0);
          for(i=0;i<=strlen(ReadString);i++)
    {
      //cout<<"no"<<endl;
      cout<<ReadString[i]<<endl;
    }
    }


  close(m_socket);
  return 0;*/
