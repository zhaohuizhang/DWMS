<?php
/**
 * 登录后的主页控制器
 */
class LoginAction extends CommonAction{
	public function index() {
		if ( session( 'user_type' ) == 0 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}

	public function country( $id="country_index" ) {
		if ( session( 'user_type' ) == 1 ) {
			switch ( $id ) {
				// 国家首页
			case 'country_index':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Government/homepage/country_index.html' );
				break;

				// 转移地图展示 侧边栏
			case 'GIS_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Government/GIS/GIS_sidebar.html' );
				break;
			case 'ajax':
				$GIS_data = M( 'gps_308033501795' )->field( 'bmap_longitude, bmap_latitude' )->where( 'longitude > 0' )->select();
				$this->ajaxReturn( $GIS_data, 'JSON' );
				/*	// 转移地图展示
			case 'transfer_display':
				$tmp_content=$this->fetch( './Public/html/Content/Government/GIS/transfer_display.html' );
				$this->ajaxReturn( $tmp_content );
				break;*/

				// 危废产生单位 侧边栏
			case 'production_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Government/production/production_sidebar.html' );
				break;
				// 危废产生单位企业基本信息
			case 'production_basic_information':
				$tmp_content=$this->fetch( './Public/html/Content/Government/production/basic_information.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废产生单位危险废物台账
			case 'production_waste_account':
				$tmp_content=$this->fetch( './Public/html/Content/Government/production/waste_account.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废产生单位RFID手持端绑定
			case 'production_rfid_hand_equipment':
				$tmp_content=$this->fetch( './Public/html/Content/Government/production/rfid_hand_equipment.html' );
				$this->ajaxReturn( $tmp_content );
				break;

				// 危废运输单位 侧边栏
			case 'transport_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Government/transport/transport_sidebar.html' );
				break;
				// 危废运输单位企业基本信息
			case 'transport_basic_information':
				$tmp_content=$this->fetch( './Public/html/Content/Government/transport/basic_information.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废运输单位运输车辆管理
			case 'transport_vehicle_management':
				$tmp_content=$this->fetch( './Public/html/Content/Government/transport/vehicle_management.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废运输单位GPS监控信息
			case 'transport_gps_monitor_information':
				$tmp_content=$this->fetch( './Public/html/Content/Government/transport/gps_monitor_information.html' );
				$this->ajaxReturn( $tmp_content );
				break;

				// 危废处置单位 侧边栏
			case 'disposal_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Government/disposal/disposal_sidebar.html' );
				break;
				// 危废处置单位企业基本信息
			case 'disposal_basic_information':
				$tmp_content=$this->fetch( './Public/html/Content/Government/disposal/basic_information.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废处置单位危废接受台账
			case 'disposal_waste_reception_account':
				$tmp_content=$this->fetch( './Public/html/Content/Government/disposal/waste_reception_account.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废处置单位企业基本信息
			case 'disposal_rfid_hand_equipment':
				$tmp_content=$this->fetch( './Public/html/Content/Government/disposal/rfid_hand_equipment.html' );
				$this->ajaxReturn( $tmp_content );
				break;

				// 危废转移 侧边栏
			case 'transfer_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Government/transfer/transfer_sidebar.html' );
				break;
				// 危废转移 转移联单管理
			case 'transfer_manifest_management':
				$tmp_content=$this->fetch( './Public/html/Content/Government/transfer/transfer_manifest_management.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废转移 转移备案管理
			case 'transfer_record_management':
				$tmp_content=$this->fetch( './Public/html/Content/Government/transfer/transfer_record_management.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废转移 转移统计分析
			case 'transfer_statistic_analysis':
				$tmp_content=$this->fetch( './Public/html/Content/Government/transfer/transfer_statistic_analysis.html' );
				$this->ajaxReturn( $tmp_content );
				break;

			default:
				$this->error( '页面不存在' );
				break;
			}
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}

	public function province() {
		if ( session( 'user_type' ) == 2 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}

	public function city() {
		if ( session( 'user_type' ) == 3 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}

	public function district() {
		if ( session( 'user_type' ) == 4 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}

	public function production( $id="production_index" ) {
		if ( session( 'user_type' ) == 5 ) {
			switch ( $id ) {
				// 首页
			case 'production_index':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Production/homepage/production_index.html' );
				break;
				// 企业基本信息 侧边栏
			case 'basic_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Production/basic/basic_sidebar.html' );
				break;
				// 企业基本信息
			case 'basic_information':
				$production_unit = M( 'production_unit' )->where( array( 'user_id' => session( 'user_id' ) ) )->find();
				$this->unit = $production_unit;
				$tmp_content=$this->fetch( './Public/html/Content/Production/basic/basic_information.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 基本信息变更
			case 'change_information':
				$production_unit = M( 'production_unit' )->where( array( 'user_id' => session( 'user_id' ) ) )->find();
				$this->unit = $production_unit;
				$tmp_content=$this->fetch( './Public/html/Content/Production/basic/change_information.html' );
				$this->ajaxReturn( $tmp_content );
				break;
				// 危废库存管理 侧边栏
			case 'warehouse_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Production/warehouse/warehouse_sidebar.html' );
				break;
				// 危废入库管理
			case 'storage_input_management':
				break;
				// 危废在库查询
			case 'storage_query':
				break;
				// 危废出库管理
			case 'storage_output_management':
				break;
			default:
				$this->error( "页面不存在！" );
				break;
			}

		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}

	public function transport() {
		if ( session( 'user_type' ) == 6 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}

	public function reception() {
		if ( session( 'user_type' ) == 7 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}

	public function logout() {
		$last_login = array(
			'user_id' => session( 'user_id' ),
			'last_login_time' => session( 'current_login_time' ),
			'last_login_ip' => session( 'current_login_ip' ),
		);
		M( 'user' )->save( $last_login );

		session_unset();
		session_destroy();
		$this->redirect( 'Home/Index/index' );
	}
}

?>
