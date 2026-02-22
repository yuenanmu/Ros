#!/usr/bin/env python3
#coding=utf-8


import rospy
import actionlib 
from move_base_msgs.msg import MoveBaseAction,MoveBaseGoal

if __name__ =="__main__":
    rospy.init_node("nav_client")
    #初始化一个action客户端，订阅move_base的action服务器
    ac=actionlib.SimpleActionClient("move_base",MoveBaseAction)

    ac.wait_for_server()#等待server服务器启动

    goal=MoveBaseGoal()#创建一个目标点对象
    goal.target_pose.header.frame_id="map"#目标点坐标系
    goal.target_pose.header.stamp=rospy.Time.now()#目标点时间戳
    goal.target_pose.pose.position.x=-3#目标点位置
    goal.target_pose.pose.position.y=2
    goal.target_pose.pose.position.z=0.0
    goal.target_pose.pose.orientation.x=0.0#目标点姿态
    goal.target_pose.pose.orientation.y=0.0
    goal.target_pose.pose.orientation.z=0.0
    goal.target_pose.pose.orientation.w=1.0

    ac.send_goal(goal)
    rospy.loginfo("正在前往目标点...")
    #小车此时开始行动，等待结果
    ac.wait_for_result()
    if ac.get_state()==actionlib.GoalStatus.SUCCEEDED:
        rospy.loginfo("成功到达目标点！")
    else:
        rospy.loginfo("未能到达目标点！")
