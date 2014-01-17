<?php
function getRfidWasteName(){
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
	$rfid = $_GET["rfid"];
	$imei = $_GET["imei"];
	
	$result = mysql_query("SELECT ownership_id FROM device WHERE device_serial_num='".$imei."'") or die(mysql_error());
	$userId = null;
	$productionId = null;
	if(!mysql_num_rows($result)){
				$error->code = 0;
				$error->des = urlencode('企业没有绑定手持设备');
				$resdata->error = $error;
				return $resdata;
	}
	
	
	$result1 = mysql_query("SELECT * FROM rfid WHERE rfid_id='".$rfid."'");
	$wasteId = null;
	$addWay = null;
	if(!mysql_num_rows($result1)){
		$error->code = 8;
		$error->des = urlencode('RFID标签没有绑定废物');
		$resdata->error = $error;
		return $resdata;
		
	}else{
		while($row = mysql_fetch_array($result1))
		  {
			  $wasteId = $row['waste_id'];
			  $wasteWay = $row['add_way'];
			  $wasteTotal = $row['waste_total'];
		  }
		$result2 = mysql_query("SELECT waste_name FROM waste WHERE waste_id='".$wasteId."'");
		if(!mysql_num_rows($result2)){
			$error->code = 9;
			$error->des = urlencode('没有这个危废物');
			$resdata->error = $error;
			return $resdata;
		}
		while($row = mysql_fetch_array($result2))
		  {
			$wasteName = urlencode($row['waste_name']);
		  }
		$newData['name'] =  $wasteName;
		$newData['id'] = $wasteId;
		$newData['way'] = $wasteWay;
		$newData['total'] = $wasteTotal;
		return $newData;
	
	}
	mysql_close($con);

}

echo urldecode (json_encode(getRfidWasteName()));

?>