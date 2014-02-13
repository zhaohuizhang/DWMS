<?php
//通用的控制器
class CommonAction extends Action{
	//自动执行的方法，判断是否通过登录跳转
	public function _initialize(){
		if (!isset($_SESSION['user_id']) || !isset($_SESSION['user_type']) || !isset($_SESSION['username'])){
			$this->redirect('Home/Index/index');
		}
	}
}
?>