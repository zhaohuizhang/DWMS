/*
Navicat MySQL Data Transfer

Source Server         : 10.50.6.70_3306
Source Server Version : 50534
Source Host           : 10.50.6.70:3306
Source Database       : dwms

Target Server Type    : MYSQL
Target Server Version : 50534
File Encoding         : 65001

Date: 2014-01-08 23:01:15
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `device`
-- ----------------------------
DROP TABLE IF EXISTS `device`;
CREATE TABLE `device` (
  `device_id` int(11) NOT NULL,
  `device_name` varchar(255) NOT NULL,
  `device_tyoe` varchar(255) NOT NULL,
  `device_serial_num` varchar(255) NOT NULL,
  `ownership_type` tinyint(4) NOT NULL,
  `ownership_id` int(11) NOT NULL,
  `device_status` tinyint(4) NOT NULL,
  PRIMARY KEY (`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of device
-- ----------------------------
INSERT INTO `device` VALUES ('0', 'DTU', 'GPS', '308033501795', '6', '6', '0');
INSERT INTO `device` VALUES ('1', 'DTU', 'GPS', '308001061692', '6', '6', '0');

-- ----------------------------
-- Table structure for `gps_308001061692`
-- ----------------------------
DROP TABLE IF EXISTS `gps_308001061692`;
CREATE TABLE `gps_308001061692` (
  `id` int(11) NOT NULL,
  `datetime` datetime NOT NULL,
  `longitude` double NOT NULL,
  `latitude` double NOT NULL,
  `height` double NOT NULL,
  `speed` double NOT NULL,
  `status` tinyint(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of gps_308001061692
-- ----------------------------

-- ----------------------------
-- Table structure for `gps_308033501795`
-- ----------------------------
DROP TABLE IF EXISTS `gps_308033501795`;
CREATE TABLE `gps_308033501795` (
  `id` int(11) NOT NULL,
  `datetime` datetime NOT NULL,
  `longitude` double NOT NULL,
  `latitude` double NOT NULL,
  `height` double NOT NULL,
  `speed` double NOT NULL,
  `status` tinyint(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of gps_308033501795
-- ----------------------------

-- ----------------------------
-- Table structure for `manifest`
-- ----------------------------
DROP TABLE IF EXISTS `manifest`;
CREATE TABLE `manifest` (
  `manifest_id` int(11) NOT NULL,
  `manifest_num` varchar(255) NOT NULL,
  `production_unit_id` int(11) NOT NULL,
  `transport_unit_id` int(11) NOT NULL,
  `receiving_unit_id` int(11) NOT NULL,
  `waste_id` int(11) NOT NULL,
  `waste_weight` varchar(255) NOT NULL,
  `waste_package` varchar(255) NOT NULL,
  `waste_transport_goal` varchar(255) NOT NULL,
  `emergency_measure` varchar(255) NOT NULL,
  `waste_shipper` varchar(255) NOT NULL,
  `waste_destination` varchar(255) NOT NULL,
  `waste_transport_time` date NOT NULL,
  `carrier_1_name` varchar(255) DEFAULT NULL,
  `carrier_1_num` varchar(255) DEFAULT NULL,
  `transport_date_1` date DEFAULT NULL,
  `vehicle_id_1` varchar(255) DEFAULT NULL,
  `transport_license_num_1` varchar(255) DEFAULT NULL,
  `transport_start_point_1` varchar(255) DEFAULT NULL,
  `transport_pass_by_1` varchar(255) DEFAULT NULL,
  `transport_destination_1` varchar(255) DEFAULT NULL,
  `carrier_2_name` varchar(255) DEFAULT NULL,
  `carrier_2_num` varchar(255) DEFAULT NULL,
  `transport_date_2` date DEFAULT NULL,
  `vehicle_id_2` varchar(255) DEFAULT NULL,
  `transport_license_num_2` varchar(255) DEFAULT NULL,
  `transport_start_point_2` varchar(255) DEFAULT NULL,
  `transport_pass_by_2` varchar(255) DEFAULT NULL,
  `transport_destination_2` varchar(255) DEFAULT NULL,
  `receiving_unit_license_num` varchar(255) DEFAULT NULL,
  `receiver_name` varchar(255) DEFAULT NULL,
  `receiver_num` varchar(255) DEFAULT NULL,
  `receive_date` date DEFAULT NULL,
  `waste_disposal_method` varchar(255) DEFAULT NULL,
  `receiving_unit_principal_name` varchar(255) DEFAULT NULL,
  `receiving_unit_audit_date` date DEFAULT NULL,
  PRIMARY KEY (`manifest_id`),
  KEY `fk_production_unit_id` (`production_unit_id`),
  KEY `fk_transport_unit_id` (`transport_unit_id`),
  KEY `fk_receiving_unit` (`receiving_unit_id`),
  KEY `fk_waste_id` (`waste_id`),
  CONSTRAINT `fk_waste_id` FOREIGN KEY (`waste_id`) REFERENCES `waste` (`waste_id`),
  CONSTRAINT `fk_production_unit_id` FOREIGN KEY (`production_unit_id`) REFERENCES `production_unit` (`production_unit_id`),
  CONSTRAINT `fk_receiving_unit` FOREIGN KEY (`receiving_unit_id`) REFERENCES `receiving_unit` (`receiving_unit_id`),
  CONSTRAINT `fk_transport_unit_id` FOREIGN KEY (`transport_unit_id`) REFERENCES `transport_unit` (`transport_unit_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of manifest
-- ----------------------------

-- ----------------------------
-- Table structure for `production_unit`
-- ----------------------------
DROP TABLE IF EXISTS `production_unit`;
CREATE TABLE `production_unit` (
  `production_unit_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `production_unit_name` varchar(255) NOT NULL,
  `production_unit_username` varchar(255) NOT NULL,
  `production_unit_code` varchar(255) NOT NULL,
  `production_unit_phone` varchar(255) NOT NULL,
  `production_unit_address` varchar(255) NOT NULL,
  `production_unit_postcode` varchar(255) NOT NULL,
  `waste_location` varchar(255) NOT NULL,
  `waste_location_county` varchar(255) NOT NULL,
  `waste_location_county_code` varchar(255) NOT NULL,
  `production_unit_jurisdiction` varchar(255) NOT NULL,
  `production_unit_trade` varchar(255) DEFAULT NULL,
  `production_unit_street` varchar(255) DEFAULT NULL,
  `production_unit_registration_type` varchar(255) DEFAULT NULL,
  `production_unit_enterprise_scale` varchar(255) DEFAULT NULL,
  `production_unit_contacts_name` varchar(255) NOT NULL,
  `production_unit_contacts_phone` varchar(255) NOT NULL,
  `production_unit_legal_person_code` varchar(255) NOT NULL,
  `production_unit_legal_person_name` varchar(255) NOT NULL,
  `production_unit_legal_person_phone` varchar(255) NOT NULL,
  `production_unit_fax` varchar(255) DEFAULT NULL,
  `production_unit_email` varchar(255) DEFAULT NULL,
  `production_unit_longitude` double DEFAULT NULL,
  `production_unit_latitude` double DEFAULT NULL,
  PRIMARY KEY (`production_unit_id`),
  KEY `fk_user_id_production` (`user_id`),
  CONSTRAINT `fk_user_id_production` FOREIGN KEY (`user_id`) REFERENCES `user` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of production_unit
-- ----------------------------
INSERT INTO `production_unit` VALUES ('0', '5', '生产单位', 'production', '0000', '13900000000', '安徽省安庆市', '246000', '安徽省安庆市', '安庆市', '246000', '', '化工', '安庆市街道', 'admin注册', 'admin企业', '张斌', '13800000000', '0000', '张斌', '18800000000', '54740000', 'admin@admin', '0', '0');

-- ----------------------------
-- Table structure for `production_unit_0`
-- ----------------------------
DROP TABLE IF EXISTS `production_unit_0`;
CREATE TABLE `production_unit_0` (
  `id` int(11) NOT NULL,
  `rfid_id` varchar(255) NOT NULL,
  `waste_id` int(11) NOT NULL,
  `add_weight` double NOT NULL,
  `add_date_time` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_waste_id_production` (`waste_id`),
  CONSTRAINT `fk_waste_id_production` FOREIGN KEY (`waste_id`) REFERENCES `waste` (`waste_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of production_unit_0
-- ----------------------------

-- ----------------------------
-- Table structure for `receiving_unit`
-- ----------------------------
DROP TABLE IF EXISTS `receiving_unit`;
CREATE TABLE `receiving_unit` (
  `receiving_unit_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `receiving_unit_name` varchar(255) NOT NULL,
  `receiving_unit_username` varchar(255) NOT NULL,
  `receiving_unit_code` varchar(255) NOT NULL,
  `receiving_unit_phone` varchar(255) NOT NULL,
  `receiving_unit_address` varchar(255) NOT NULL,
  `receiving_unit_postcode` varchar(255) NOT NULL,
  `receiving_unit_county` varchar(255) NOT NULL,
  `receiving_unit_county_code` varchar(255) NOT NULL,
  `receiving_unit_jurisdiction` varchar(255) NOT NULL,
  `receiving_unit_trade` varchar(255) DEFAULT NULL,
  `receiving_unit_street` varchar(255) DEFAULT NULL,
  `receiving_unit_registration_type` varchar(255) DEFAULT NULL,
  `receiving_unit_enterprise_scale` varchar(255) DEFAULT NULL,
  `receiving_unit_license_num` varchar(255) NOT NULL,
  `receiving_unit_reference_num` varchar(255) NOT NULL,
  `license_issuing_authority` varchar(255) NOT NULL,
  `license_issuing_date` date NOT NULL,
  `license_expiry_date` date NOT NULL,
  `receiving_unit_contacts_name` varchar(255) NOT NULL,
  `receiving_unit_contacts_phone` varchar(255) NOT NULL,
  `receiving_unit_legal_person_code` varchar(255) NOT NULL,
  `receiving_unit_legal_person_name` varchar(255) NOT NULL,
  `receiving_unit_legal_person_phone` varchar(255) NOT NULL,
  `receiving_unit_fax` varchar(255) DEFAULT NULL,
  `receiving_unit_email` varchar(255) DEFAULT NULL,
  `receiving_unit_longitude` double DEFAULT NULL,
  `receiving_unit_latitude` double DEFAULT NULL,
  PRIMARY KEY (`receiving_unit_id`),
  KEY `fk_user_id_receiving` (`user_id`),
  CONSTRAINT `fk_user_id_receiving` FOREIGN KEY (`user_id`) REFERENCES `user` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of receiving_unit
-- ----------------------------
INSERT INTO `receiving_unit` VALUES ('0', '7', '接受单位', 'reception', '0000', '13900000000', '安徽省安庆市', '246000', '安庆市', '246000', '安庆市', '化工', '安庆市街道', 'admin企业', 'admin规模', '0000', '0000', 'omnilab', '2014-01-08', '2014-01-08', '张斌', '13800000000', '0000', '张斌', '18800000000', '54740000', 'admin@admin', '0', '0');

-- ----------------------------
-- Table structure for `receiving_unit_0`
-- ----------------------------
DROP TABLE IF EXISTS `receiving_unit_0`;
CREATE TABLE `receiving_unit_0` (
  `id` int(11) NOT NULL,
  `rfid_id` varchar(255) NOT NULL,
  `waste_id` int(11) NOT NULL,
  `total_weight` double NOT NULL,
  `receive_date_time` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_waste_id_receiving` (`waste_id`),
  CONSTRAINT `fk_waste_id_receiving` FOREIGN KEY (`waste_id`) REFERENCES `waste` (`waste_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of receiving_unit_0
-- ----------------------------

-- ----------------------------
-- Table structure for `transport_unit`
-- ----------------------------
DROP TABLE IF EXISTS `transport_unit`;
CREATE TABLE `transport_unit` (
  `transport_unit_id` int(11) NOT NULL DEFAULT '0',
  `user_id` int(11) NOT NULL,
  `transport_unit_name` varchar(255) NOT NULL,
  `transport_unit_username` varchar(255) NOT NULL,
  `transport_unit_code` varchar(255) NOT NULL,
  `transport_unit_phone` varchar(255) NOT NULL,
  `transport_unit_address` varchar(255) NOT NULL,
  `transport_unit_postcode` varchar(255) NOT NULL,
  `transport_unit_county` varchar(255) NOT NULL,
  `transport_unit_county_code` varchar(255) NOT NULL,
  `transport_unit_jurisdiction` varchar(255) NOT NULL,
  `transport_unit_trade` varchar(255) DEFAULT NULL,
  `transport_unit_street` varchar(255) DEFAULT NULL,
  `transport_unit_registration_type` varchar(255) DEFAULT NULL,
  `transport_unit_enterprise_scale` varchar(255) DEFAULT NULL,
  `transport_unit_license_num` varchar(255) NOT NULL,
  `transport_unit_reference_num` varchar(255) NOT NULL,
  `license_issuing_authority` varchar(255) NOT NULL,
  `license_issuing_date` date NOT NULL,
  `license_expiry_date` date NOT NULL,
  `transport_unit_contacts_name` varchar(255) NOT NULL,
  `transport_unit_contacts_phone` varchar(255) NOT NULL,
  `transport_unit_legal_person_code` varchar(255) NOT NULL,
  `transport_unit_legal_person_name` varchar(255) NOT NULL,
  `transport_unit_legal_person_phone` varchar(255) NOT NULL,
  `transport_unit_fax` varchar(255) DEFAULT NULL,
  `transport_unit_email` varchar(255) DEFAULT NULL,
  `transport_unit_longitude` double DEFAULT NULL,
  `transport_unit_latitude` double DEFAULT NULL,
  PRIMARY KEY (`transport_unit_id`),
  KEY `fk_user_id_transport` (`user_id`),
  CONSTRAINT `fk_user_id_transport` FOREIGN KEY (`user_id`) REFERENCES `user` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of transport_unit
-- ----------------------------

-- ----------------------------
-- Table structure for `user`
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `user_id` int(11) NOT NULL DEFAULT '0',
  `user_type` tinyint(4) NOT NULL,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `email` varchar(255) DEFAULT NULL,
  `phone_num` varchar(255) DEFAULT NULL,
  `add_time` datetime NOT NULL,
  `change_time` datetime NOT NULL,
  `last_login_time` datetime NOT NULL,
  `current_login_time` datetime NOT NULL,
  `last_login_ip` varchar(255) NOT NULL,
  `current_login_ip` varchar(255) NOT NULL,
  `is_verify` tinyint(1) NOT NULL,
  `lock` tinyint(1) NOT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES ('0', '0', 'admin', '21232f297a57a5a743894a0e4a801fc3', 'admin@admin', '', '2014-01-05 13:54:12', '2014-01-05 13:54:42', '2014-01-05 15:30:17', '2014-01-08 13:43:39', '127.0.0.1', '127.0.0.1', '1', '0');
INSERT INTO `user` VALUES ('1', '1', 'country', 'e909c2d7067ea37437cf97fe11d91bd0', 'country@country', '', '2014-01-07 20:57:18', '2014-01-07 20:57:18', '2014-01-07 20:57:18', '2014-01-08 22:00:31', '127.0.0.1', '202.120.32.17', '1', '0');
INSERT INTO `user` VALUES ('2', '2', 'province', '53aad639aca4b5c010927cf610c3ff9c', 'province@province', '', '2014-01-07 20:58:51', '2014-01-07 20:58:51', '2014-01-07 20:58:51', '2014-01-07 20:58:51', '127.0.0.1', '127.0.0.1', '1', '0');
INSERT INTO `user` VALUES ('3', '3', 'city', '4ed5d2eaed1a1fadcc41ad1d58ed603e', 'city@city', '', '2014-01-05 13:54:21', '2014-01-05 13:54:46', '2014-01-07 21:29:25', '2014-01-08 21:58:45', '127.0.0.1', '219.228.106.234', '1', '0');
INSERT INTO `user` VALUES ('4', '4', 'district', '6b77ef4b602800a89d88e6e3f93a322c', 'district@district', '', '2014-01-05 13:54:25', '2014-01-05 13:54:49', '2014-01-05 15:09:50', '2014-01-07 13:44:25', '127.0.0.1', '127.0.0.1', '1', '0');
INSERT INTO `user` VALUES ('5', '5', 'production', 'fd89784e59c72499525556f80289b2c7', 'production@production', '', '2014-01-05 13:54:29', '2014-01-05 13:54:53', '2014-01-05 15:11:24', '2014-01-07 13:44:28', '127.0.0.1', '127.0.0.1', '1', '0');
INSERT INTO `user` VALUES ('6', '6', 'transport', '7b334b7260361141659fa9862e803476', 'transport@transport', '', '2014-01-05 13:54:33', '2014-01-05 13:54:57', '2014-01-05 15:11:56', '2014-01-07 13:44:32', '127.0.0.1', '127.0.0.1', '1', '0');
INSERT INTO `user` VALUES ('7', '7', 'reception', '1da95b279fc0d21024cece2c68a4c200', 'reception@reception', '', '2014-01-05 13:54:38', '2014-01-05 13:55:01', '2014-01-05 15:12:28', '2014-01-07 13:44:35', '127.0.0.1', '127.0.0.1', '1', '0');

-- ----------------------------
-- Table structure for `vehicle`
-- ----------------------------
DROP TABLE IF EXISTS `vehicle`;
CREATE TABLE `vehicle` (
  `vehicle_id` int(11) NOT NULL,
  `vehicle_type` varchar(255) NOT NULL,
  `vehicle_num` varchar(255) NOT NULL,
  `ownership_id` int(11) NOT NULL,
  `gps_device_id` int(11) NOT NULL,
  PRIMARY KEY (`vehicle_id`),
  KEY `fk_ownership_id` (`ownership_id`),
  KEY `fk_gps_device_id` (`gps_device_id`),
  CONSTRAINT `fk_gps_device_id` FOREIGN KEY (`gps_device_id`) REFERENCES `device` (`device_id`),
  CONSTRAINT `fk_ownership_id` FOREIGN KEY (`ownership_id`) REFERENCES `transport_unit` (`transport_unit_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of vehicle
-- ----------------------------

-- ----------------------------
-- Table structure for `waste`
-- ----------------------------
DROP TABLE IF EXISTS `waste`;
CREATE TABLE `waste` (
  `waste_id` int(11) NOT NULL,
  `waste_name` varchar(255) NOT NULL,
  `waste_ingredient` varchar(255) NOT NULL,
  `waste_code` varchar(255) NOT NULL,
  `waste_form` varchar(255) NOT NULL,
  `waste_property` varchar(255) NOT NULL,
  PRIMARY KEY (`waste_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of waste
-- ----------------------------
