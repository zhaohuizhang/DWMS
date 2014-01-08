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

	public function country() {
		if ( session( 'user_type' ) == 1 ) {
			$this->display();
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

	public function production() {
		if ( session( 'user_type' ) == 5 ) {
			$this->display();
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
