#include<ros/ros.h>
#include<std_msgs/String.h>
int main(int argc,char  *argv[])
{
    ros::init(argc,argv,"yao");
    printf( "过去生。。。。\n");
    ros::NodeHandle nh;
    ros::Publisher pub=nh.advertise<std_msgs::String>("qiu_dai_fei",10);
    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        printf("yao_刷屏\n");
        std_msgs::String msg;
        msg.data="求带飞\n";
        pub.publish(msg);
        loop_rate.sleep();
    }
    
    return 0;
}