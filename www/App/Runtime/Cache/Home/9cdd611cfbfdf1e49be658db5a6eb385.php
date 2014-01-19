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
DWMS_page_config.pageid = 1;


function switchControllor() {
    if ($("#map-col").hasClass("col-md-9")) {
        //$("#switch-icon").removeClass("glyphicon-chevron-left").addClass("glyphicon-chevron-right");
        $("#map-controllor").fadeOut("normal", function() {
            $("#map-col").removeClass("col-md-9").addClass("col-md-12");
        });
    } else {
        //$("#switch-icon").removeClass("glyphicon-chevron-right").addClass("glyphicon-chevron-left");
        $("#map-col").removeClass("col-md-12").addClass("col-md-9");
        $("#map-controllor").fadeIn("normal");
    }
}
</script>

<div class="row">
    <div class="col-md-3" id="map-controllor">
        <div class="panel panel-primary sidebar_div">
            <div class="panel panel-heading" style="margin-bottom:1px">
                <h3 class="panel-title">数据选项</h3>
            </div>
            <div class="panel panel-body" style="margin-bottom:1px">
                <ul class="nav nav-pills nav-stacked sidebar_list">
                    <li>
                        <a class="dropdown-toggle" data-toggle="dropdown">数据来源<span class="badge pull-right">交通图</span></a>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="#">交通图</a>
                            </li>
                        </ul>
                    </li>
                    <li>
                        <a class="dropdown-toggle" data-toggle="dropdown">查看模式<span class="badge pull-right">图标</span></a>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="#">交通图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">卫星图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">混合</a>
                            </li>
                        </ul>
                    </li>
                    <li>
                        <a class="dropdown-toggle" data-toggle="dropdown">查看模式<span class="badge pull-right">图标</span></a>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="#">交通图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">卫星图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">混合</a>
                            </li>
                        </ul>
                    </li>
                </ul>
            </div>
        </div>

        <div class="panel panel-primary sidebar_div">
            <div class="panel panel-heading" style="margin-bottom:1px">
                <h3 class="panel-title">地图选项</h3>
            </div>
            <div class="panel panel-body" style="margin-bottom:1px">
                <ul class="nav nav-pills nav-stacked sidebar_list">
                    <li>
                        <a class="dropdown-toggle" data-toggle="dropdown">地图类型<span class="badge pull-right">交通图</span></a>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="#">交通图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">卫星图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">混合</a>
                            </li>
                        </ul>
                    </li>
                    <li>
                        <a class="dropdown-toggle" data-toggle="dropdown">查看模式<span class="badge pull-right">图标</span></a>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="#">交通图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">卫星图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">混合</a>
                            </li>
                        </ul>
                    </li>
                </ul>
            </div>
        </div>

        <div class="panel panel-primary sidebar_div">
            <div class="panel panel-heading" style="margin-bottom:1px">
                <h3 class="panel-title">地图选项</h3>
            </div>
            <div class="panel panel-body" style="margin-bottom:1px">
                <ul class="nav nav-pills nav-stacked sidebar_list">
                    <li>
                        <a class="dropdown-toggle" data-toggle="dropdown">地图类型<span class="badge pull-right">交通图</span></a>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="#">交通图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">卫星图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">混合</a>
                            </li>
                        </ul>
                    </li>
                    <li>
                        <a class="dropdown-toggle" data-toggle="dropdown">查看模式<span class="badge pull-right">图标</span></a>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="#">交通图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">卫星图</a>
                            </li>
                            <li class="divider"></li>
                            <li><a href="#">混合</a>
                            </li>
                        </ul>
                    </li>
                </ul>
            </div>
        </div>
    </div>
    <div class="col-md-9" id="map-col">

        <div class="panel panel-primary">
            <div class="panel panel-heading" style="margin-bottom:1px" onclick="switchControllor()">
                <h3 class="panel-title">地 图</h3>
            </div>
            <div id="map_container" style="width:auto">Map Loading...</div>
        </div>
    </div>
</div>
<script type="text/javascript">

$('.sidebar_list').children().click(
function() {
        $(this).addClass("active");
    }
  ).hover(
     function() {

    },
    function() {
        $(this).removeClass("active");
    });

function setmap() {
    var windowHeight = $(window).height();
    $("#map_container").css("height", "" + windowHeight - 200 + "px");
    //DWMS_page_config.pageid = 0;
    mapCtr = new BMap.Map("map_container"); // 创建Map实例

    mapCtr.centerAndZoom("上海", 15); // 初始化地图,设置中心o点坐标和地图级别。
    mapCtr.enableScrollWheelZoom();

    mapCtr.addControl(new BMap.NavigationControl());


    /*var myIcon = new BMap.Icon("http://stacky.takau.net/think/DWMS/www/Public/image/header_logo.jpg", new BMap.Size(90, 90), {
        // 指定定位位置。
        // 当标注显示在地图上时，其所指向的地理位置距离图标左上
        // 角各偏移10像素和25像素。您可以看到在本例中该位置即是
        // 图标中央下端的尖角位置。
        offset: new BMap.Size(45, 45),
        // 设置图片偏移。
        // 当您需要从一幅较大的图片中截取某部分作为标注图标时，您
        // 需要指定大图的偏移位置，此做法与css sprites技术类似。
        imageOffset: new BMap.Size(0, 0) // 设置图片偏移
    });
    // 创建标注对象并添加到地图
    var marker = new BMap.Marker(point, {
        icon: myIcon
    });
    map.addOverlay(marker);
    marker.addEventListener("click", function() {
        var opts = {
            width: 250, // 信息窗口宽度
            height: 100, // 信息窗口高度
            title: "药药且可闹" // 信息窗口标题
        }
        var infoWindow = new BMap.InfoWindow("煎饼果子来一套", opts); // 创建信息窗口对象
        map.openInfoWindow(infoWindow, marker.getPosition()); // 打开信息窗口
    });*/


    $.ajax({
        type: "get",
        url: "city?id=bmap_ajax",
        dataType: "json",
        success: function(data) {

            var pointArray = new Array();
            var idx;
            var lat, lng;

            for (idx in data) {
                lat = data[idx].bmap_latitude;
                lng = data[idx].bmap_longitude;

                pointArray.push(new BMap.Point(lng, lat));
                // console.log("lat:"+lat+" lng:"+lng);
            }
            // pointArray.push(new BMap.Point(116.399, 39.910));
            mapCtr.centerAndZoom(pointArray[0], 12);
            var polyline = new BMap.Polyline(pointArray, {
                strokeColor: "blue",
                strokeWeight: 6,
                strokeOpacity: 0.8
            });
            mapCtr.addOverlay(polyline);
            $(".anchorBL").hide()
        },
        error: function(XMLHttpRequest, textStatus, errorThrown) {
            console.log("Error:Ajax_Content_Load" + errorThrown);
        }
    });


}
setmap();
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