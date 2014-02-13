<?php
/**
 *
 */
class DistrictBusinessAction extends CommonAction{
	// -------- 业务办理->侧边栏 --------
	public function business_sidebar() {
		layout( './Common/frame' );
		$this->display( './Public/html/Content/District/business/business_sidebar.html' );
	}

	// 业务办理->待办业务->转移备案管理
	public function transfer_record_management() {
		$record = M( 'record' )->where( 'record_status>0' )->getField( 'record_id,production_unit_id,record_code,record_date,record_status' );
		$record_json = json_encode( $record );

		$unit_name = M( 'production_unit' )->getField( 'production_unit_id, production_unit_name' );
		$unit_name_json = json_encode( $unit_name );

		if ( $record_json == null ) {
			$this->ajaxReturn( "备案表没有找到：" .  $record_json );
		} else if ( $unit_name_json == null ) {
				$this->ajaxReturn( "生产单位表没有找到：" .  $record_json );
			} else {
			$tmp_content=$this->fetch( './Public/html/Content/District/business/transfer_record_management.html' );
			$tmp_content = "<script>record_json = $record_json; unit_name_json = $unit_name_json; </script> $tmp_content";
			$this->ajaxReturn( $tmp_content );
		}
	}

	// 业务办理->待办业务->转移备案管理：详细信息页
	public function transfer_record_management_page() {
		$record = M( 'record' )->where( array( 'record_id' =>$record_id ) )->find();
		$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => $record['production_unit_id'] ) )->find();
		$this->record = $record;
		$this->unit = $production_unit;

		$record_id_json = json_encode( $record_id );
		$record_status_json = json_encode( $record['record_status'] );

		$tmp_content=$this->fetch( './Public/html/Content/District/business/transfer_record_management_page.html' );
		$tmp_content = "<script>record_id_json = $record_id_json; record_status_json = $record_status_json; </script> $tmp_content";
		$this->ajaxReturn( $tmp_content );
	}

	// 业务办理->待办业务->转移备案管理：审核
	public function transfer_record_management_audit() {
		$record_status = I( 'post.record_status' );
		$current_record_status = array(
			'record_id' => $record_id,
			'record_status' => $record_status,
		);
		$result = M( 'record' )->save( $current_record_status );
		if ( $result ) {
			$this->ajaxReturn( 1, '审核成功！', 1 );
		} else {
			$this->ajaxReturn( 0, '审核失败！', 0 );
		}
	}

	// 业务办理->待办业务->转移联单管理
	public function transfer_manifest_management() {
		$tmp_content=$this->fetch( './Public/html/Content/District/business/transfer_manifest_management.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 业务办理->待办业务->企业用户管理
	public function enterprise_user_management() {
		$record = M( 'alluser' )->select();
		$record_json = json_encode( $record );
		$tmp_content=$this->fetch( './Public/html/Content/District/business/enterprise_user_management.html' );

		$tmp_content="<script>record_json=$record_json; </script> $tmp_content";
		$this->ajaxReturn( "$tmp_content" );
	}

	// 业务办理->待办业务->企业用户管理：生产企业
	public function enterprise_user_management_page_production($record_id="") {
		$production_unit = M( 'production_unit' )->where( array( 'user_id' => $record_id ) )->find();
		$this->formData = $production_unit;
		$tmp_content=$this->fetch( './Public/html/Content/District/business/enterprise_user_management_page_production.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 业务办理->待办业务->企业用户管理：运输企业
	public function enterprise_user_management_page_transport($record_id="") {
		$transport_unit = M( 'transport_unit' )->where( array( 'user_id' => $record_id ) )->find();
		$this->formData = $transport_unit;
		$tmp_content=$this->fetch( './Public/html/Content/District/business/enterprise_user_management_page_transport.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 业务办理->待办业务->企业用户管理：接受企业
	public function enterprise_user_management_page_reception($record_id="") {
		$reception_unit = M( 'reception_unit' )->where( array( 'user_id' => $record_id ) )->find();
		$this->formData = $reception_unit;
		$tmp_content=$this->fetch( './Public/html/Content/District/business/enterprise_user_management_page_reception.html' );
		$this->ajaxReturn( $tmp_content );
	}

	//
	public function enterprise_user_management_ajaxpost() {
		$munit=M( 'user' );

		if ( I( 'post.action' )=="lock" ) {
			if ( I( 'post.value' )=='0' )
				$data['lock'] = '0';
			else
				$data['lock'] = '1';
			$munit->where( array( 'user_id' =>I( 'post.user_id' ) ) )->save( $data );
			$this->show( "lock_ok".I( 'post.user_id' ) );
		}
		else if ( I( 'post.action' )=="verify" ) {
				if ( I( 'post.value' )=='0' )
					$data['is_verify'] = '0';
				else
					$data['is_verify'] = '1';

				$munit->where( array( 'user_id' =>I( 'post.user_id' ) ) )->save( $data );
				$this->show( "verify_ok".I( 'post.user_id' ) );
			}

		else {
			$this->error( "action_error" );
		}
	}

	// 业务办理->待办业务->企业信息管理
	public function enterprise_information_management(){
		$tmp_content=$this->fetch( './Public/html/Content/District/business/enterprise_information_management.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 业务办理->业务查询->转移备案查询
	public function transfer_record_query(){
		$tmp_content=$this->fetch( './Public/html/Content/District/business/transfer_record_query.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 业务办理->业务查询->转移联单查询
	public function transfer_manifest_query(){
		$tmp_content=$this->fetch( './Public/html/Content/District/business/transfer_manifest_query.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 业务办理->业务查询->企业信息查询
	public function enterprise_information_query(){
		$tmp_content=$this->fetch( './Public/html/Content/District/business/enterprise_information_query.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 业务办理->业务查询->用户信息查询
	public function user_information_query(){
		$tmp_content=$this->fetch( './Public/html/Content/District/business/user_information_query.html' );
		$this->ajaxReturn( $tmp_content );
	}

}
?>
