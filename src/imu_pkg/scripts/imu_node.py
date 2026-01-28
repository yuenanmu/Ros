#!/usr/bin/env python3
#coding=utf-8

import rospy
from sensor_msgs.msg import Imu
from tf.transformations import euler_from_quaternion
import math
from geometry_msgs.msg import Twist

def imu_callback(msg):
    global vel_pub
    #判断消息有效性
    if msg.orientation_covariance[0]<0:
        rospy.logerr("imu no orientation info")
        return
    #四元数
    qx=msg.orientation.x
    qy=msg.orientation.y
    qz=msg.orientation.z
    qw=msg.orientation.w
    quaternion=[qx,qy,qz,qw]
    #转换为欧拉角

    (roll,pitch,yaw)=euler_from_quaternion(quaternion)
    #转换为角度制
    roll=roll*180/math.pi
    pitch=pitch*180/math.pi
    yaw=yaw*180/math.pi
    rospy.loginfo("roll: %.2f,pitch: %.2f,raw: %.2f,",roll,pitch,yaw)
    #发布速度消息，实现速度控制
    #创建Twist对象-小车速度消息
    vel_cmd=Twist()
    target_yaw=90
    diff_angle=target_yaw-yaw
    vel_cmd.angular.z=diff_angle*0.01
    vel_cmd.linear.x=0.5
    vel_pub.publish(vel_cmd)

if __name__=='__main__':
    rospy.init_node("imu_node")

    imu_sub=rospy.Subscriber("/imu/data",Imu,imu_callback,queue_size=100)

    vel_pub=rospy.Publisher("/cmd_vel",Twist,queue_size=20)

    rospy.spin()