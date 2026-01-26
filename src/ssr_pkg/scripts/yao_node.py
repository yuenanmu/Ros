#!/usr/bin/env python3 
#coding=utf-8
 
import rospy
from std_msgs.msg import String

if __name__=="__main__":
    rospy.init_node("yao_node")
    rospy.logwarn("yao_node is started ")

    pub=rospy.Publisher("yao_node_toptic",String,queue_size=10)

    rate=rospy.Rate(1)

    while not rospy.is_shutdown():
        rospy.loginfo("yao_node is running")
        
        msg=String()
        msg.data="This is yao_node publishing data"
        
        pub.publish(msg)
        rate.sleep()