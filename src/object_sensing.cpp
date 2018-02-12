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
static const std::string OPENCV_WINDOW = "environment";
static const std::string HAAR_WINDOW = "applied";




/** Global variables */
String HOG_object = "/home/ros/classifier/HOGBlock.xml";
CascadeClassifier object_HOGCascade;

string window_name = "Object sensing";
RNG rng(12345);


class ImageDiverter
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
  ImageDiverter()
    : it_(node_),update_image_(false), running_(false),spinner_(0)
  {
  }

  ~ImageDiverter()
  {
  }


  void imageViewer()
  {
    cv::Mat img;

    int counter;

    bool_publisher =node_.advertise<robot_communication::demo_msg>("/demo_topic",1000);

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








        if( !object_HOGCascade.load(HOG_object) ){ printf("--(!)Error loading\n"); break; };

        if( !crop.empty() )
        {
          //stringstream file;
          display_bool=detectAndDisplay(crop);
          msg.object=display_bool;
          bool_publisher.publish(msg);
          imshow(window_name,crop);
          //file << "/home/ros/classifier/diff_env/" << counter << ".jpg";
          //counter++;
          //imwrite(file.str(),frame);
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

    sub_ = it_.subscribe("/kinect2/hd/image_color_rect",5, &ImageDiverter::imageCb, this);


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

  //function to display

  bool detectAndDisplay( Mat &frame )
  {
    std::vector<Rect> object;
    Mat frame_gray;
    bool boolshow;

    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect object
    object_HOGCascade.detectMultiScale( frame_gray, object, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(60, 60), Size(90,90));

    for( size_t i = 0; i < object.size(); i++ )
    {
      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      rectangle( frame, object[i].tl() , object[i].br(), color, 2, 8, 0 );
    }
    cout<<object.size()<<endl;
    cout<<boolshow;
    if(object.size()>0)
    {
      return true;}
    else{
      return false;
    }
    //-- Show what you got

  }



};









int main(int argc, char **argv)
{
  ros::init(argc, argv,"object_sensing");


  if(!ros::ok())
  {
    return 0;
  }


  //Class to display and forward the images


  ImageDiverter id;
  id.run();


  //Close the ROS node


  ros::shutdown();

  return 0;

}
