<?php
date_default_timezone_set("PRC");
function addWaste(){
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
	$json_string = $_POST['txt_json'];
	 if(ini_get("magic_quotes_gpc")=="1")
	{
	  $json_string=stripslashes($json_string);
	}
	$json_data = json_decode($json_string);
	$rfid = $json_data->rfid;
	$wasteid = $json_data->wasteid;
	$imei = $json_data->imei;
	$addway = $json_data->addway;
	$addnum = $json_data->addnum;
	
	$column = null;
	if($addnum<=0){
		$error->code = 16;
		$error->des = urlencode('输入的数值必须为正数');
		$resdata->error = $error;
		return $resdata;
	}
	if($addway==0){
		$column = 'add_weight';
	}else{
		$column = 'add_num';
	}

	$result = mysql_query("SELECT ownership_id FROM device WHERE device_serial_num='".$imei."'");
	$userId = null;
	$productionId = null;
	if(!mysql_num_rows($result)){
				$error->code = 0;
				$error->des = urlencode('企业没有绑定手持设备');
				$resdata->error = $error;
				return $resdata;
	}

	while($row = mysql_fetch_array($result))
	  {
	  $userId = $row['ownership_id'];
	  }
	$result1 = mysql_query("SELECT production_unit_id FROM production_unit WHERE user_id='".$userId."'");

	if(!mysql_num_rows($result1)){
				$error->code = 2;
				$error->des = urlencode('该用户没有企业');
				$resdata->error = $error;
				return $resdata;
	}


	while($row = mysql_fetch_array($result1))
	{
	  $productionId = $row['production_unit_id'];
	}
	$productionUnit = "production_unit_".$productionId;


	$time = date("Y-m-d H:i:s");
	$result2 = mysql_query("SELECT * FROM rfid WHERE rfid_id='".$rfid."'");
	if(!mysql_num_rows($result2)){
		$error->code = 8;
		$error->des = urlencode('RFID标签没有绑定废物');
		$resdata->error = $error;
		return $resdata;
	}
	$wasteTotal = 0;
	while($row = mysql_fetch_array($result2))
		  {
			  $wasteTotal = $row['waste_total'];
			  $wasteStatus = $row['status'];
		  }
	if($wasteStatus==1 or $wasteStatus==2){
		$error->code = 12;
		$error->des = urlencode('废物已出库');
		$resdata->error = $error;
		return $resdata;
	}
		  
		  
	//echo $wasteTotal;
	$wasteTotal = $wasteTotal + $addnum;
	$sql1 = "UPDATE rfid SET update_date_time = '$time',status = 3, waste_total = '$wasteTotal' WHERE rfid_id = '$rfid' AND waste_id = '$wasteid'";
	
	$key = 0;
	if (!mysql_query($sql1,$con))
	{
		$error[$key]->code = 3;
		$error[$key]->des = urlencode('更新RFID数据库失败');
		$error[$key]->rfid = $rfid;
		$key++;
	}else{
		$sql2 = "INSERT INTO $productionUnit (rfid_id, waste_id, add_date_time,android_num,$column) VALUES ('$rfid', '$wasteid', '$time','$imei','$addnum')";
		if (!mysql_query($sql2,$con))
		{
			$error[$key]->code = 4;
			$error[$key]->des = urlencode('写入企业库存数据失败');
			$error[$key]->rfid = $rfid;
		}
	}
	

	mysql_close($con);
	if(isset($error)){
				$newerror['error'] = $error;
				return $newerror;
	}else{
				$resultData->code = 200;
				return $resultData;
	}
}

echo urldecode(json_encode(addWaste()));

?>