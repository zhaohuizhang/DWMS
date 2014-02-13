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
		//判断该用户是否被锁定
		if ( $user['lock'] ) {
			$this->error( '用户被锁定' );
		}
		//更新数据库中的上次登录时间和登录IP
		$logintime = date( 'Y-m-d H:i:s', time() );
		$data = array(
			'id' => $user['user_id'],
			'logintime' => $logintime,
			'loginip' => get_client_ip(),
		);
		M( 'user' )->save( $data );
		//存入session中
		session( 'uid', $user['user_id'] );
		session( 'username', $user['username'] );
		session( 'logintime', $logintime );
		session( 'loginip', $user['loginip'] );
		//重定向页面
		$this->redirect( 'Admin/Login/index/' );
	}

	public function verify() {
		//导入ThinkPHP扩展中的验证码库
		import( 'ORG.Util.Image' );
		//调用验证码生成函数，第一个参数表示验证码位数，第二个参数表示类型
		//第二个参数=5时为大小写+数字的混合模式，严格区分大小写
		Image::buildImageVerify( 1, 1, 'png' );
	}

}

?>
