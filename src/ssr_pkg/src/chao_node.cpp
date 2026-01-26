// #include<ros/ros.h>
// int main(int argc,char  *argv[])
// {
//     ros::init(argc,argv,"chao");
//     printf( "hello\n");
//     while (ros::ok())
//     {
//         printf("hafjiao");
//     }
    
//     return 0;
// }
#include<ros/ros.h>
#include<std_msgs/String.h>
int main(int argc,char  *argv[])
{
    ros::init(argc,argv,"chao");
    printf( "My Gun Go And back\n");
    ros::NodeHandle nh;
    ros::Publisher pub=nh.advertise<std_msgs::String>("kuaishangchekaiheiqun",10);
    ros::Rate loop_rate(10);//频率控制对象
    while (ros::ok())
    {
        printf("chao_刷屏\n");
        std_msgs::String msg;
        msg.data="马超带你飞\n";
        pub.publish(msg);
        loop_rate.sleep();//调用对象
    }
    
    return 0;
}