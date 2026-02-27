#include<ros/ros.h>
#include<cv_bridge/cv_bridge.h>//格式转换器cv_bridge提供了ROS图像消息和OpenCV图像之间的转换功能
#include<sensor_msgs/image_encodings.h>//图像格式编码头文件
#include<opencv2/imgproc/imgproc.hpp>//opencv图像处理函数头文件
#include<opencv2/highgui/highgui.hpp>//opencv界面显示头文件
#include<vector>

using namespace cv;
using namespace std;

static int iLowH=10;
static int iHighH=40;
static int iLowS=90;
static int iHighS=255;
static int iLowV=1;
static int iHighV=255;

void CamRGBCallback(const sensor_msgs::ImageConstPtr& msg){

    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr=cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
    }
    catch(cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception:%s",e.what());
        return;
    }

    Mat imgBGR8,imgHSV;
    Mat imgThresholded;
    imgBGR8=cv_ptr->image;
    //将原始图像从BGR色彩空间转换到HSV色彩空间
    cvtColor(imgBGR8,imgHSV,COLOR_BGR2HSV);

    //在HSV空间做直方图的均衡化
    vector<Mat> hsvSplit;
    split(imgHSV,hsvSplit);//将HSV图像分割成3个通道
    equalizeHist(hsvSplit[2],hsvSplit[2]);//对HSV空间的V通道进行直方图均衡化
    merge(hsvSplit,imgHSV);//将处理后的V通道与原来的H、S通道合并成新的HSV图像

    //根据图像的H,S,V值进行图像分割，得到二值图像
    inRange(imgHSV,Scalar(iLowH,iLowS,iLowV),Scalar(iHighH,iHighS,iHighV),imgThresholded);//结果保存到imgThresholded对象中

    //对图像进行开操作和闭操作
    Mat element=getStructuringElement(MORPH_RECT,Size(5,5));
    morphologyEx(imgThresholded,imgThresholded,MORPH_OPEN,element);//开操作（腐蚀，去噪点）
    morphologyEx(imgThresholded,imgThresholded,MORPH_CLOSE,element);//闭操作（膨胀，连通“域”）

    //遍历二值化的图像数据
    int nTargetX=0;
    int nTargetY=0;
    int nPixelCount=0;
    int nImgWidth=imgThresholded.cols;
    int nImgHeight=imgThresholded.rows;
    int nImgChannels=imgThresholded.channels();//通道是每个像素点占用字节数，灰度图像是1，彩色图像是3

    for(int y=0;y<nImgHeight;y++){
        for(int x=0;x<nImgWidth;x++){
            if(imgThresholded.data[y*nImgWidth+x]==255){//白色像素点
                nTargetX+=x;
                nTargetY+=y;
                nPixelCount++;
            }
        }
    }
    if(nPixelCount>0){
        nTargetX/=nPixelCount;
        nTargetY/=nPixelCount;
        cout<<"目标中心坐标：("<<nTargetX<<","<<nTargetY<<")"<<endl;
        Point line_start(nTargetX-10,nTargetY);
        Point line_end(nTargetX+10,nTargetY);
        line(imgBGR8,line_start,line_end,Scalar(255,0,0),5);
        line_start=Point(nTargetX,nTargetY-10);
        line_end=Point(nTargetX,nTargetY+10);
        line(imgBGR8,line_start,line_end,Scalar(255,0,0),5);
    }else{
        cout<<"未检测到目标"<<endl;
    }
    //显示处理结果
    imshow("view",cv_ptr->image);
    imshow("HSV",imgHSV);
    imshow("Result",imgThresholded);

     waitKey(5);
}
int main(int argc, char** argv)
{
    ros::init(argc, argv, "cv_hsv_node");

    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("/kinect2/qhd/image_color_rect",1,&CamRGBCallback);

    //动态调节HSV颜色分割参数
    namedWindow("Control Threshold",WINDOW_AUTOSIZE);
    createTrackbar("LowH","Control Threshold",&iLowH,179);//Hue(色调)范围是0-179
    createTrackbar("HighH","Control Threshold",&iHighH,179);
    createTrackbar("LowS","Control Threshold",&iLowS,255);//Saturation(饱和度)范围是0-255
    createTrackbar("HighS","Control Threshold",&iHighS,255);
    createTrackbar("LowV","Control Threshold",&iLowV,255);//Value(亮度)范围是0-255
    createTrackbar("HighV","Control Threshold",&iHighV,255);

    namedWindow("view");//通过回调函数，在不同的窗口显示原始图像数据
    namedWindow("HSV");
    namedWindow("Result");

    ros::Rate loop_rate(10);
    ros::spin();
}
