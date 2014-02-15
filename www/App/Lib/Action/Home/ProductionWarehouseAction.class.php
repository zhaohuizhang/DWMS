<?php
/**
 *
 */
class ProductionWarehouseAction extends CommonAction{
	// -------- 危废库存->侧边栏 --------
	public function warehouse_sidebar() {
		layout( './Common/frame' );
		$this->display( './Public/html/Content/Production/warehouse/warehouse_sidebar.html' );
	}

	// 危废库存->危废入库管理
	public function storage_input_management($record_id="") {
		$rfid = M( 'rfid' )->where( array( 'ownership_id' => session( 'production_unit_id' ) ) )->select();
		$rfid_json = json_encode( $rfid );
		$tmp_content=$this->fetch( './Public/html/Content/Production/warehouse/storage_input_management.html' );
		$tmp_content="<script> record_json=$rfid_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废库存->危废入库管理->详情
	public function storage_input_management_page() {
		$production_unit = "production_unit_".session( 'production_unit_id' );
		$waste = M( $production_unit )->where( array( 'rfid_id' => $record_id ) )->select();
		$rfid_json = json_encode( $waste );
		//$this->waste_data=$rfid_json;
		$tmp_content=$this->fetch( './Public/html/Content/Production/warehouse/storage_input_management_page.html' );
		$tmp_content="<script> waste_data=$rfid_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废库存->危废在库查询
	public function storage_query() {
		$tmp_content=$this->fetch( './Public/html/Content/Production/warehouse/storage_query.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 危废库存->危废出库管理
	public function storage_output_management() {
		$tmp_content=$this->fetch( './Public/html/Content/Production/warehouse/storage_output_management.html' );
		$this->ajaxReturn( $tmp_content );
	}
}
?>
