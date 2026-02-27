#include<ros/ros.h>
#include<cv_bridge/cv_bridge.h>//格式转换器cv_bridge提供了ROS图像消息和OpenCV图像之间的转换功能
#include<sensor_msgs/image_encodings.h>//图像格式编码头文件
#include<opencv2/imgproc/imgproc.hpp>//opencv图像处理函数头文件
#include<opencv2/highgui/highgui.hpp>//opencv界面显示头文件
#include<opencv2/objdetect/objdetect.hpp>
#include<vector>


using namespace cv;
using namespace std;

static CascadeClassifier face_cascade;//定义一个级联分类器对象

static Mat frame_gray;//定义一个灰度图像对象
static vector<Rect> faces;//定义一个矩形对象的向量，用于存储检测到的人脸区域
static vector<Rect>::const_iterator face_iter;//定义一个矩形对象的向量的常量迭代器，用于遍历检测到的人脸区域
static int nFaceCount=0;//定义一个整数变量，用于存储检测到的人脸数量


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
    //将原始图像转换为灰度图像
    cvtColor(img,frame_gray,COLOR_BGR2GRAY);
    //对灰度图像进行直方图均衡化，增强对比度
    equalizeHist(frame_gray,frame_gray);
    //人脸检测：使用级联分类器检测人脸区域，结果保存在faces向量中
    face_cascade.detectMultiScale(frame_gray,faces,1.1,9,0|CASCADE_SCALE_IMAGE,Size(30,30));
    //标注位置
    if(faces.size()>0){
        for(face_iter=faces.begin();face_iter!=faces.end();++face_iter){//从头到尾遍历
            rectangle(img,Point(face_iter->x,face_iter->y),Point(face_iter->x+face_iter->width,face_iter->y+face_iter->height),CV_RGB(255,0,255),2);//在原始图像上绘制矩形框，标注人脸位置
        }
    }
    imshow("faces",img);//显示标注后的人脸图像
    waitKey(2);//等待2ms，处理界面事件
    
}
int main(int argc, char** argv)
{
    ros::init(argc, argv, "cv_face_detec");

    namedWindow("faces",WINDOW_AUTOSIZE);
    std::string strLoadFile;
    char const*home=std::getenv("HOME");//获取当前用户的主目录路径
    strLoadFile=home;
    strLoadFile+="/catkin_ws";
    strLoadFile+="/src/wpr_simulation/config/haarcascade_frontalface_alt.xml";
    
    bool res=face_cascade.load(strLoadFile);//根据路径，加载级联分类器模型文件
    //加载级联分类器模型文件
    if(res==false){
        ROS_ERROR("无法加载级联分类器模型文件");
        return 0;
    }
     
    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("/kinect2/qhd/image_color_rect",1,&CamRGBCallback);
    ros::spin();
}

