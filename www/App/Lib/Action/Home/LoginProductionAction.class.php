<?php
/**
 *
 */
class LoginProductionAction extends CommonAction{
	public function production( $id="production_index", $record_id="" ) {
		if ( session( 'user_type' ) == 5 ) {
			switch ( $id ) {
				// -------- 生产企业->首页 --------
			case 'production_index':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Production/homepage/production_index.html' );
				break;

				// -------- 企业基本信息->侧边栏 --------
			case 'basic_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Production/basic/basic_sidebar.html' );
				break;
				// 企业基本信息->企业基本信息
			case 'production_basic_information':
				$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->find();
				$this->unit = $production_unit;
				$tmp_content=$this->fetch( './Public/html/Content/Production/basic/production_basic_information.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 企业基本信息->基本信息变更
			case 'production_change_information':
				$production_unit = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->find();
				$this->unit = $production_unit;
				$tmp_content=$this->fetch( './Public/html/Content/Production/basic/production_change_information.html' );
				$this->ajaxReturn( $tmp_content );
				break;

				// -------- 危废库存->侧边栏 --------
			case 'warehouse_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Production/warehouse/warehouse_sidebar.html' );
				break;
				// 危废库存->危废入库管理
			case 'storage_input_management':
				$tmp_content=$this->fetch( './Public/html/Content/Production/warehouse/storage_input_management.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废库存->危废在库查询
			case 'storage_query':
				$tmp_content=$this->fetch( './Public/html/Content/Production/warehouse/storage_query.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废库存->危废出库管理
			case 'storage_output_management':
				$tmp_content=$this->fetch( './Public/html/Content/Production/warehouse/storage_output_management.html' );
				$this->ajaxReturn( $tmp_content );
				break;

				// -------- 转移备案->侧边栏 --------
			case 'record_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Production/record/record_sidebar.html' );
				break;
				// 转移备案->转移备案申请
			case 'transfer_record_request':
				$unit = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->find();
				$this->unit = $unit;
				$tmp_content=$this->fetch( './Public/html/Content/Production/record/transfer_record_request.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 转移备案->转移备案申请->表单提交
			case 'transfer_record_request_form':
				$record = M( 'record' ); //实例化record对象
				$record->create(); // 根据表单提交的POST数据创建数据对象
				$time = date( 'Y-m-d H:i:s', time() );
				$record->record_add_time = $time;
				$record->record_modify_time = $time;

				$record->production_unit_id = session( 'production_unit_id' );
				$record->record_code = session( 'production_unit_id' ) . '-' . date( 'Y-m' ) . '-' . ( M( 'record' )->max( 'record_id' )+1 );
				$record->record_status = I('post.record_status');
				$result = $record->add(); // 根据条件保存修改的数据

				$this->ajaxReturn();

				break;
				// 转移备案->转移备案查询
			case 'transfer_record_query':
				$record = M( 'record' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->getField( 'record_id,record_code,record_date,record_status' );
				$record_json = json_encode( $record );

				$unit_name = M( 'production_unit' )->where( array( 'production_unit_id' => session( 'production_unit_id' ) ) )->getField( 'production_unit_name' );
				$unit_json = json_encode( $unit_name );

				$tmp_content=$this->fetch( './Public/html/Content/Production/record/transfer_record_query.html' );
				$tmp_content = "<script>record_json = $record_json; unit_json = $unit_json</script> $tmp_content";
				$this->ajaxReturn( $tmp_content );
				break;
				// 转移备案->转移备案查询->详细信息页
			case 'transfer_record_query_page':
				$record = M( 'record' )->where( array('record_id' =>$record_id))->find();
				$production_unit = M( 'production_unit' )->where( array('production_unit_id' => $record['production_unit_id']))->find();
				$this->record = $record;
				$this->unit = $production_unit;
				$tmp_content=$this->fetch( './Public/html/Content/Production/record/transfer_record_query_page.html' );
				$this->ajaxReturn( $tmp_content );

				// -------- 转移联单->侧边栏 --------
			case 'manifest_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Production/manifest/manifest_sidebar.html' );
				break;
				// 转移联单->转移联单申请
			case 'transfer_manifest_request':
				$tmp_content=$this->fetch( './Public/html/Content/Production/manifest/transfer_manifest_request.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 转移联单->转移联单查询
			case 'transfer_manifest_query':
				$tmp_content=$this->fetch( './Public/html/Content/Production/manifest/transfer_manifest_query.html' );
				$this->ajaxReturn( $tmp_content );
				break;

			default:
				$this->error( "页面不存在！" );
				break;
			}

		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}
}

?>