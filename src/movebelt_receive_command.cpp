#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "robot_communication/demo_msg.h"
#include <iostream>
#include <stdio.h>
#include<math.h>
#include<sensor_msgs/Image.h>
#include <boost/thread/thread.hpp>
#include <boost/chrono/chrono.hpp>
#include <ros/spinner.h>
#include <ros/callback_queue.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include"pick_and_place/object_coordinate.h"
#include"learning_tf/diff_pos.h"
#include <ros/callback_queue.h>
#include <sstream>
#include<math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define PORT  1001

using namespace std;




class Controller
{
private:
  ros::NodeHandle node_;
  ros::Subscriber sub1_,sub2_;
  ros::Publisher bool_publisher;
  boost::mutex lock_;
  bool update_x_,execution;
  bool running_;
  ros::AsyncSpinner spinner_;
  robot_communication::demo_msg msg;
  bool display_bool;
  char check[30],up_z[300],place[200],send_x[200],send_y[200],send_z[200],initial[100], char_y[100], char_z[100], final_s[100],char_x[100];
  double x;
  double y;
  double z;
  double final_z;
  int m_socket;
  int iPort;
  bool bConnected;
  bool status, send,rcv,execution_status;
  const char* c_Host;
  char ReadString[5000];

public:
  Controller()
    : running_(false),spinner_(0),x(0.0),y(0.0),z(0.0),iPort(PORT),bConnected(false), c_Host("192.168.0.124"), status(false),final_z(0.0),send(false),rcv(false),execution_status(false),update_x_(false),execution(false)
  {
  }

  ~Controller()
  {
  }


  //Function for making the socket communication with particular protocol
  bool ClientConnect(const char* host, int port, int iIdSocket)

