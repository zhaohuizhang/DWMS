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
