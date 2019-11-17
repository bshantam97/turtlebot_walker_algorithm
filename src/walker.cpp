/**
 * MIT License

 Copyright (c) [2019] [Shantam Bajpai]

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

/*
 * @brief: Header files for walker.cpp
 */

#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include "walker.h"

/*
 * @brief: Implementation of the laserScanCallback method of walker class
 * @param: Laser Scan sensor message
 */
void walker::laserScanCallback(const sensor_msgs::LaserScan::ConstPtr &range) {
  for (auto i = 0; i < range->ranges.size(); i++) {
    if (range->ranges[i] < 0.3) {
      obstacleRange = true;
      return;
    }
  }
  obstacleRange = false;
}

/*
 * @brief: Implementation of obstacleInRange method, returns obstacleRange
 * @type: bool
 */
bool walker::obstacleInRange() {
  return obstacleRange;
}

/*
 * @brief: Implementation of the walker constructor which will be used to help the turtlebot
 *         move forward and avoid obstacles
 * @param: nh is the node handler declared for creating publisher and subscriber
 * @param: subs is the subscriber to the topic generated by the turtlebot simulation
 * @param: pub is the publisher used to publish velocity data
 */

walker::walker(ros::NodeHandle nh, ros::Subscriber subs, ros::Publisher pub) {
  linearVel = 0.4;
  angularVel = 1.0;
  pub = nh.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1);
  subs = nh.subscribe<sensor_msgs::LaserScan>("/scan", 1000,
                                              &walker::laserScanCallback, this);
  ros::Rate loopRate(5)
  while (ros::ok()) {
    // Initialize twist
    twist.linear.x = 0;
    twist.linear.y = 0;
    twist.linear.y = 0;
    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = 0;
    if (obstacleInRange()) {
      ROS_INFO_STREAM("The obstacle is in the range, Turning...");
      twist.linear.x = 0.0
      twist.angular.z = angularVel;
    } else {
      ROS_INFO_STREAM("Moving forward, No obstacle present");
      twist.angular.z = 0.0;
      twist.linear.x = linearVel;
    }
    pub.publish(twist);
    ros::spinOnce();
    loopRate.sleep();
  }

}

