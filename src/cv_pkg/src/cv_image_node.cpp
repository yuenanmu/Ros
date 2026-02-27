#include<ros/ros.h>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;

void CamRGBCallback(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;//一个CvImagePtr类型的指针
    try{
        cv_ptr=cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);//将ROS图像消息转换为OpenCV图像，并且指定编码格式，赋值给指针
    }
    catch(cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception:%s",e.what());
        return;
    }
    Mat img=cv_ptr->image;//定义一个openCV的图像对象
    imshow("view",img);
    waitKey(1);//暂停1ms回调函数.如果不加这个函数，图像窗口会卡死，无法刷新显示新的图像

}
int main(int argc, char** argv)
{
    ros::init(argc, argv, "cv_image_node");
    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("/kinect2/qhd/image_color_rect",1,&CamRGBCallback);

    ros::Rate loop_rate(10);

    namedWindow("view");
    ros::spin();
}
