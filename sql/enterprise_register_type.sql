/*
Navicat MySQL Data Transfer

Source Server         : local
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : dwms

Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001

Date: 2014-01-18 16:48:45
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `enterprise_register_type`
-- ----------------------------
DROP TABLE IF EXISTS `enterprise_register_type`;
CREATE TABLE `enterprise_register_type` (
  `enterprise_register_type_id` int(11) NOT NULL AUTO_INCREMENT,
  `enterprise_register_type_code` int(11) DEFAULT NULL,
  `enterprise_register_type_name` varchar(255) DEFAULT NULL,
  `enterprise_register_type_description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`enterprise_register_type_id`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of enterprise_register_type
-- ----------------------------
INSERT INTO `enterprise_register_type` VALUES ('1', '110', '国有企业', '企业全部资产归国家所有,并按《中华人民共和国企业法人登记管理条例》规定登记注册的非公司制的经济组织。不包括有限责任公司中的国有独资公司。');
INSERT INTO `enterprise_register_type` VALUES ('2', '120', '集体企业', '企业资产归集体所有,并按《中华人民共和国企业法人登记管理条例》规定登记注册的经济组织。');
INSERT INTO `enterprise_register_type` VALUES ('3', '130', '股份合作企业', '以合作制为基础,由企业职工共同出资入股,吸收一定比例的社会资产投资组建,实行自主经营,自负盈亏,共同劳动,民主管理,按劳分配与按股分红相结合的一种集体经济组织。');
INSERT INTO `enterprise_register_type` VALUES ('4', '141', '国有联营企业', '指两个及两个以上相同或不同所有制性质的企业法人或事业单位法人,按自愿、平等、互利的原则,共同投资组成的经济组织。');
INSERT INTO `enterprise_register_type` VALUES ('5', '142', '集体联营企业', '指两个及两个以上相同或不同所有制性质的企业法人或事业单位法人,按自愿、平等、互利的原则,共同投资组成的经济组织。');
INSERT INTO `enterprise_register_type` VALUES ('6', '143', '国有与集体联企业', '指两个及两个以上相同或不同所有制性质的企业法人或事业单位法人,按自愿、平等、互利的原则,共同投资组成的经济组织。');
INSERT INTO `enterprise_register_type` VALUES ('7', '149', '其他联营企业', '指两个及两个以上相同或不同所有制性质的企业法人或事业单位法人,按自愿、平等、互利的原则,共同投资组成的经济组织。');
INSERT INTO `enterprise_register_type` VALUES ('8', '150', '有限责任公司', '根据《中华人民共和国公司登记管理条例》规定登记注册,由两个以上,五十个以下的股东共同出资,每个股东以其所认缴的出资额对公司承担有限责任,公司以其全部资产对其债务承担责任的经济组织。有限责任公司包括国有独资公司以及其他有限责任公司。');
INSERT INTO `enterprise_register_type` VALUES ('9', '151', '国有独资的有限责任公司', '国有独资公司是指国家授权的投资机构或者国家授权的部门单独投资设立的有限责任公司。');
INSERT INTO `enterprise_register_type` VALUES ('10', '159', '其他有限责任公司', '其他有限责任公司是指国有独资公司以外的其他有限责任公司。');
INSERT INTO `enterprise_register_type` VALUES ('11', '160', '股份有限公司', '根据《中华人民共和国公司登记管理条例》规定登记注册，其全部注册资本由等额股份构成并通过发行股票筹集资本，股东以其认购的股份对公司承担有限责任，公司以其全部资产对其债务承担责任的经济组织。');
INSERT INTO `enterprise_register_type` VALUES ('12', '171', '私营独资企业', '私营独资企业指按《私营企业暂行条例》的规定，由一名自然人投资经营，以雇佣劳动为基础，投资者对企业债务承担无限责任的企业。');
INSERT INTO `enterprise_register_type` VALUES ('13', '172', '私营合作企业', '私营合作企业是指按《合伙企业法》或《私营企业暂行条例》的规定，由两个以上自然人按照协议共同投资、共同经营、共负盈亏，以雇佣劳动为基础，对债务承担无限责任的企业。');
INSERT INTO `enterprise_register_type` VALUES ('14', '173', '私营有限责任公司', '私营有限责任公司是指按《公司法》、《私营企业暂行条例》的规定，由两个以上自然人投资或由单个自然人控制的有限责任公司。');
INSERT INTO `enterprise_register_type` VALUES ('15', '174', '私营股份有限公司', '私营股份有限公司是指按《公司法》规定，由五个以上自然人投资，或由单个自然人控制的股份有限公司。');
INSERT INTO `enterprise_register_type` VALUES ('16', '190', '其他内资', '上述单位之外的其他内资经济组织。');
INSERT INTO `enterprise_register_type` VALUES ('17', '210', '与港、澳、台商合资经营企业', '港澳台地区投资者与内地的企业依照《中华人民共和国中外合资经营企业法》及有关法律的规定，按合同规定的比例投资设立、分享利润和分担风险的企业。');
INSERT INTO `enterprise_register_type` VALUES ('18', '220', '与港、澳、台商合作经营企业', '港澳台地区投资者与内地企业依照《中华人民共和国中外合作经营企业法》及有关法律的规法,依照合作合同的约定进行投资或提供条件设立、分配利润和分担风险的企业。');
INSERT INTO `enterprise_register_type` VALUES ('19', '230', '港、澳、台商独资经营企业', '依照《中华人民共和国外资企业法》及有关法律的规定，在内地由港澳台地区投资者全额投资设立的企业。');
INSERT INTO `enterprise_register_type` VALUES ('20', '240', '港、澳、台商投资股份有限公司', '根据国家有关规定，经外经贸部依法批准成立，其中港、澳、台商的股本占公司注册资本的比例达25%以上的股份有限公司。凡其中港、澳、台的股本占公司注册资本的比例小于25%的，属于内资企业中股份有限公司。');
INSERT INTO `enterprise_register_type` VALUES ('21', '310', '中外合资经营企业', '外国企业或外国人与中国内地企业依照《中华人民共和国中外合资经营企业法》及有关法律的规定，按合同规定的比例投资设立、分享利润和分担风险企业。');
INSERT INTO `enterprise_register_type` VALUES ('22', '320', '中外合作经营企业', '外国企业或外国人与内地企业依照《中华人民共和国中外合作经营企业法》及有关法律的规定，依照合作合同的约定进行投资或提供条件设立、分配利润和分担风险的企业。');
INSERT INTO `enterprise_register_type` VALUES ('23', '330', '外商独资企业', '依照《中华人民共和国外资企业法》及有关法律的规定，在中国内地由外国投资者全额投资设立的企业。');
INSERT INTO `enterprise_register_type` VALUES ('24', '340', '外商投资股份有限公司', '根据国家有关规定，经外经贸部依法批准设立，其中外资的股本占公司注册资本的比例达25%以上的股份有限公司。凡其中外资股本占公司注册资本的比例小于25%的，属于内资企业中的股份有限公司。');
INSERT INTO `enterprise_register_type` VALUES ('25', '340', '外商投资股份有限公司', '根据国家有关规定，经外经贸部依法批准设立，其中外资的股本占公司注册资本的比例达25%以上的股份有限公司。凡其中外资股本占公司注册资本的比例小于25%的，属于内资企业中的股份有限公司。');
