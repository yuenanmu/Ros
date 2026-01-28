#include<ros/ros.h>
#include<sensor_msgs/LaserScan.h>
#include<geometry_msgs/Twist.h>

//声明全局对象
ros::Publisher vel_pub;
int ncount=0;
//激光雷达回调函数
void LidarCallback(const sensor_msgs::LaserScan msg){
    float dist;
    dist=msg.ranges[180];
    ROS_INFO("前方%.2f 米处有障碍物",dist);
    if(ncount>0){
        ncount--;
        return;
    }
    //创建Twist对象
    geometry_msgs::Twist vel_cmd;
    //障碍物检测与避让
    if(dist<1.5){
        vel_cmd.angular.z=0.3;
        ncount=50;
    }
    else{
        vel_cmd.linear.x=0;
    }
    vel_pub.publish(vel_cmd);
    
}
int main(int argc, char  *argv[])
{
    //中文环境配置
    setlocale(LC_ALL,"");
    //初始化节点
    ros::init(argc,argv,"lidar_node");

    //订阅话题
    ros::NodeHandle nh;
    ros::Subscriber lidar_sub=nh.subscribe("/scan",10,&LidarCallback);//C++不支持关键字参数匹配
    vel_pub=nh.advertise<geometry_msgs::Twist>("/cmd_vel",10);
    //保持运行，随时查看
    ros::spin();

    return 0;
}
