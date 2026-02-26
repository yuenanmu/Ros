#include<ros/ros.h>
#include<std_msgs/String.h>
ros::Publisher wp_pub;
std::vector<std::string> waypoints={"1","2","3","4"};
int wp_index=0;


void PublishWayPoint(ros::Publisher& wp_pub){
    if(wp_index>=(int)waypoints.size()){
        ROS_INFO("All waypoints have been published.");
        return;
    }
    std_msgs::String msg;
    msg.data=waypoints[wp_index];
    wp_pub.publish(msg);
    ROS_INFO("[PublishWayPoint]: %s", msg.data.c_str());
}
void NavResultCallback(const std_msgs::String::ConstPtr& msg)//指针引用
{
    ROS_INFO("[NavResultCallback]: %s", msg->data.c_str());
    if(msg->data=="done"){
        wp_index++;
    }

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "wp_node");
    ros::NodeHandle nh;
    wp_pub = nh.advertise<std_msgs::String>("/waterplus/navi_waypoint", 10);
    ros::Subscriber wp_sub = nh.subscribe("/waterplus/navi_result", 10, &NavResultCallback);
    while (ros::ok()) {
        ros::Rate loop_rate(15); // 15 Hz
        PublishWayPoint(wp_pub);//发布第一个航点
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}