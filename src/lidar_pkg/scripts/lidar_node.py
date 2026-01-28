#!/usr/bin/env python3
#coding='utf-8'

#导入模块
import rospy
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist


#激光雷达回调函数
def LidarCallback(msgs):
    global ncount,vel_pub
    ncount=0
    dist=msgs.ranges[180]
    rospy.loginfo("前方测距：%.2f m",dist)
    if ncount>0:
        ncount-=1
        return
    #创建Twist对象
    vel_cmd=Twist()
    #障碍物检测与避让
    if dist<1.5:
        vel_cmd.angular.z=0.3
        ncount=50
    else:
        vel_cmd.linear.x=0
    vel_pub.publish(vel_cmd)

if __name__ =='__main__':
    #初始化节点
    rospy.init_node("lidar_node")

    #create Subscriber
    rospy.Subscriber("/scan",LaserScan,LidarCallback,queue_size=10)
    
    #create Publisher
    vel_pub=rospy.Publisher("/cmd_vel",Twist,queue_size=10)
    #spin
    rospy.spin()
