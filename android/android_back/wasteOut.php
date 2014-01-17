<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<?php
date_default_timezone_set("PRC");
function wasteOut(){
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
	
	if(!mysql_num_rows($result)){
				$error->code = 0;
				$error->des = urlencode('企业没有绑定手持设备');
				$resdata->error = $error;
				return $resdata;
	}

	$key = 0;
	foreach($rfidList as $rfidobj){
		$rfid = $rfidobj->rfid;
		$result1 = mysql_query("SELECT * FROM rfid WHERE rfid_id='".$rfid."'");
		if(!mysql_num_rows($result1)){
			$error[$key]->code = 8;
			$error[$key]->des = urlencode('RFID标签没有绑定废物');
			$error[$key]->rfid = $rfid;
			$key++;
		}else{
			while($row = mysql_fetch_array($result1))
			{
				$stat = $row['status'];
			}
			if($stat==1 or $stat ==2){
				$error[$key]->code = 12;
				$error[$key]->des = urlencode('废物已经出库');
				$error[$key]->rfid = $rfid;
				$key++;
			}elseif($stat == 0){
				$error[$key]->code = 14;
				$error[$key]->des = urlencode('废物为空');
				$error[$key]->rfid = $rfid;
				$key++;
			}else{
				$time = date("Y-m-d H:i:s");
				$sql1 = "UPDATE rfid SET update_date_time = '$time',status = 1 WHERE rfid_id = '$rfid'";
				if (!mysql_query($sql1,$con))
				{
					$error[$key]->code = 3;
					$error[$key]->des = urlencode('更新RFID数据库失败');
					$error[$key]->rfid = $rfid;
					$key++;
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

echo urldecode(json_encode(wasteOut()));

?>