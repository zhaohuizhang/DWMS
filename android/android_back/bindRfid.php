<?php
date_default_timezone_set("PRC");
function bindRfid(){
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
		//$json_string = file_get_contents("php://input");
		 if(ini_get("magic_quotes_gpc")=="1")
		 {
		  $json_string=stripslashes($json_string);
		 }
		$json_data = json_decode($json_string);
		$wasteBind = $json_data->wasteBindList;
		$imei = $json_data->imei;
		function isNotExist($rfid){
			$result = mysql_query("SELECT * FROM rfid WHERE rfid_id='".$rfid."'");
			if (!mysql_num_rows($result))
				{
					return true;
				}
			else
				{
					return false;
				}
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
		  
		
		$result1 = mysql_query("SELECT production_unit_id FROM production_unit WHERE user_id='".$userId."'") or die(mysql_error());
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
		
		$key = 0;
		foreach($wasteBind as $wasteRfid){
			$rfid = $wasteRfid->rfid;
			$wasteId = $wasteRfid->wasteid;
			$addWay = $wasteRfid->addway;
			echo $rfid."  ".$wasteId."  ".$addWay;
			$time = date("Y-m-d H:i:s");
			if(isNotExist($rfid)){
				$sql1 = "INSERT INTO rfid (rfid_id, waste_id, add_date_time,status,add_way,user_id,waste_total) VALUES ('$rfid','$wasteId','$time','0','$addWay','$userId','0')";
				if (!mysql_query($sql1,$con))
				  {
					//die(mysql_error());
					$error[$key]->code = 3;
					$error[$key]->des = urlencode('写入RFID数据库失败');
					$error[$key]->rfid = $rfid;
					$key++;
				  }else{
				  $sql2 = "INSERT INTO $productionUnit (rfid_id, waste_id, add_date_time,android_num) VALUES ('$rfid', '$wasteId', '$time','$imei')";
				  if (!mysql_query($sql2,$con))
					 {
						$error[$key]->code = 4;
						$error[$key]->des = urlencode('写入企业库存数据失败');
						$error[$key]->rfid = $rfid;
						$key++;
					 }
				  }
				
			}else{
				$error[$key]->code = 5;
				$error[$key]->des = urlencode('RFID标签已经绑定');
				$error[$key]->rfid = $rfid;
			}
			$key++;
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

echo urldecode(json_encode(bindRfid()));

?>