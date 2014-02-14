<?php
/**
 *
 */
class ProductionBasicAction extends CommonAction{
	// -------- 企业基本信息->侧边栏 --------
	public function basic_sidebar(){
		layout( './Common/frame' );
		$this->display( './Public/html/Content/Production/basic/basic_sidebar.html' );
	}

	// 企业基本信息->企业基本信息
	public function production_basic_information(){
		$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->find();
		$this->unit = $production_unit;
		$tmp_content=$this->fetch( './Public/html/Content/Production/basic/production_basic_information.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 企业基本信息->基本信息变更
	public function production_change_information(){
		$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->find();
		$this->unit = $production_unit;
		$tmp_content=$this->fetch( './Public/html/Content/Production/basic/production_change_information.html' );
		$this->ajaxReturn( $tmp_content );
	}
}
?>