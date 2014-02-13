<?php

function getWasteName(){

	$con = mysql_connect("10.50.6.70","root","root1234");
	if (!$con)
	  {
		$error->code = 7;
		$error->des = urlencode('数据库连接失败');
		$resdata->error = $error;
		return $resdata;
	  }
	mysql_query("set names 'utf8'");
	mysql_select_db("dwms", $con);
	$imei = $_GET["imei"];

	$result = mysql_query("SELECT ownership_id FROM device WHERE device_serial_num='".$imei."'") or die(mysql_error());
	$userId = null;
	$wasteArray = array();
	$wasteName = null;
	if(!mysql_num_rows($result)){
		$error->code = 0;
		$error->des = urlencode('企业没有绑定手持设备');
		$resdata->error = $error;
		return $resdata;
	}
	while($row = mysql_fetch_array($result))
	  {
	  $userId = $row['ownership_id'];
	  //echo $userId;
	  }
	$result1 = mysql_query("SELECT production_unit_waste FROM production_unit WHERE user_id='".$userId."'") or die(mysql_error());

	if(!mysql_num_rows($result1)){
		$error->code = 1;
		$error->des = urlencode('企业没有注册危险固废');
		$resdata->error = $error;
		return $resdata;
	}

	while($row = mysql_fetch_array($result1))
	  {
	  $wasteArray = split(",",$row['production_unit_waste']);
	  }
	foreach ($wasteArray as $key => $value) {
		//echo $value;
		$result2 = mysql_query("SELECT waste_name FROM waste WHERE waste_id='".$value."'") or die(mysql_error());
		while($row = mysql_fetch_array($result2))
	  {
		$wasteName = urlencode($row['waste_name']);
	  }
		$newDate[$key]['name'] =  $wasteName;
		$newDate[$key]['id'] = $value;
	}
	$resultData['code'] = 200;
	$resultData['wasteOptions'] = $newDate;
	mysql_close($con);
	return $resultData;
}
echo urldecode(json_encode(getWasteName()));
?>