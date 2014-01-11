<?php
/**
 * SelectSocketServer Class
 * By James.Napu(zhangzhaohui@sjtu.edu.cn)
**/
set_time_limit(0);
date_default_timezone_set("PRC");

class SelectSocketServer 
{
	private static $socket;
	private static $timeout = 60;
	private static $maxconns = 1024;
	private static $connections = array();
	function InsertDB($line){
		if(strpos($line,"#")>3){
			$con = mysql_connect('10.50.6.70', 'root', 'root1234');
			if (!$con)
			{
			 die('Could not connect: ' . mysql_error());
			}
			mysql_select_db("dwms", $con);
			$status = 1;
			$latitude = 0.0;
			$longitude = 0.0; 
			$speed = 0.0;
			$heigh = 0.0;
			$time = date("Y-m-d H:i:s");
			$id = substr($line,0,strpos($line,"#"));
			if((bool)strpos($line,"A")){
				$latitude =  substr($line,strpos($line,"N")+2,strpos($line,"E")-strpos($line,"N")-3);
				$longitude = substr($line,strpos($line,"E")+2,strpos($line,"V")-strpos($line,"E")-3);
				$speed = substr($line,strpos($line,"V")+2,strpos($line,"H")-strpos($line,"V")-3);
				$heigh = substr($line,strpos($line,"H")+2,strlen($line)-strpos($line,"H")-5);
				$status = 0;
			}
			$tableName = trim("gps_".$id);
			$sql = "INSERT INTO $tableName (datetime,longitude,latitude,height,speed,status) VALUES ('$time','$longitude','$latitude','$heigh','$speed','$status')";
			if(!mysql_query($sql,$con)){
			{
			  die('Error: ' . mysql_error());
			  }
			}
		}
	}
	function SelectSocketServer($port) 
	{
		global $errno, $errstr;
		if ($port < 1024) {
			die("Port must be a number which bigger than 1024\n");
		}
		
		$socket = socket_create_listen($port);
		if (!$socket) die("Listen $port failed");
		
		socket_set_nonblock($socket); // non_blocking
		
		while (true) 
		{
			$readfds = array_merge(self::$connections, array($socket));
			$writefds = array();
			
			//select one link and get the pip 
			if (socket_select($readfds, $writefds, $e = null, $t = self::$timeout)) 
			{
				if (in_array($socket, $readfds)) {
					$newconn = socket_accept($socket);
					$i = (int) ((bool)$newconn);
					$reject = '';
					if (count(self::$connections) >= self::$maxconns) {
						$reject = "Server full, Try again later.\n";
					}

					self::$connections[$i] = $newconn;

					$writefds[$i] = $newconn;

					if ($reject) {
						socket_write($writefds[$i], $reject);
						unset($writefds[$i]);
						self::close($i);
					} else {
						echo "Client $i come.\n";
					}
					// remove the listening socket from the clients-with-data array
					$key = array_search($socket, $readfds);
					unset($readfds[$key]);
				}
				
				
				foreach ($readfds as $rfd) {
				
					$i = (int) $rfd;
				
					$line = @socket_read($rfd, 2048, PHP_NORMAL_READ);
					if ($line === false) {
					    
						echo "Connection closed on socket $i.\n";
						self::close($i);
						continue;
					}
					$tmp = substr($line, -1);
					if ($tmp != "\r" && $tmp != "\n") {
	
						continue;
					}

					$line = trim($line);
					if ($line == "quit") {
						echo "Client $i quit.\n";
						self::close($i);
						break;
					}
					if ($line) {
						echo "Client $i >>" . $line . "\n";

						self::InsertDB($line);
						//$this->bar();
					}
				}
				
				//foreach ($writefds as $wfd) {
				//	$i = (int) $wfd;
				//	$w = socket_write($wfd, "Welcome Client $i!\n");
				//}
			}
		}
	}
	
	function close ($i) 
	{
		socket_shutdown(self::$connections[$i]);
		socket_close(self::$connections[$i]);
		unset(self::$connections[$i]);
	}
}
new SelectSocketServer(10008);
?>