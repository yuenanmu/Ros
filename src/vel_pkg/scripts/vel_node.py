#!/usr/bin/env python3
#coding=utf-8

import rospy
from geometry_msgs.msg import Twist

if __name__=="__main__":
    try:
        #init
        rospy.init_node("vel_node")
        rospy.logwarn("vel_node is started")
        #pulish topic
        vel_pub=rospy.Publisher("cmd_vel",Twist,queue_size=10)
        #pulish message
        vel_msg=Twist()
        vel_msg.linear.y=0
        #set rate
        rate=rospy.Rate(5)
        #loop
        while not rospy.is_shutdown():
            #publishing
            vel_pub.publish(vel_msg)
            rate.sleep()
    except rospy.ROSInterruptException:
        rospy.logerr("节点运行中断")
