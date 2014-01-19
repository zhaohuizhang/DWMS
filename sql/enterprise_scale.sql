/*
Navicat MySQL Data Transfer

Source Server         : local
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : dwms

Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001

Date: 2014-01-18 16:48:55
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `enterprise_scale`
-- ----------------------------
DROP TABLE IF EXISTS `enterprise_scale`;
CREATE TABLE `enterprise_scale` (
  `enterprise_scale_id` int(11) NOT NULL AUTO_INCREMENT,
  `enterprise_scale_code` int(11) DEFAULT NULL,
  `enterprise_scale_name` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`enterprise_scale_id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of enterprise_scale
-- ----------------------------
INSERT INTO `enterprise_scale` VALUES ('1', '1', '特大型');
INSERT INTO `enterprise_scale` VALUES ('2', '2', '大型一档');
INSERT INTO `enterprise_scale` VALUES ('3', '3', '大型二档');
INSERT INTO `enterprise_scale` VALUES ('4', '4', '中型一档');
INSERT INTO `enterprise_scale` VALUES ('5', '5', '中型二档');
INSERT INTO `enterprise_scale` VALUES ('6', '6', '小型');
INSERT INTO `enterprise_scale` VALUES ('7', '7', '其他');
