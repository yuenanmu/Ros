//使用新定义的消息类型
#include<ros/ros.h>
#include<std_msgs/String.h>
#include<qq_msgs/Carry.h>

int main(int argc,char  *argv[])
{
    ros::init(argc,argv,"chao");
    printf( "My Gun Go And back\n");
    ros::NodeHandle nh;
    ros::Publisher pub=nh.advertise<qq_msgs::Carry>("kuaishangchekaiheiqun",10);
    ros::Rate loop_rate(10);//频率控制对象
    while (ros::ok())
    {
        printf("chao_刷屏\n");
        qq_msgs::Carry msg;
        msg.grade="王者";
        msg.star=100;
        msg.data="马超带你飞\n";
        pub.publish(msg);
        loop_rate.sleep();//调用对象
    }
    
    return 0;
}