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

	public function country( $id="homepage" ) {

		if ( session( 'user_type' ) == 1 ) {
			switch ( $id ) {
			case 'homepage':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Government/homepage/country_index.html' );
				break;
			default:
				$this->error( '404' );
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
			case 'basic_sidebar':
				layout( './Common/frame' );
				$this->display( './Public/html/Content/Production/basic/basic_sidebar.html' );
				break;
			case 'basic_information':
				$production_unit = M( 'production_unit' )->where( array( 'user_id' => session( 'user_id' ) ) )->find();
				$this->unit = $production_unit;
				$tmp_content=$this->fetch( './Public/html/Content/Production/basic/basic_information.html' );
				$this->ajaxReturn( $tmp_content );
				break;
			case 'change_information':
				$production_unit = M( 'production_unit' )->where( array( 'user_id' => session( 'user_id' ) ) )->find();
				$this->unit = $production_unit;
				$tmp_content=$this->fetch( './Public/html/Content/Production/basic/change_information.html' );
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
