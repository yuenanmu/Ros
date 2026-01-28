#include<ros/ros.h>
#include<sensor_msgs/Imu.h>
#include<tf/tf.h>
#include<geometry_msgs/Twist.h>//发布小车速度消息

//定义全局对象
ros::Publisher vel_pub;

void IMUCallback(const sensor_msgs::Imu msg){
    //判断是否有有效的四元数数据
    if(msg.orientation_covariance[0]<0)
        return;
    //tf工具四元数转化为欧拉角
    //调用构造函数，定义四元数变量quat
    tf::Quaternion quat(
        msg.orientation.x,
        msg.orientation.y,
        msg.orientation.z,
        msg.orientation.w
    );
    //定义欧拉角变量rpy
    double roll,pitch,yaw;
    //四元数转欧拉角
    tf::Matrix3x3(quat).getRPY(roll,pitch,yaw);
    roll=roll*180/M_PI;
    pitch=pitch*180/M_PI;
    yaw=yaw*180/M_PI;
    ROS_INFO("roll:%.0f,pitch:%.0f,yaw:%.0f",roll,pitch,yaw);//宏函数

    //发布速度消息，实现速度控制
    //创建Twist对象-小车速度消息
    geometry_msgs::Twist vel_cmd;
    float target_yaw=90;
    float diff_yaw=target_yaw-yaw;
    vel_cmd.angular.z=0.01*diff_yaw;//比例控制
    vel_cmd.linear.x=0.5;
    vel_pub.publish(vel_cmd);
}
int main (int argc,char *argv[]){
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"imu_node");

    //创建节点句柄
    ros::NodeHandle nh;
    //通过句柄订阅话题
    ros::Subscriber imu_sub=nh.subscribe("/imu/data",100,&IMUCallback);//填写显式函数指针
    //通过句柄初始化发布者对象
    vel_pub=nh.advertise<geometry_msgs::Twist>("/cmd_vel",20);

    ros::spin();
    //设置循环频率
    ros::Rate loop_rate(10);
    loop_rate.sleep();
    return 0 ;
}