<?php
date_default_timezone_set("PRC");
function wasteIn(){
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
	$rfidList = $json_data->rfidlist;
	$imei = $json_data->imei;
	$result = mysql_query("SELECT ownership_id FROM device WHERE device_serial_num='".$imei."'");
	$userId = null;
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
	
	$result1 = mysql_query("SELECT reception_unit_id FROM reception_unit WHERE user_id='".$userId."'");

	if(!mysql_num_rows($result1)){
				$error->code = 2;
				$error->des = urlencode('该用户没有企业');
				$resdata->error = $error;
				return $resdata;
	}


	while($row = mysql_fetch_array($result1))
	{
	  $receivingId = $row['reception_unit_id'];
	}
	$receivingUnit = "reception_unit_".$receivingId;
	
	$key = 0;
	foreach($rfidList as $rfidobj){
		$rfid = $rfidobj->rfid;
		$result2 = mysql_query("SELECT * FROM rfid WHERE rfid_id='".$rfid."'");
		if(!mysql_num_rows($result2)){
			$error[$key]->code = 8;
			$error[$key]->des = urlencode('RFID标签没有绑定废物');
			$error[$key]->rfid = $rfid;
			$key++;
		}else{
			while($row = mysql_fetch_array($result2))
			{
				$stat = $row['status'];
				$wasteId = $row['waste_id'];
				$addWay = $row['add_way'];
				$total = $row['waste_total'];
			}
			if($stat==2){
				$error[$key]->code = 15;
				$error[$key]->des = urlencode('废物已经入库');
				$error[$key]->rfid = $rfid;
				$key++;
			}elseif($stat!=1){
				$error[$key]->code = 11;
				$error[$key]->des = urlencode('不能够入库');
				$error[$key]->rfid = $rfid;
				$key++;
			}else{
				$column = null;
				if($addway==0){
					$column = 'total_weight';
				}else{
					$column = 'total_num';
				}
				$time = date("Y-m-d H:i:s");
				$sql3 = "UPDATE rfid SET update_date_time = '$time',status = 2 WHERE rfid_id = '$rfid'";
				if (!mysql_query($sql3,$con))
				{
					$error[$key]->code = 3;
					$error[$key]->des = urlencode('更新RFID数据库失败');
					$error[$key]->rfid = $rfid;
					$key++;
				}else{
					$sql4 = "INSERT INTO $receivingUnit (rfid_id, waste_id, receive_date_time,android_num,$column) VALUES ('$rfid', '$wasteId', '$time','$imei','$total')";
					if (!mysql_query($sql4,$con))
					{
						$error[$key]->code = 13;
						$error[$key]->des = urlencode('更新仓库数据失败');
						$error[$key]->rfid = $rfid;
						$key++;
					}
				}
				
			}
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

echo urldecode(json_encode(wasteIn()));

?>