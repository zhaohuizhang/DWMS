<?php
/**
 *
 */
class ProductionRecordAction extends CommonAction{
	// -------- 转移备案->侧边栏 --------
	public function record_sidebar() {
		layout( './Common/frame' );
		$this->display( './Public/html/Content/Production/record/record_sidebar.html' );
	}

	// 转移备案->转移备案申请
	public function transfer_record_request() {
		$unit = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->find();
		$this->unit = $unit;
		$tmp_content=$this->fetch( './Public/html/Content/Production/record/transfer_record_request.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 转移备案->转移备案申请->表单提交
	public function transfer_record_request_form() {
		$record = M( 'record' ); //实例化record对象
		$record->create(); // 根据表单提交的POST数据创建数据对象
		$time = date( 'Y-m-d H:i:s', time() );
		$record->record_add_time = $time;
		$record->record_modify_time = $time;

		$record->production_unit_id = session( 'production_unit_id' );
		$record->record_code = session( 'production_unit_id' ) . '-' . date( 'Y-m' ) . '-' . ( M( 'record' )->max( 'record_id' )+1 );

		$record->record_status = I( 'post.record_status' );
		$result = $record->add(); // 根据条件保存修改的数据

		if ( $result ) {
			$this->ajaxReturn( 1, '保存成功！', 1 );
		} else {
			$this->ajaxReturn( 0, '保存失败！', 0 );
		}
	}

	// 转移备案->转移备案查询
	public function transfer_record_query() {
		$record = M( 'record' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->getField( 'record_id,record_code,record_date,record_status' );
		$record_json = json_encode( $record );

		$unit_name = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->getField( 'production_unit_name' );
		$unit_json = json_encode( $unit_name );

		$tmp_content=$this->fetch( './Public/html/Content/Production/record/transfer_record_query.html' );
		$tmp_content = "<script>record_json = $record_json; unit_json = $unit_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 转移备案->转移备案查询->详细信息页
	public function transfer_record_query_detail($record_id="") {
		$record = M( 'record' )->where( array( 'record_id' =>$record_id ) )->find();
		$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->find();
		$this->record = $record;
		$this->unit = $production_unit;

		$tmp_content=$this->fetch( './Public/html/Content/Production/record/transfer_record_query_detail.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 转移备案->转移备案查询->修改信息页
	public function transfer_record_query_modify($record_id="") {
		$record = M( 'record' )->where( array( 'record_id' =>$record_id ) )->find();
		$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->find();
		$this->record = $record;
		$this->unit = $production_unit;

		$record_id_json = json_encode( $record_id );
		$record_status_json = json_encode( $record['record_status'] );

		$tmp_content=$this->fetch( './Public/html/Content/Production/record/transfer_record_query_modify.html' );
		$tmp_content = "<script>record_id_json = $record_id_json; record_status_json = $record_status_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 转移备案->转移备案查询->修改提交
	public function transfer_record_query_modified($record_id="") {
		$record = M( 'record' ); // 实例化record对象
		$record->create(); // 根据表单提交的POST数据创建数据对象
		$record->record_id = $record_id;
		$time = date( 'Y-m-d H:i:s', time() );
		$record->record_modify_time = $time;
		$record_status_old = I( 'post.record_status_old' );
		switch ( $record_status_old ) {
		case '0':
			$record_status = 0;
			break;
		case '3':
			$record_status = 4;
			break;
		case '4':
			$record_status = 4;
			break;
		default:
			$record_status = -1;
			break;
		}
		$record->record_status = $record_status;
		$result = $record->save(); // 根据条件保存修改的数据

		if ( $result ) {
			$this->ajaxReturn( 1, '修改成功！', 1 );
		} else {
			$this->ajaxReturn( 0, '修改失败！', 0 );
		}
	}

	// 转移备案->转移备案查询->提交备案
	public function transfer_record_query_submit($record_id="") {
		$record = M( 'record' )->where( array( 'record_id' =>$record_id ) )->find();
		$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->find();
		$this->record = $record;
		$this->unit = $production_unit;

		$record_id_json = json_encode( $record_id );
		$record_status_json = json_encode( $record['record_status'] );

		$tmp_content=$this->fetch( './Public/html/Content/Production/record/transfer_record_query_submit.html' );
		$tmp_content = "<script>record_id_json = $record_id_json; record_status_json = $record_status_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 转移备案->转移备案查询->提交备案
	public function transfer_record_query_submited($record_id="") {
		$record_status_old = I( 'post.record_status_old' );
		switch ( $record_status_old ) {
		case '0':
			$record_status = 1;
			break;
		case '4':
			$record_status = 1;
			break;
		default:
			$record_status = -1;
			break;
		}
		$record = M( 'record' ); // 实例化record对象
		$data['record_id'] = $record_id;
		$data['record_status'] = $record_status;
		$record->save( $data );
	}

}
?>
