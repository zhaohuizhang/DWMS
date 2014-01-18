<?php
/**
 * 这是后台首页的控制器
 */
class IndexAction extends Action{
	public function index() {
		$this->display();
	}

	public function login() {
		//防止直接输入路径登录页面
		if ( !IS_POST ) halt( "页面不存在" );
		//判断验证码是否错误
		if ( I( 'code', '', 'md5' ) != session( 'verify' ) ) {
			$this->error( '验证码错误' );
		}

		$username = I( 'username' );
		$password = I( 'password' , '', 'md5' );
		//查询数据库
		$user = M( 'user' )->where( array( 'username' => $username ) )->find();
		//判断用户名或密码是否错误
		if ( !$user || ( $user['password'] != $password ) ) {
			$this->error( '用户名或密码错误' );
		}
		//判断该用户是否审核通过
		if ( !$user['is_verify'] ) {
			$this->error( '您的审核还未通过，请耐心等待。' );
		}
		//判断该用户是否被锁定
		if ( $user['lock'] ) {
			$this->error( '用户被锁定' );
		}
		//更新数据库中的上次登录时间和登录IP
		$current_login_time = date( 'Y-m-d H:i:s', time() );
		$current_login = array(
			'user_id' => $user['user_id'],
			'current_login_time' => $current_login_time,
			'current_login_ip' => get_client_ip(),
		);
		M( 'user' )->save( $current_login );
		//存入session中
		session( 'user_id', $user['user_id'] );
		session( 'user_type', $user['user_type'] );
		session( 'username', $user['username'] );
		session( 'last_login_time', $user['last_login_time'] );
		session( 'current_login_time', $user['current_login_time'] );
		session( 'last_login_ip', $user['last_login_ip'] );
		session( 'current_login_ip', $user['current_login_ip'] );

		//重定向页面
		switch ( session( 'user_type' ) ) {
		case 0:
			$this->redirect( 'Home/Login/index' );
			break;
		case 1:
			$this->redirect( 'Home/Login/country' );
			break;
		case 2:
			$this->redirect( 'Home/Login/province' );
			break;
		case 3:
			$this->redirect( 'Home/Login/city' );
			break;
		case 4:
			$this->redirect( 'Home/Login/district' );
			break;
		case 5:
			$unit = M('production_unit')->where( array( 'user_id' => session( 'user_id' ) ) )->find();
			session('production_unit_id', $unit['production_unit_id']);

			$this->redirect( 'Home/Login/production' );
			break;
		case 6:
			$this->redirect( 'Home/Login/transport' );
			break;
		case 7:
			$this->redirect( 'Home/Login/reception' );
			break;
		default:
			$this->redirect( 'Home/Index/index' );
			break;
		}
	}

	public function verify() {
		//导入ThinkPHP扩展中的验证码库
		import( 'ORG.Util.Image' );
		//调用验证码生成函数，第一个参数表示验证码位数，第二个参数表示类型
		//第二个参数=5时为大小写+数字的混合模式，严格区分大小写
		Image::buildImageVerify( 1, 1, 'png' );
	}

	public function register() {

	}

}

?>
