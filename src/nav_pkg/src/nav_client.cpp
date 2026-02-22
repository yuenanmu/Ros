#include<ros/ros.h>
#include<move_base_msgs/MoveBaseAction.h>
#include<actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
    //初始化ROS节点
    ros::init(argc, argv, "nav_client");

    //tell the action client that we want to spin a thread by default
    //创建一个action client，传入move_base的action server名称，并设置自动spin线程
    MoveBaseClient ac("move_base", true);

    //等待move_base服务器action server启动
    while(!ac.waitForServer(ros::Duration(5.0))){
        ROS_INFO("waiting for the move_base action server to come up");
    }

    //创建一个move_base_msgs::MoveBaseGoal对象，并设置目标位置和姿态
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id="map";
    goal.target_pose.header.stamp= ros::Time::now();

    goal.target_pose.pose.position.x=-3.0;
    goal.target_pose.pose.position.y=2.0;
    goal.target_pose.pose.position.z=0.0;
    goal.target_pose.pose.orientation.x=0.0;
    goal.target_pose.pose.orientation.y=0.0;
    goal.target_pose.pose.orientation.z=0.0;
    goal.target_pose.pose.orientation.w=1.0;
    ROS_INFO("Sending goal");

    //发送目标位置和姿态到move_base服务器，并等待结果
    ac.sendGoal(goal);
    ac.waitForResult();
    if(ac.getState()==actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Mission complete");
    else
        ROS_INFO("Mission failed");
}
    