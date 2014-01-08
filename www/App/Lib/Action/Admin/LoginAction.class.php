<?php
/**
 * 登录后的主页控制器
 */
class LoginAction extends CommonAction{
	public function index() {
		$this->display();
	}

	public function logout(){
		session_unset();
		session_destroy();
		$this->redirect('Admin/Index/index');
	}
}

?>
