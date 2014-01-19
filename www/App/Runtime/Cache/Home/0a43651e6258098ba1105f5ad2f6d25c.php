<?php if (!defined('THINK_PATH')) exit();?><!doctype html>
<html lang="zh-cn">

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>危险废物管理信息系统</title>
    <!-- Bootstrap core CSS -->
    <link href="__PUBLIC__/css/bootstrap.min.css" rel="stylesheet">
    <link href="__PUBLIC__/css/jquery.dataTables.css" rel="stylesheet">
    <!-- My CSS -->
    <link href="__PUBLIC__/css/layout.css" rel="stylesheet">

</head>

<body style="background:url(__PUBLIC__/image/bg_4.jpg);background-position:center;background-repeat: no-repeat;background-attachment:fixed">
    <div class="container">
        <div class="panel panel-default" style="margin-top:15px">
    <div class="panel-body" style="padding-top:0px">
        <img id="logo" src="__PUBLIC__/image/header_logo.jpg" alt="logo"/>
        <div id="webpage_title">
            <div id="datetime"></div>            
            <h2 id="title">危险废物管理信息系统</h2>        
            <p id="welcome">
                <span class="glyphicon glyphicon-user"></span> 欢迎您，<?php echo ((session('username'))?(session('username')):"先生"); ?>
                <a id="password" href=#><span class="glyphicon glyphicon-edit"></span> 修改密码</a>
                <a id="logout" href="<?php echo U('Home/Login/logout');?>"><span class="glyphicon glyphicon-log-out"></span> 退出登录</a>
            </p>
          
        </div>      
         <div id="header_button_container"></div>  
    </div>
</div>

<script type="text/javascript" src="__PUBLIC__/js/jquery-2.0.3.min.js"></script>
<script type="text/javascript" src="__PUBLIC__/js/jquery.dataTables.min.js"></script>
<script type="text/javascript" src="__PUBLIC__/js/bootstrap.min.js"></script>
<script type="text/javascript" src="__PUBLIC__/js/highcharts.js"></script>
<script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=COQzjeiY7wEEt5LtYOGP0pUt">
</script>

        <div class="panel panel-default">
            <div class="panel-body" id="content-container-panel"><script type="text/javascript">
DWMS_page_config = {
    "menu": [{
        "id": 0,
        "name": "首页",
        "url": "city?id=city_index"
    }, {
        "id": 1,
        "name": "地图展示",
        "url": "city?id=GIS_sidebar"
    }, {
        "id": 2,
        "name": "产生单位",
        "url": "city?id=production_sidebar"
    }, {
        "id": 3,
        "name": "运输单位",
        "url": "city?id=transport_sidebar"
    }, {
        "id": 4,
        "name": "处置单位",
        "url": "city?id=disposal_sidebar"
    }, {
        "id": 5,
        "name": "危废转移",
        "url": "city?id=transfer_sidebar"
    }, {
        "id": 6,
        "name": "业务办理",
        "url": "city?id=business_sidebar"
    }, {
        "id": 7,
        "name": "系统管理",
        "url": "city?id=system_sidebar"
    }]
}
</script>

<script type="text/javascript">
DWMS_page_config.pageid = 7;
DWMS_sidebar_config = {
    "sidebar_groups": [{
        "sidebar_group_id": 0,
        "sidebar_group_title": "系统信息设置",
        "sidebar_group_content": [{
            "id": 0,
            "name": "路线管理",
            "url": "city?id=GIS_set_route"
        },{
            "id": 0,
            "name": "管辖权限",
            "url": "city?id=jurisdiction"
        },{
            "id": 1,
            "name": "区县代码",
            "url": "city?id=district_code"
        },{
            "id": 2,
            "name": "行业代码",
            "url": "city?id=trade_code"
        },{
            "id": 3,
            "name": "企业规模",
            "url": "city?id=enterprise_scale"
        },{
            "id": 4,
            "name": "注册类型",
            "url": "city?id=register_type"
        },{
            "id": 5,
            "name": "废物代码",
            "url": "city?id=waste_code"
        },{
            "id": 6,
            "name": "废物类别",
            "url": "city?id=waste_category"
        },{
            "id": 7,
            "name": "废物形态",
            "url": "city?id=waste_form"
        },{
            "id": 8,
            "name": "包装方式",
            "url": "city?id=package_method"
        },{
            "id": 9,
            "name": "废物处理方式",
            "url": "city?id=waste_disposal_method"
        },{
            "id": 10,
            "name": "废物去向",
            "url": "city?id=waste_direction"
        },{
            "id": 11,
            "name": "运输方式",
            "url": "city?id=transport_method"
        }

        ]
    }]
}
</script>
<div class="row">
    <div class="col-md-3">
        <div class="panel panel-primary" id="sidebar_div">
        </div>
    </div>
    <div class="col-md-9">
        <div id="content_page">
        </div>
    </div>
