<?php
/**
 *
 */
class DistrictProductionAction extends CommonAction{
	// -------- 危废产生单位->侧边栏 --------
	public function production_sidebar(){
		layout( './Common/frame' );
		$this->display( './Public/html/Content/District/production/production_sidebar.html' );
	}

	// 危废产生单位->企业基本信息->企业基本信息
	public function production_basic_information(){
		$production_unit = M( 'production_unit' )->getField('production_unit_id,production_unit_name,production_unit_address,waste_location_county,production_unit_jurisdiction');
		$production_unit_table = json_encode( $production_unit );
		$tmp_content = $this->fetch('./Public/html/Content/District/production/production_basic_information.html');
		$tmp_content = "<script>production_table_json = $production_unit_table;</script> $tmp_content";
		$this->ajaxReturn($tmp_content);
	}

	// 危废产生单位->企业基本信息->企业基本信息->企业详细信息
	public function production_basic_information_page($record_id=""){
		$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => $record_id ) )->find();
		$this->unit = $production_unit;
		$tmp_content=$this->fetch( './Public/html/Content/District/production/production_basic_information_page.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 危废产生单位->危险废物台账->危废产生月报统计
	public function waste_account_monthly_statistics(){
		$tmp_content=$this->fetch( './Public/html/Content/District/production/waste_account_monthly_statistics.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 危废产生单位->RFID手持端->RFID手持端绑定
	public function production_rfid_hand_equipment(){
		$tmp_content=$this->fetch( './Public/html/Content/District/production/production_rfid_hand_equipment.html' );
		$this->ajaxReturn( $tmp_content );
	}
}

?>