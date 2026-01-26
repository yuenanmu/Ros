#include<ros/ros.h>
#include<std_msgs/String.h>
void chao_callback(std_msgs::String msg){
    // printf(msg.data.c_str());
    // printf("\n");
    ROS_INFO(msg.data.c_str());
}
void yao_callback(std_msgs::String msg){
    //ROS_INFO(msg.data.c_str());
    ROS_WARN(msg.data.c_str());
}
int main(int argc,char  *argv[])
{
    setlocale(LC_ALL,"");//正常显示中文
    ros::init(argc,argv,"ma");
    printf( "小马哥\n");
    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("qiu_dai_fei",10,yao_callback);
    ros::Subscriber sub_2=nh.subscribe("kuaishangchekaiheiqun",10,chao_callback);
    //ros::Rate loop_rate(100);
    while (ros::ok())
    {
          ros::spinOnce();
        //loop_rate.sleep();
    }
    
    return 0;
}