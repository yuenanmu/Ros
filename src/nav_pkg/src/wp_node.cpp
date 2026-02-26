#include<ros/ros.h>
#include<std_msgs/String.h>
void NavResultCallback(const std_msgs::String::ConstPtr& msg)//指针引用
{
    ROS_INFO("[Received]: %s", msg->data.c_str());
    
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "wp_node");
    ros::NodeHandle nh;
    ros::Publisher wp_pub = nh.advertise<std_msgs::String>("/waterplus/navi_waypoint", 10);
    ros::Subscriber wp_sub = nh.subscribe("/waterplus/navi_result", 10, &NavResultCallback);
        
    ros::Rate loop_rate(1); // 1 Hz

    while (ros::ok())
    {
        std_msgs::String msg;
        msg.data = "1";
        wp_pub.publish(msg);
        ROS_INFO("Published: %s", msg.data.c_str());
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}