</div>
<script type="text/javascript">
function sidebar_generator() {
    if (typeof(DWMS_sidebar_config) == 'undefined') {
        console.log("Error:var DWMS_sidebar_config Not Found.");
        return;
    }

    var grp_idx, itm_idx;
    var newHtml = '<div class="panel-group" id="accordion">';
    var tmpid = "";
    for (grp_idx in DWMS_sidebar_config.sidebar_groups) {

        newHtml += '<div class="panel panel-primary"><div class="panel-heading"><a data-toggle="collapse" data-toggle="collapse" data-parent="#accordion" style="color:white" href="#collapse_' + grp_idx + '"><h4 class="panel-title"><span class="glyphicon glyphicon-th-list"> ' + DWMS_sidebar_config.sidebar_groups[grp_idx].sidebar_group_title + ' </h4></a></div><div id="collapse_' + grp_idx;

        if (grp_idx == 0) {
            newHtml += '" class="panel-collapse collapse in" ><div class="panel-body" ><ul class="nav nav-pills nav-stacked DWMS-sidebar-content">';
        } else {
            newHtml += '"class="panel-collapse collapse"><div class="panel-body" ><ul class="nav nav-pills nav-stacked DWMS-sidebar-content">';
        }


        for (itm_idx in DWMS_sidebar_config.sidebar_groups[grp_idx].sidebar_group_content) {
            tmpid = "sidebarItem_" + grp_idx + "_" + itm_idx;
            newHtml += '<li id="'

            + tmpid + '"><a href="#" onclick="refresh_sidebar(\'' + tmpid + '\'); fetch_page(\'' + DWMS_sidebar_config.sidebar_groups[grp_idx].sidebar_group_content[itm_idx].url + '\')"><span class="glyphicon glyphicon-book"></span> ' + DWMS_sidebar_config.sidebar_groups[grp_idx].sidebar_group_content[itm_idx].name + '</a></li>'
        }

        newHtml += "</ul></div></div></div>";

    }
    newHtml += "</div>";
    $("#sidebar_div").html(newHtml);
}

sidebar_generator();


function fetch_page(ajaxURL) {

    last_ajaxURL = ajaxURL;
    $("#content_page").html('<center style="margin-top:100px"><h4>努力地加载中...</h4><div class="progress progress-striped active" style="width: 50%"><div class="progress-bar"  role="progressbar" aria-valuenow="100" aria-valuemin="0" aria-valuemax="100" style="width: 100%"></div></div></center>');


    $.ajax({
        type: "get",
        url: ajaxURL,
        dataType: "json",
        success: function(data) {
            $("#content_page").hide();
            $("#content_page").html(data);
            $("#content_page").fadeIn();
        },
        error: function(XMLHttpRequest, textStatus, errorThrown) {
            console.log("Error:Ajax_Content_Load" + errorThrown);
            console.log("XMLHttpRequest.status:" + XMLHttpRequest.status);
            console.log("XMLHttpRequest.readyState:" + XMLHttpRequest.readyState);
            console.log("textStatus:" + textStatus);
        }
    });
}

function refresh_sidebar(itemID) {
    $(".DWMS-sidebar-content").children().removeClass('active');
    $("#" + itemID).addClass('active');
}




last_ajaxURL = 0;

function refresh_page() {
    if (last_ajaxURL == 0) {
        console.log("Error:last_ajaxURL undefined.");
        return -1;
    }
    fetch_page(last_ajaxURL);
    return 0;
}
$("#sidebarItem_0_0 a").click();
</script>
</div>
        </div>
        <!-- My JavaScript-->
<script type="text/javascript" src="__PUBLIC__/js/datetime.js"></script>

<div class="panel panel-info">
    <div class="panel-heading"></div>
    <div class="panel panel-body" id="footer-content">
    	<b>危险废物管理信息系统 DWMS</b> Powered By ThinkPHP
        <br/>Copyright © 2014-2015<br/> <span class="glyphicon glyphicon-send"></span> SJTU OMNILab
    </div>
</div>

</div>

        <script type="text/javascript">
function menu_generator() {
    if (typeof(DWMS_page_config) == 'undefined') {
        console.log("Error:var DWMS_page_config Not Found.");
        return;
    }
    var idx;
    var newHtml = '<ul class="nav nav-pills">';
    for (idx in DWMS_page_config.menu) {

        if (DWMS_page_config.menu[idx].id == DWMS_page_config.pageid) {
            newHtml += '<li class="active"><a><span class="glyphicon glyphicon-folder-open"></span> ' + DWMS_page_config.menu[idx].name + '</a></li>';
        } else {
            newHtml += '<li><a href="' + DWMS_page_config.menu[idx].url + '"><span class="glyphicon glyphicon-folder-close"></span> ' + DWMS_page_config.menu[idx].name + '</a></li>';
        }
    }
    newHtml += "</ul>";
    $("#header_button_container").html(newHtml);
}
menu_generator();
</script>

    </div>
</body>

</html>