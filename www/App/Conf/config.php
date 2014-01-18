<?php
return array(
	//'配置项'=>'配置值'
	//开启应用分组
	'APP_GROUP_LIST'	=>	'Home,Admin',
	'DEFAULT_GROUP'		=>	'Home',  //默认分组

	// 添加数据库配置信息
	'DB_TYPE'   => 'mysql',   // 数据库类型
	'DB_HOST'   => '10.50.6.70', // 服务器地址
	// 'DB_HOST'	=> 'localhost',	// 本地服务器地址
	'DB_USER'   => 'root',    // 用户名
	'DB_PWD'    => 'root1234',  // 密码
	'DB_PORT'   => 3306,    // 端口
	'DB_NAME'   => 'dwms',  // 数据库名
	'DB_PREFIX' => '',  // 数据库表前缀

	// 显示页面Trace信息
	'SHOW_PAGE_TRACE' => true,
	// 关闭布局模板配置，使用layout控制模板布局
	'LAYOUT_ON'   => false,
	// 自动侦测模板主题
	'TMPL_DETECT_THEME' => true,
	// 支持的模板主题项
	'THEME_LIST'  => '',
	// 设置默认模板主题
	"DEFAULT_THEME"  => '',
	// 使用路由功能，前提是你的URL支持PATH_INFO
	'URL_ROUTER_ON'   => true, //开启路由
	// 配置路由规则
	'URL_ROUTE_RULES'=>array(
    	// 'content'		=>	'Public/html/Content',
    	// 'fragment'		=>	'Public/html/Fragment',
 	),
	// 模板替换
	'TMPL_PARSE_STRING'  =>array(
		// 更改默认的/Public 替换规则
		//'__PUBLIC__'	=>	'/Public',
		// 增加新的CSS类库路径替换规则
		//'__CSS__'		=>	'/Public/css'
		// 增加新的HTML类库路径替换规则
		//'__HTML__'	=>	'/Public/html'
		// 增加新的JS类库路径替换规则
		//'__JS__'		=>	'/Public/js',
		// 增加新的上传路径替换规则
		//'__UPLOAD__'	=>	'/Uploads',
	),
	//点语法默认解析
	//'TMPL_VAR_IDENTIFY' => 'array',
	//模板路径，重定义控制器名称和方法模板名称的分隔符
	//'TMPL_FILE_DEPR' => '_',
	//默认过滤函数
	//'DEFAULT_FILTER' => '',
);
?>
