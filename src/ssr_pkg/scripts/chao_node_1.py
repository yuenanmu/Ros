#!/usr/bin/env python3 
#coding=utf-8
 
import rospy
from std_msgs.msg import String
from qq_msgs.msg import Carry
if __name__=="__main__":
    rospy.init_node("chao_node")
    rospy.logwarn("chao_node is started ")

    pub=rospy.Publisher("chao_node_toptic",Carry,queue_size=10)

    rate=rospy.Rate(1)

    while not rospy.is_shutdown():
        rospy.loginfo("chao_node is running")
        msg=Carry()
        msg.grade="王者"
        msg.star=50
        msg.data="This is chao_node publishing data\n"
        pub.publish(msg)
        rate.sleep()