<?php
/**
 *
 */
class DistrictSystemAction extends CommonAction{
	// -------- 系统管理->侧边栏 --------
	public function system_sidebar(){
		layout( './Common/frame' );
		$this->display( './Public/html/Content/District/system/system_sidebar.html' );
	}

	// 系统管理->系统信息设置->管辖权限
	public function jurisdiction(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/jurisdiction.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->区县代码
	public function district_code(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/district_code.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->行业代码
	public function trade_code(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/trade_code.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->企业规模
	public function enterprise_scale(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/enterprise_scale.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->注册类型
	public function register_type(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/register_type.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->废物代码
	public function waste_code(){
		$waste = M( 'waste' )->order( 'waste_id DESC' )->select();
		$waste_json = json_encode( $waste );
		$tmp_content=$this->fetch( './Public/html/Content/District/system/waste_code.html' );
		$tmp_content="<script> record_json=$waste_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->废物类别
	public function waste_category(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/waste_category.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->废物形态
	public function waste_form(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/waste_form.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->包装方式
	public function package_method(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/package_method.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->危废处理方式
	public function waste_disposal_method(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/waste_disposal_method.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->危废去向
	public function waste_direction(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/waste_direction.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 系统管理->系统信息设置->运输方式
	public function transport_method(){
		$tmp_content=$this->fetch( './Public/html/Content/District/system/transport_method.html' );
		$this->ajaxReturn( $tmp_content );
	}

}
?>