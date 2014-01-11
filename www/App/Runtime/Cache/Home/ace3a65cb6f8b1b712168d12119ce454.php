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

    <div class="panel panel-default">
  <div class="panel-body">
  
     <div class="btn-group" style="float:right;padding-top:30px">
 
   <button type="button" class="btn btn-primary btn-lg">Left</button>
   <button type="button" class="btn btn-default btn-lg">Left</button>
    <button type="button" class="btn btn-default btn-lg">Left</button>
     <button type="button" class="btn btn-default btn-lg">Left</button>
      <button type="button" class="btn btn-default btn-lg">Left</button>    

</div>
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
 </div>
</div>
        <div>

    <div class="row">  

<div class="col-md-6">
    <div class="panel panel-default">
          <div class="panel-heading">Panel heading without title</div>
          <div class="panel-body">
            <div class="col-md-6"><img src="http://www.baidu.com/img/bdlogo.gif"></div>
          </div>
    </div>
</div> 
<div class="col-md-6">
    <div class="panel panel-default">
          <div class="panel-heading">Panel heading without title</div>
          <div class="panel-body">
            <div class="col-md-6"><img src="http://www.baidu.com/img/bdlogo.gif"></div>
          </div>
    </div>
</div> 

<div class="col-md-6">
    <div class="panel panel-default">
          <div class="panel-heading">Panel heading without title</div>
          <div class="panel-body">
            <div class="col-md-6"><img src="http://www.baidu.com/img/bdlogo.gif"></div>
          </div>
    </div>
</div> 
<div class="col-md-6">
    <div class="panel panel-default">
          <div class="panel-heading">Panel heading without title</div>
          <div class="panel-body">
            <div class="col-md-6"><img src="http://www.baidu.com/img/bdlogo.gif"></div>
          </div>
    </div>
</div> 


<div class="col-md-6">
    <div class="panel panel-default">
          <div class="panel-heading">Panel heading without title</div>
          <div class="panel-body">
            <div class="col-md-6"><img src="http://www.baidu.com/img/bdlogo.gif"></div>
          </div>
    </div>
</div> 
<div class="col-md-6">
    <div class="panel panel-default">
          <div class="panel-heading">Panel heading without title</div>
          <div class="panel-body">
            <div class="col-md-6"><img src="http://www.baidu.com/img/bdlogo.gif"></div>
          </div>
    </div>
</div> 
</div>
      <script type="text/javascript" src="__PUBLIC__/js/jquery-2.0.3.min.js"></script>
    <!-- Bootstrap core JavaScript-->
    <script type="text/javascript" src="__PUBLIC__/js/bootstrap.min.js"></script>
    <!-- My JavaScript-->
    <script type="text/javascript" src="__PUBLIC__/js/datetime.js"></script>
    <div class="well well-lg"><p style="text-align:center">CopyRight xxxx 2014</p></div>

</body>

</html>