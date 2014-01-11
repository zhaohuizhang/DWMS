<?php if (!defined('THINK_PATH')) exit();?><!doctype html>
<html lang="zh-cn">

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>安庆市危废管理系统</title>

    <!-- Bootstrap core CSS -->
    <link href="__PUBLIC__/css/bootstrap.css" rel="stylesheet">
    <!-- My CSS -->
    <link href="__PUBLIC__/css/layout.css" rel="stylesheet">
    <link href="__PUBLIC__/css/color.css" rel="stylesheet">

</head>

<body>
    <div class="container">
        <div id="webpage_title">
            <img id="logo" src="__PUBLIC__/image/aqshbj_logo_64.png" alt="logo" />
            <h2 id="title">安庆市危险废物管理信息系统</h2>
        </div>
        <div id="">
            <p id="welcome">欢迎您，<?php echo ((session('username'))?(session('username')):"先生"); ?></p>
            <a id="password" href=#>修改密码</a>
            <a id="logout" href="<?php echo U('Home/Login/logout');?>">退出登录</a>
            <div id="datetime"></div>
        </div>
        <div>
            <!-- Nav tabs -->
            <ul class="nav nav-tabs">
            <!-- <ul class="nav nav-pills"> -->
                <li class="active"><a href="#home" data-toggle="tab">首页</a>
                </li>
                <li><a href="#production" data-toggle="tab">产生单位</a>
                </li>
                <li><a href="#transport" data-toggle="tab">运输单位</a>
                </li>
                <li><a href="#reception" data-toggle="tab">处置单位</a>
                </li>
                <li><a href="#transfer" data-toggle="tab">危废转移</a>
                </li>
                <li><a href="#business" data-toggle="tab">业务办理</a>
                </li>
                <li><a href="#system" data-toggle="tab">系统管理</a>
                </li>
            </ul>

            <!-- Tab panes -->
            <div class="tab-content">
                <div class="tab-pane fade in active" id="home">首页</div>
                <div class="tab-pane fade" id="production">
                    <div class="panel-group" id="accordion">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h4 class="panel-title">
                                    <a data-toggle="collapse" data-toggle="collapse" data-parent="#accordion" href="#collapseOne">
          企业基本信息
        </a>
                                </h4>
                            </div>
                            <div id="collapseOne" class="panel-collapse collapse in">
                                <div class="panel-body">
                                    单位名称
                                </div>
                            </div>
                        </div>
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h4 class="panel-title">
                                    <a data-toggle="collapse" data-toggle="collapse" data-parent="#accordion" href="#collapseTwo">
          自行处置设备信息
        </a>
                                </h4>
                            </div>
                            <div id="collapseTwo" class="panel-collapse collapse">
                                <div class="panel-body">
                                    自行处置设备信息
                                </div>
                            </div>
                        </div>
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h4 class="panel-title">
                                    <a data-toggle="collapse" data-toggle="collapse" data-parent="#accordion" href="#collapseThree">
          监督执法
        </a>
                                </h4>
                            </div>
                            <div id="collapseThree" class="panel-collapse collapse">
                                <div class="panel-body">
                                    监督执法
                                </div>
                            </div>
                        </div>
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h4 class="panel-title">
                                    <a data-toggle="collapse" data-toggle="collapse" data-parent="#accordion" href="#collapseFour">
          监测报告
        </a>
                                </h4>
                            </div>
                            <div id="collapseFour" class="panel-collapse collapse">
                                <div class="panel-body">
                                    监测报告
                                </div>
                            </div>

                        </div>
                    </div>
                </div>
                <div class="tab-pane fade" id="transport">运输单位</div>
                <div class="tab-pane fade" id="reception">处置单位</div>
                <div class="tab-pane fade" id="transfer">危废转移</div>
                <div class="tab-pane fade" id="business">业务办理</div>
                <div class="tab-pane fade" id="system">系统管理</div>
            </div>
        </div>

    </div>
    <!-- /container -->

    <!-- Placed at the end of the document so the pages load faster -->
    <!-- jQuery core JavaScript -->
    <script type="text/javascript" src="__PUBLIC__/js/jquery-2.0.3.min.js"></script>
    <!-- Bootstrap core JavaScript-->
    <script type="text/javascript" src="__PUBLIC__/js/bootstrap.min.js"></script>
    <!-- My JavaScript-->
    <script type="text/javascript" src="__PUBLIC__/js/datetime.js"></script>

</body>

</html>