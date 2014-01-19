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
DWMS_page_config.pageid = 0;
</script>
<div class="row">
    <div class="col-md-6">
        <div class="panel panel-primary">
    <div class="panel-heading">
        <h3 class="panel-title">（生产、运输、接受）企业统计图</h3>
    </div>
    <div class="panel-body">
        <div id="statistics_graph_enterprise_container"></div>
    </div>
</div>


<script type="text/javascript">
$('#statistics_graph_enterprise_container').highcharts({
    chart: {
        plotBackgroundColor: null,
        plotBorderWidth: null,
        plotShadow: false
    },
    title: {
        text: '企业统计图'
    },
    tooltip: {
        pointFormat: '{series.name}: <b>{point.percentage:.1f}%</b>'
    },
    plotOptions: {
        pie: {
            allowPointSelect: true,
            cursor: 'pointer',
            dataLabels: {
                enabled: true,
                color: '#000000',
                connectorColor: '#000000',
                format: '<b>{point.name}</b>: {point.percentage:.1f} %'
            }
        }
    },
    series: [{
        type: 'pie',
        name: 'Browser share',
        data: [
            ['Firefox', 45.0],
            ['IE', 26.8], {
                name: 'Chrome',
                y: 12.8,
                sliced: true,
                selected: true
            },
            ['Safari', 8.5],
            ['Opera', 6.2],
            ['Others', 0.7]
        ]
    }]

});
</script>

    </div>
    <div class="col-md-6">
        <div class="panel panel-primary">
    <div class="panel-heading">
        <h3 class="panel-title">（生产、运输、接受）企业统计表</h3>
    </div>
    <div class="panel-body">
        <table class="table" id="statistics_table_enterprise_container">
        </table>
    </div>
</div>

<script type="text/javascript">
$('#statistics_table_enterprise_container').dataTable({
    "aaData": [
        /* Reduced data set */
        ["Trident", "Internet Explorer 4.0", "Win 95+", 4, "X"],
        ["Trident", "Internet Explorer 5.0", "Win 95+", 5, "C"],
        ["Trident", "Internet Explorer 5.5", "Win 95+", 5.5, "A"],
        ["Trident", "Internet Explorer 6.0", "Win 98+", 6, "A"],
        ["Trident", "Internet Explorer 7.0", "Win XP SP2+", 7, "A"],
        ["Gecko", "Firefox 1.5", "Win 98+ / OSX.2+", 1.8, "A"],
        ["Gecko", "Firefox 2", "Win 98+ / OSX.2+", 1.8, "A"],
        ["Gecko", "Firefox 3", "Win 2k+ / OSX.3+", 1.9, "A"],
        ["Webkit", "Safari 1.2", "OSX.3", 125.5, "A"],
        ["Webkit", "Safari 1.3", "OSX.3", 312.8, "A"],
        ["Webkit", "Safari 2.0", "OSX.4+", 419.3, "A"],
        ["Webkit", "Safari 3.0", "OSX.4+", 522.1, "A"]
    ],
    "aoColumns": [{
        "sTitle": "Engine"
    }, {
        "sTitle": "Browser"
    }, {
        "sTitle": "Platform"
    }, {
        "sTitle": "Version",
        "sClass": "center"
    }, {
        "sTitle": "Grade",
        "sClass": "center"
    }],
    "iDisplayLength": 6,
    "sPaginationType": "full_numbers",
    "bLengthChange": false,
    "bInfo": false,
    "bFilter": false,
    "bSort": false,

});
</script>

    </div>
</div>

<div class="row">
    <div class="col-md-6">
         <div class="panel panel-primary">
          <div class="panel-heading">
            <h3 class="panel-title">（转移联单及其危废重量）危废统计图</h3>
          </div>
          <div class="panel-body">   
			<div id="statistics_graph_waste_container"></div>
		  </div>
</div>


<script type="text/javascript">
$('#statistics_graph_waste_container').highcharts({
              chart: {
            type: 'column'
        },
        title: {
            text: '危废统计图'
        },
        xAxis: {
            categories: ['Apples', 'Oranges', 'Pears', 'Grapes', 'Bananas']
        },
        yAxis: {
            min: 0,
            title: {
                text: 'Total fruit consumption'
            },
            stackLabels: {
                enabled: true,
                style: {
                    fontWeight: 'bold',
                    color: (Highcharts.theme && Highcharts.theme.textColor) || 'gray'
                }
            }
        },
        legend: {
            align: 'right',
            x: -70,
            verticalAlign: 'top',
            y: 20,
            floating: true,
            backgroundColor: (Highcharts.theme && Highcharts.theme.legendBackgroundColorSolid) || 'white',
            borderColor: '#CCC',
            borderWidth: 1,
            shadow: false
        },
        tooltip: {
            formatter: function() {
                return '<b>'+ this.x +'</b><br/>'+
                    this.series.name +': '+ this.y +'<br/>'+
                    'Total: '+ this.point.stackTotal;
            }
        },
        plotOptions: {
            column: {
                stacking: 'normal',
                dataLabels: {
                    enabled: true,
                    color: (Highcharts.theme && Highcharts.theme.dataLabelsColor) || 'white'
                }
            }
        },
        series: [{
            name: 'John',
            data: [5, 3, 4, 7, 2]
        }, {
            name: 'Jane',
            data: [2, 2, 3, 2, 1]
        }, {
            name: 'Joe',
            data: [3, 4, 4, 2, 5]
        }]
   
});</script>

    </div>
    <div class="col-md-6">
        <div class="panel panel-primary">
    <div class="panel-heading">
        <h3 class="panel-title">（转移联单及其危废重量）危废统计表</h3>
    </div>
    <div class="panel-body">
        <table class="table" id="statistics_table_waste_container">
        </table>
    </div>
</div>

<script type="text/javascript">
$('#statistics_table_waste_container').dataTable({
    "aaData": [
        /* Reduced data set */
        ["Trident", "Internet Explorer 4.0", "Win 95+", 4, "X"],
        ["Trident", "Internet Explorer 5.0", "Win 95+", 5, "C"],
        ["Trident", "Internet Explorer 5.5", "Win 95+", 5.5, "A"],
        ["Trident", "Internet Explorer 6.0", "Win 98+", 6, "A"],
        ["Trident", "Internet Explorer 7.0", "Win XP SP2+", 7, "A"],
        ["Gecko", "Firefox 1.5", "Win 98+ / OSX.2+", 1.8, "A"],
        ["Gecko", "Firefox 2", "Win 98+ / OSX.2+", 1.8, "A"],
        ["Gecko", "Firefox 3", "Win 2k+ / OSX.3+", 1.9, "A"],
        ["Webkit", "Safari 1.2", "OSX.3", 125.5, "A"],
        ["Webkit", "Safari 1.3", "OSX.3", 312.8, "A"],
        ["Webkit", "Safari 2.0", "OSX.4+", 419.3, "A"],
        ["Webkit", "Safari 3.0", "OSX.4+", 522.1, "A"]
    ],
    "aoColumns": [{
        "sTitle": "Engine"
    }, {
        "sTitle": "Browser"
    }, {
        "sTitle": "Platform"
    }, {
        "sTitle": "Version",
        "sClass": "center"
    }, {
        "sTitle": "Grade",
        "sClass": "center"
    }],
    "iDisplayLength": 6,
    "sPaginationType": "full_numbers",
    "bLengthChange": false,
    "bInfo": false,
    "bFilter": false,
    "bSort": false,

});
</script>

    </div>
</div>
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