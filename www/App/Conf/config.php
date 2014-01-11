<?php
return array(
	//'配置项'=>'配置值'
	//开启应用分组
	'APP_GROUP_LIST' => 'Home,Admin',
	'DEFAULT_GROUP' => 'Home',		//默认分组

	// 添加数据库配置信息
	'DB_TYPE'   => 'mysql',   // 数据库类型
	'DB_HOST'   => '10.50.6.70', // 服务器地址
	'DB_USER'   => 'root',    // 用户名
	'DB_PWD'    => 'root1234',  // 密码
	'DB_PORT'   => 3306,    // 端口
	'DB_NAME'   => 'dwms',  // 数据库名
	'DB_PREFIX' => '',  // 数据库表前缀

	'SHOW_PAGE_TRACE' =>true, // 显示页面Trace信息
	//点语法默认解析
	//'TMPL_VAR_IDENTIFY' => 'array',
	//模板路径，重定义控制器名称和方法模板名称的分隔符
	//'TMPL_FILE_DEPR' => '_',
	//默认过滤函数
	//'DEFAULT_FILTER' => '',
);
?>
