<?php
/**
 *
 */
class DistrictReceptionAction extends CommonAction{
	// -------- 危废处置单位->侧边栏 --------
	public function reception_sidebar(){
		layout( './Common/frame' );
		$this->display( './Public/html/Content/District/reception/reception_sidebar.html' );
	}

	// 危废处置单位->企业基本信息->企业基本信息
	public function reception_basic_information(){
		$reception_unit = M( 'reception_unit' )->getField( 'reception_unit_id,reception_unit_name,reception_unit_address,reception_unit_county,reception_unit_jurisdiction' );
		$reception_unit_table = json_encode( $reception_unit );
		$tmp_content=$this->fetch( './Public/html/Content/District/reception/reception_basic_information.html' );
		$tmp_content = "<script>reception_table_json = $reception_unit_table;</script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废处置单位->企业基本信息->企业基本信息->详情
	public function reception_basic_information_page(){
		$reception_unit = M( 'reception_unit' )->where( array( 'reception_unit_id' => $record_id ) )->find();
		$this->unit = $reception_unit;
		$tmp_content=$this->fetch( './Public/html/Content/District/reception/reception_basic_information_page.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 危废处置单位->危废接受台账->危废接受月报统计
	public function waste_reception_account_monthly_statistics(){
		$tmp_content=$this->fetch( './Public/html/Content/District/reception/waste_reception_account_monthly_statistics.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 危废处置单位->RFID手持端->RFID手持端绑定
	public function reception_rfid_hand_equipment(){
		$tmp_content=$this->fetch( './Public/html/Content/District/reception/reception_rfid_hand_equipment.html' );
		$this->ajaxReturn( $tmp_content );
	}

}
?>