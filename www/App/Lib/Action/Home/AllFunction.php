<?php
date_default_timezone_set("PRC");

function bindRfid($json_string){
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
		//$json_string = $_POST['txt_json'];
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
		  
		
		$result1 = mysql_query("SELECT production_unit_id FROM production_unit WHERE production_unit_id='".$userId."'");
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
			$time = date("Y-m-d H:i:s");
			if(isNotExist($rfid)){
				$sql1 = "INSERT INTO rfid (rfid_id, waste_id, add_date_time,status,add_way,ownership_id,waste_total) VALUES ('$rfid','$wasteId','$time','0','$addWay','$userId','0')";
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
				$key++;
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

function addWaste($json_string){
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
	//$json_string = $_POST['txt_json'];
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
	if($addnum<=0 or !is_numeric($addnum)){
		$error->code = 16;
		$error->des = urlencode('输入的数值必须为正数');
		$resdata->error = $error;
		return $resdata;
	}
	if($addway==0){
		$column = 'add_weight';
	}else{
		$column = 'add_num';
		$addnum = ceil($addnum);
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
	$result1 = mysql_query("SELECT production_unit_id FROM production_unit WHERE production_unit_id='".$userId."'");

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

function getRfidWasteName($rfid,$imei){
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
	// $rfid = $_GET["rfid"];
	// $imei = $_GET["imei"];
	
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

function getWasteName($imei){

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
	//$imei = $_GET["imei"];

	$result = mysql_query("SELECT ownership_id FROM device WHERE device_serial_num='".$imei."'");
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
	$result1 = mysql_query("SELECT production_unit_waste FROM production_unit WHERE production_unit_id='".$userId."'");

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
		$result2 = mysql_query("SELECT waste_name FROM waste WHERE waste_id='".$value."'");
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

function wasteIn($json_string){
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
	//$json_string = $_POST['txt_json'];
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
	
	$result1 = mysql_query("SELECT reception_unit_id FROM reception_unit WHERE reception_unit_id='".$userId."'");

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
				if($addWay==0){
					$column = 'total_weight';
				}else{
					$column = 'total_num';
				}
				$time = date("Y-m-d H:i:s");
				$sql3 = "UPDATE rfid SET update_date_time = '$time',status = 2,ownership_id = '$userId' WHERE rfid_id = '$rfid'";
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

function wasteOut($json_string){
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
	//$json_string = $_POST['txt_json'];
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
	$userId = null;
	while($row = mysql_fetch_array($result))
		  {
		  $userId = $row['ownership_id'];
		  }
		  
		
	$result1 = mysql_query("SELECT production_unit_id FROM production_unit WHERE production_unit_id='".$userId."'");
	if(!mysql_num_rows($result1)){
		$error->code = 2;
		$error->des = urlencode('该用户没有企业');
		$resdata->error = $error;
		return $resdata;
	}
	
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

?>