<?php
/**
 *
 */
class DistrictTransportAction extends CommonAction{
	// -------- 危废运输单位->侧边栏 --------
	public function transport_sidebar(){
		layout( './Common/frame' );
		$this->display( './Public/html/Content/District/transport/transport_sidebar.html' );
	}

	// 危废运输单位->企业基本信息->企业基本信息
	public function transport_basic_information(){
		$transport_unit = M( 'transport_unit' )->getField( 'transport_unit_id,transport_unit_name,transport_unit_address,transport_unit_county,transport_unit_jurisdiction' );
		$transport_unit_table = json_encode( $transport_unit );
		$tmp_content=$this->fetch( './Public/html/Content/District/transport/transport_basic_information.html' );
		$tmp_content = "<script>transport_table_json = $transport_unit_table;</script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废运输单位->企业基本信息->企业基本信息->详情
	public function transport_basic_information_page(){
		$transport_unit = M( 'transport_unit' )->where( array( 'transport_unit_id' => $record_id ) )->find();
		$this->unit = $transport_unit;
		$tmp_content=$this->fetch( './Public/html/Content/District/transport/transport_basic_information_page.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 危废运输单位->运输车辆管理->运输车辆管理
	public function transport_vehicle_management(){
		$tmp_content=$this->fetch( './Public/html/Content/City/transport/transport_vehicle_management.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 危废运输单位->GPS监控信息->GPS监控信息
	public function transport_gps_monitor_information(){
		$tmp_content=$this->fetch( './Public/html/Content/City/transport/transport_gps_monitor_information.html' );
		$this->ajaxReturn( $tmp_content );
	}

}
?>