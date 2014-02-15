<?php
date_default_timezone_set("PRC");
$con = mysql_connect("10.50.6.70","root","root1234") or die("error");
mysql_select_db("dwms", $con);
$username=$_POST['user'];
$password=$_POST['pass'];
$password=md5($password);
$query=mysql_query("SELECT * from user where username='$username' and password='$password'");
if(mysql_num_rows($query)>0)
	echo "yes";
else
	echo "error";

?>