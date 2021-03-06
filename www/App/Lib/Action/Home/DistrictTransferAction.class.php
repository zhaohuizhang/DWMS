<?php
/**
 *
 */
class DistrictTransferAction extends CommonAction{
	// -------- 危废转移->侧边栏 --------
	public function transfer_sidebar(){
		layout( './Common/frame' );
		$this->display( './Public/html/Content/District/transfer/transfer_sidebar.html' );
	}

	// 危废转移->转移备案管理->转移备案
	public function transfer_record(){
		$record = M( 'record' )->where( 'record_status>0' )->getField( 'record_id,record_code,record_date,record_status' );
		$record_json = json_encode( $record );

		$unit_name = M( 'production_unit' )->getField( 'production_unit_name' );
		$unit_json = json_encode( $unit_name );

		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/transfer_record.html' );
		$tmp_content = "<script>record_json = $record_json; unit_json = $unit_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废转移->转移备案管理->转移备案：详情页
	public function transfer_record_page($record_id=""){
		$record = M( 'record' )->where( array( 'record_id' =>$record_id ) )->find();
		$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => $record['production_unit_id'] ) )->find();
		$this->record = $record;
		$this->unit = $production_unit;

		//$record_id_json = json_encode( $record_id );
		//$record_status_json = json_encode( $record['record_status'] );

		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/transfer_record_page.html' );
		//$tmp_content = "<script>record_id_json = $record_id_json; record_status_json = $record_status_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废转移->转移联单管理->生产单位转移联单
	public function production_transfer_manifest(){
		$production_transfer_manifest = M( 'manifest' )->where( 'manifest_status=1 or manifest_status=4 or manifest_status=5' )->getField( 'manifest_id,manifest_num,manifest_add_time,manifest_status' );
		$production_transfer_manifest_json = json_encode( $production_transfer_manifest );

		$unit_name = M( 'production_unit' )->getField( 'production_unit_name' );
		$unit_json = json_encode( $unit_name );

		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/production_transfer_manifest.html' );
		$tmp_content = "<script>manifest_json = $production_transfer_manifest_json; unit_json = $unit_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}
	//危废转移->转移联单管理->生产单位转移联单: 详情页
	public function production_transfer_manifest_page($manifest_id=""){
		$manifest = M( 'manifest' )->where( array( 'manifest_id' =>$manifest_id ) )->find();
		$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => $manifest['production_unit_id'] ) )->find();
		$this->manifest = $manifest;
		$this->unit = $production_unit;

		// $manifest_id_json = json_encode( $manifest_id );
		// $manifest_status_json = json_encode( $manifest['manifest_status'] );

		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/production_transfer_manifest_page.html' );
		// $tmp_content = "<script>manifest_id_json = $manifest_id_json; manifest_status_json = $manifest_status_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废转移->转移联单管理->运输单位转移联单
	public function transport_transfer_manifest(){
		$transport_transfer_manifest = M( 'manifest' )->where( 'manifest_status=2' )->getField( 'manifest_id,manifest_num,manifest_add_time,manifest_status' );
		$transport_transfer_manifest_json = json_encode( $transport_transfer_manifest );

		$unit_name = M( 'transport_unit' )->getField( 'transport_unit_name' );
		$unit_json = json_encode( $unit_name );

		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/transport_transfer_manifest.html' );
		$tmp_content = "<script>manifest_json = $transport_transfer_manifest_json; unit_json = $unit_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}
	//危废转移->转移联单管理->运输单位转移联单：详情页
	public function transport_transfer_manifest_page($manifest_id=""){
		$manifest = M( 'manifest' )->where( array( 'manifest_id' =>$manifest_id ) )->find();
		$transport_unit = M( 'transport_unit' )->where( array( 'transport_unit_id' => $manifest['transport_unit_id'] ) )->find();
		$this->manifest = $manifest;
		$this->unit = $transport_unit;

		// $manifest_id_json = json_encode( $manifest_id );
		// $manifest_status_json = json_encode( $manifest['manifest_status'] );

		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/transport_transfer_manifest_page.html' );
		// $tmp_content = "<script>manifest_id_json = $manifest_id_json; manifest_status_json = $manifest_status_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废转移->转移联单管理->接受单位转移联单
	public function reception_transfer_manifest(){
		$reception_transfer_manifest = M( 'manifest' )->where( 'manifest_status=3 or manifest_status=6 manifest_status=7 manifest_status=8 manifest_status=9' )->getField( 'manifest_id,manifest_num,manifest_add_time,manifest_status' );
		$reception_transfer_manifest_json = json_encode( $reception_transfer_manifest );

		$unit_name = M( 'reception_unit' )->getField( 'reception_unit_name' );
		$unit_json = json_encode( $unit_name );

		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/reception_transfer_manifest.html' );
		$tmp_content = "<script>manifest_json = $reception_transfer_manifest_json; unit_json = $unit_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废转移->转移联单管理->接受单位转移联单: 详情页
	public function reception_transfer_manifest_page($manifest_id=""){
		$manifest = M( 'manifest' )->where( array( 'manifest_id' =>$manifest_id ) )->find();
		$reception_unit = M( 'reception_unit' )->where( array( 'reception_unit_id' => $manifest['reception_unit_id'] ) )->find();
		$this->manifest = $manifest;
		$this->unit = $reception_unit;

		// $manifest_id_json = json_encode( $manifest_id );
		// $manifest_status_json = json_encode( $manifest['manifest_status'] );

		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/reception_transfer_manifest_page.html' );
		// $tmp_content = "<script>manifest_id_json = $manifest_id_json; manifest_status_json = $manifest_status_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 危废转移->统计分析->转移去向统计
	public function transfer_direction_statistic(){
		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/transfer_direction_statistic.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 危废转移->统计分析->转移联单统计
	public function transfer_manifest_statistic(){
		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/transfer_manifest_statistic.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 危废转移->统计分析->转移专题定制
	public function transfer_topic_customize(){
		$tmp_content=$this->fetch( './Public/html/Content/District/transfer/transfer_topic_customize.html' );
		$this->ajaxReturn( $tmp_content );
	}

}
?>