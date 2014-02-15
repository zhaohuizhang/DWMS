<?php
/**
 *
 */
class DistrictMapAction extends CommonAction{
	// -------- 转移地图 侧边栏 --------
	public function map_sidebar() {
		layout( './Common/frame' );
		$this->display( './Public/html/Content/District/map/map_sidebar.html' );
	}

	// 转移地图->地图展示->转移地图展示
	public function transfer_map_display() {
		$vehicle = M( 'vehicle' );
		$vehicle_operating = $vehicle->where( 'vehicle_status=2' );
		$vehicle_info = $vehicle_operating->find();
		$vehicle_info_json = json_encode( $vehicle_info );

		$vehicle_id = $vehicle_operating->getField( 'vehicle_id' );
		$ownership_id = $vehicle_operating->getField( 'ownership_id' );
		$transport_unit = M( 'transport_unit' )->where( array( 'transport_unit_id' => $ownership_id ) )->find();
		$transport_unit_json = json_encode( $transport_unit );

		if ( !$vehicle_id ) {
			$this->ajaxReturn( "没找到车辆表ID:" . $vehicle_id );
		}
		/*foreach ($vehicle as $vehicle_idx) {
			$route_id = M( 'route_vehicle' )->where( 'vehicle_id=$vehicle_idx->vehicle_id' )->getField('route_id');
			$route = M( 'route' )->where( 'route_id=$route_id' )->find();
		}*/
		$route_id = M( 'route_vehicle' )->where( array( 'vehicle_id' => $vehicle_id ) )->getField( 'route_id' );
		if ( !$route_id ) {
			$this->ajaxReturn( "没找到路线ID:" . $route_id );
		}
		$route = M( 'route' )->where( array( 'route_id' => $route_id ) )->find();
		if ( !$route ) {
			$this->ajaxReturn( "没找到路线:" . $route );
		}
		$route_json = json_encode( $route );

		$tmp_content=$this->fetch( './Public/html/Content/District/map/transfer_map_display.html' );
		$tmp_content = "<script>transport_unit_json=$transport_unit_json; vehicle_info_json=$vehicle_info_json; route_json=$route_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 转移地图->地图展示->转移地图展示：获取实时GPS数据
	public function ajax_gps_data() {
		$vehicle_gps_id = M( 'vehicle' )->where( 'vehicle_status=2' )->getField( 'vehicle_gps_id' );
		if ( !$vehicle_gps_id ) {
			$this->ajaxReturn( "没找到车辆表GPS的ID:" . $vehicle_gps_id );
		}
		$device_serial_num = M( 'device' )->where( array( 'device_id' => $vehicle_gps_id ) )->getField( 'device_serial_num' );
		if ( !$device_serial_num ) {
			$this->ajaxReturn( "没找到设备表GPS的序列号:" . $device_serial_num );
		}
		$gps_table_name = 'gps_' . $device_serial_num;
		// $gps_table_name = 'gps_308033501795';
		$gps = M( $gps_table_name );
		$gps_max_id = $gps->where( 'status=0' )->max( 'id' );
		$gps_data = $gps->where( array( 'id' => $gps_max_id ) )->find();
		$this->ajaxReturn( $gps_data, 'JSON' );
	}

	// 转移地图->地图展示->仓库地图展示
	public function warehouse_map_display() {
		$tmp_content=$this->fetch( './Public/html/Content/District/map/warehouse_map_display.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 转移地图->路线规划->运输路线规划，按照鼠标点规划路线
	public function transfer_route_plan_1() {
		$record = M( 'vehicle_gps_transport' )->where( "vehicle_status<2" )->select();
		$record_json = json_encode( $record );

		$tmp_content=$this->fetch( './Public/html/Content/District/map/transfer_route_plan_1.html' );
		$this->ajaxReturn( "<script>record_json=$record_json; </script> $tmp_content" );
	}

	// 转移地图->路线规划->运输路线规划，按照百度API自助规划路线
	public function transfer_route_plan_2() {
		$record = M( 'vehicle_gps_transport' )->where( "vehicle_status<2" )->select();
		$record_json = json_encode( $record );

		$tmp_content=$this->fetch( './Public/html/Content/District/map/transfer_route_plan_2.html' );
		$tmp_content="<script>record_json=$record_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 转移地图->路线规划->运输路线规划：ajax传回路线数据
	public function ajax_map_plan_receiver() {
		$table=M( "route" );
		if ( $table ) {
			$data["route_lng_lat"]=I( 'post.route_lng_lat' );
			$time = date( 'Y-m-d H:i:s', time() );
			$data["route_add_time"]=$time;
			$data["route_modify_time"]=$time;
			$data["route_status"]=0;
			$table->add( $data );
			$this->show( "succ" );
		}else {
			$this->show( "fail" );
		}
	}

	// 转移地图->路线规划->运输路线规划，百度API规划路线：生产单位->接受单位
	public function transfer_route_plan_3() {
		// $record = M( 'vehicle_gps_transport' )->where( "vehicle_status=0" )->select();
		// $record_json = json_encode( $record );

		$production_unit = M( 'production_unit' )->select();
		$production_unit_json = json_encode( $production_unit );
		$reception_unit = M( 'reception_unit' )->select();
		$reception_unit_json = json_encode( $reception_unit );

		$tmp_content=$this->fetch( './Public/html/Content/District/map/transfer_route_plan_3.html' );
		$tmp_content="<script> production_unit_json=$production_unit_json; reception_unit_json=$reception_unit_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 转移地图->路线规划->运输路线规划：ajax传回路线数据
	public function ajax_transfer_route_receiver() {
		$table=M( "route" );
		if ( $table ) {
			$data["production_unit_id"] = I( 'post.production_unit_id' );
			$data["reception_unit_id"] = I( 'post.reception_unit_id' );
			$data["route_lng_lat"]=I( 'post.route_lng_lat' );
			$time = date( 'Y-m-d H:i:s', time() );
			$data["route_add_time"]=$time;
			$data["route_modify_time"]=$time;
			$data["route_status"]=0;
			$table->add( $data );
			$this->show( "succ" );
		}else {
			$this->show( "fail" );
		}
	}

	// 转移地图->路线查询->运输路线查询
	public function transfer_route_query() {
		$production_unit = M( 'production_unit' )->getField( 'production_unit_id, production_unit_name' );
		$reception_unit = M( 'reception_unit' )->getField( 'reception_unit_id, reception_unit_name' );
		$production_unit_json = json_encode( $production_unit );
		$reception_unit_json = json_encode( $reception_unit );
		if ( $production_unit_json && $reception_unit_json ) {
			$tmp_content=$this->fetch( './Public/html/Content/District/map/transfer_route_query.html' );
			$tmp_content = "<script>production_unit_json=$production_unit_json; reception_unit_json=$reception_unit_json; </script> $tmp_content";
			$this->ajaxReturn( $tmp_content );
		} else {
			$this->show( 'fail' );
		}
	}

	// 转移地图->地图展示->转移地图展示：查找路线
	public function ajax_search_route() {
		$route = M( 'route' )->where( array( 'production_unit_id' => I( 'post.production_unit_id' ), 'reception_unit_id' => I( 'post.reception_unit_id' ) ) )->select();
		// $route = htmlspecialchars_decode($route);
		// $route = stripslashes($route);

		$route_json = json_encode( $route );
		//$route_json = htmlspecialchars_decode( $route_json );
		//$route_json = json_encode( $route_json );

		//$route_json = stripslashes($route_json);
		$this->ajaxReturn( $route_json, 'JSON' );
	}

}
?>
