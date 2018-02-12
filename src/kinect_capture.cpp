#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "robot_communication/demo_msg.h"
#include <iostream>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <boost/thread/thread.hpp>
#include <boost/chrono/chrono.hpp>
#include <ros/spinner.h>
#include <ros/callback_queue.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>


using namespace std;
using namespace cv;
static const std::string window_name = "environment";




class kinect_capture
{
private:
  ros::NodeHandle node_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber sub_;
  ros::Publisher bool_publisher;
  cv::Mat frame;
  boost::mutex lock_;
  bool update_image_;
  bool running_;
  ros::AsyncSpinner spinner_;
  robot_communication::demo_msg msg;
  bool display_bool;

public:
  kinect_capture()
    : it_(node_),update_image_(false), running_(false),spinner_(0)
  {
  }

  ~kinect_capture()
  {
  }  

void imageViewer()
  {
    cv::Mat img;

    int counter;

    while(running_ && ros::ok())
    {


      if(update_image_)
      {
        lock_.lock();
        img = this->frame;
        update_image_ = false;
        lock_.unlock();
       // cv::imshow(OPENCV_WINDOW, img);
       // cv::waitKey(0);

	    cv::Rect roi(490,210,1260,690);
	    //cv::rectangle(img,roi,cv::Scalar(255),1,8,0); //to show region
	    cv::Mat crop=img(roi); //applying crop




        if( !crop.empty() )
        {
             
 	    stringstream file;    //only use it to save the image into a specific file            
  	    file << "/home/ros/classifier/project_pic/" << counter << ".jpg";
            counter++;
            imwrite(file.str(),crop);
	    imshow(window_name,crop);   
            char key = waitKey(10);
            if ( key == 27)
            {break;}


        }
        else
        {
            printf("No image frame");

        }



      }
    }


    cv::destroyAllWindows();
  }



  void imageCb(const sensor_msgs::Image::ConstPtr& msg)
  {

    cv:Mat img;
    cv_bridge::CvImageConstPtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    cv_ptr->image.copyTo(img);


    lock_.lock();
    this->frame = img;
    update_image_ = true;
    lock_.unlock();

  }

  void run()
  {

    running_ = true;

    sub_ = it_.subscribe("/kinect2/hd/image_color_rect",5, &kinect_capture::imageCb, this);


    spinner_.start();


    boost::chrono::milliseconds duration(1);
    while(!update_image_)
    {
      if(!ros::ok())
      {
        return;
      }
      boost::this_thread::sleep_for(duration);


    }

     imageViewer();

     spinner_.stop();

      running_ = false;






  }


};


int main(int argc, char **argv)
{
  ros::init(argc, argv,"kinect_capture");


  if(!ros::ok())
  {
    return 0;
  }


//Class to display and forward the images


  kinect_capture object;
  object.run();


//Close the ROS node


  ros::shutdown();

  return 0;

}