  {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);
    if(connect(iIdSocket, (sockaddr *) &addr, sizeof(sockaddr)) <0)
    {
      return false;
    }
    else
    {
      return true;
    }

  }

  void coordinateCallback(const pick_and_place::object_coordinate& object)
  {


    double x_check;
    // ros::Rate loop_rate(0.4);

    // In the following lines object coordinate is coming from pcl_filter through /object_current_pos topic
    x=object.PosX;
    y=object.PosY;
    z=object.PosZ;

    //in the following lines xyz coordinate's values are going to be rounded as they are very fluctuating after 3 decimal value
    x=floor(x*100)/100;
    y=floor(y*100)/100;
    z=floor(z*100)/100;

    final_z= 0.15-(z-0.71);
    //here we write the equation for getting the distance from the end effector tip to object
    x=x*1000;
    x=1000-x;
    y= y+0.028;
    y= y*1000;
    final_z= final_z*1000;


    // loop_rate.sleep();

  }

  void presenceCallback(const robot_communication::demo_msg& msg)
  {
    //ros::Rate loop_rate(1);
    status = msg.object;
    //ROS_INFO("satus is:",status);
    //loop_rate.sleep();
  }

  void sendstring()
  {

    double x_check,given_y,given_final_z;
    double distance,time,speed;

    speed = 203.00;


    while(rcv && ros::ok())
    {
      //if(update_x_)
      // {
      lock_.lock();
      x_check=this->x;
      given_final_z=this->final_z;
      given_y=this->y;
      execution_status=this->status;
      lock_.unlock();


      distance= x_check-550;

      time = distance/speed;
      time = time-1;




      execution=true;
      update_x_ = false;

      if(execution_status==true && time >=0)
      {

        ROS_INFO("Received X coordinate is: %f",x_check);
        ROS_INFO("Received Y coordinate is: %f",given_y);


        //do the calcaution for distance and time
        while(execution)
        {

          // if(send==true)
          //{
          ROS_INFO("Time calculated is : %f",time);


          strcpy(send_y,"1,550,");
          // sprintf(char_x,"%f",x);
          // strcat(send_y,char_x);
          // strcat(send_y,",");
          sprintf(char_y,"%f",given_y);
          strcat(send_y,char_y);
          //---for z
          strcpy(send_z,send_y); //later execution of z axis action and sucking of robot ..here string is "1,550,y"
          strcpy(up_z,send_y);  //later execution of getting up position ..here string is "1,550,y"
          strcat(send_y,",100,0,0,0,30,0\r\n");
          //  sleep(1);


          send = write(m_socket,send_y,strlen(send_y)); //comes to the position of the object
          ROS_INFO("goes toward object...second position is send");
          strcpy(check,"3,0,0,0,0,0,0,0,0\r\n");
          send = write(m_socket,check,strlen(check));
          rcv=recv(m_socket,ReadString,5000,0);

          //ros::spinOnce();

          if(rcv==true)
          {
            sleep(time);
            strcat(send_z,",");
            sprintf(char_z,"%f",15.00);
            strcat(send_z,char_z);

            strcat(send_z,",0,0,0,40,2\r\n");// here the sucking position string is "1,550,y,z,0,0,0,10,2\r\n"
            //  sleep(1);
            send = write(m_socket,send_z,strlen(send_z)); //z axis is lowered
            ROS_INFO("Z axis is lowered...third position is send");
            //  sleep(1);


            if(send==true)
            {


              strcpy(place,"1,0,-550,150,0,0,0,20,0\r\n");  //arbitary position to drop the object on the plate
              send = write(m_socket,place,strlen(place));
              ROS_INFO("goes to arbitary position");

              // sleep(1);
              if(send==true)
              {
                strcpy(final_s,"1,0,-550,150,0,0,0,20,1\r\n");  //drop the object on the arbitary position
                send = write(m_socket,final_s,strlen(final_s));
                ROS_INFO("drops the object");



              }
              else
              {
                ROS_INFO("The object dropped position string is not send to the controller");
              }



            }
            else
            {
              ROS_INFO("up position string not send to the controller");
            }
          }
          else
          {
            ROS_INFO("Z coordinate not send to the controller");
          }




          execution_status=false;
          execution=false;


        }

      }
      else
      {

        strcpy(final_s,"1,210,-450,100,0,0,0,20,0\r\n");  //drop the object on the arbitary position
        send = write(m_socket,final_s,strlen(final_s));
        //sleep(2);
        ROS_INFO("Received X coordinate is: %f",x_check);
        ROS_INFO("staying at the initial position while no object present");
        //sleep(1);
      }


      sleep(1);

    }





  }

  void execute()
  {

    running_ = true;
    m_socket = socket(AF_INET, SOCK_STREAM, 0);

    sub1_ = node_.subscribe("/object_current_pos", 10,  &Controller::coordinateCallback, this);

    sub2_ = node_.subscribe("/demo_topic", 10,  &Controller::presenceCallback,this);

    do
    {

      bConnected = ClientConnect(c_Host,iPort,m_socket); //do connection with the socket
      ROS_INFO("Trying to connect with the controller");

      if(bConnected == true)
      {
        ROS_INFO( "connected");
      }
      else
      {
        ROS_INFO("Notconnected");
      }
    }
    while(bConnected!=true);

    do
    {
      ROS_INFO("checking if its alive...");
      strcpy(check,"3,0,0,0,0,0,0,0,0\r\n");     //for reading the position of the robot from the controller
      //  strcpy(check,"4,0,0,0,0,0,0,0,0\r\n");
      send = write(m_socket,check,strlen(check));
      rcv=recv(m_socket,ReadString,5000,0);

    }
    while(rcv!=true);

    ROS_INFO("servo motor is on");

    while(ros::ok())
    {

      spinner_.start();
      //update_x_=true;


      sendstring();






      spinner_.stop();

      //  running_ = false;
    }

  }


};



int main(int argc, char **argv)
{

  ros::init(argc, argv, "receive_command"); //create a node name "receive_command"


  if(!ros::ok())
  {
    return 0;
  }


  //Class to display and forward the images

  Controller ob;
  ob.execute();


  //Close the ROS node


  ros::shutdown();

  return 0;

}
