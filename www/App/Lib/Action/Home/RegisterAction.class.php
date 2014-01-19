<?php
/**
 *
 */
class RegisterAction extends Action{
	public function index( $id="" ) {
		switch ( $id ) {

		case 'production':
			$this->display( './Register/register_production' );
			break;

		case 'transport':
			$this->display( './Register/register_transport' );
			break;

		case 'reception':
			$this->display( './Register/register_reception' );
			break;

		default:
			$this->error( '页面不存在' );
			break;
		}
	}

	public function do_reg( $id=0 ) {
		switch ( $id ) {
		case 'production':
			$postData=( I( 'post.' ) );
			$unit= M( 'production_unit' );
			$user=M( 'user' );

			$AccountData["user_type"]=5;
			$AccountData["username"]=$postData["username"];
			$AccountData["password"]=md5( $postData["password"] );
			$AccountData["email"]=$postData["email"];
			$AccountData["phone_num"]=$postData["phone_num"];
			$AccountData["add_time"]=date( 'Y-m-d H:i:s', time() );
			$AccountData["modify_time"]=date( 'Y-m-d H:i:s', time() );
			$AccountData["is_verify"]=0;
			$AccountData["lock"]=0;

			if ( $user_id=$user->add( $AccountData ) ) {
				$unit->create();
				$unit->user_id=$user_id;

				if ( $unit->add() ) {
					$this->success( '生产企业注册成功!', "../../../../../", 5 );
				}
				else {
					$user->where( 'user_id=$user_id' )->delete();
					$this->error( '账户创建失败:企业信息写入失败', "../../../../../", 5 );
				}
			}
			else {
				$this->error( '账户创建失败:账户信息写入失败', "../../../../../", 5 );
			}
			break;

		case 'transport':

			//$this->success( '运输企业注册成功!',"../../../../",5);
			$postData=( I( 'post.' ) );
			$unit= M( 'transport_unit' );
			$user=M( 'user' );

			$AccountData["user_type"]=6;
			$AccountData["username"]=$postData["username"];
			$AccountData["password"]=md5( $postData["password"] );
			$AccountData["email"]=$postData["email"];
			$AccountData["phone_num"]=$postData["phone_num"];
			$AccountData["add_time"]=date( 'Y-m-d H:i:s', time() );
			$AccountData["modify_time"]=date( 'Y-m-d H:i:s', time() );
			$AccountData["is_verify"]=0;
			$AccountData["lock"]=0;

			if ( $user_id=$user->add( $AccountData ) ) {
				$unit->create();
				$unit->user_id=$user_id;

				if ( $unit->add() ) {
					$this->success( '运输企业注册成功!', "../../../../../", 5 );
				}
				else {
					$user->where( 'user_id=$user_id' )->delete();
					$this->error( '账户创建失败:企业信息写入失败', "../../../../../", 5 );
				}
			}

			else {
				$this->error( '账户创建失败:账户信息写入失败', "../../../../../", 5 );
			}

			break;

		case 'reception':

			//$this->success( '接受企业注册成功!',"../../../../../",5);
			$postData=( I( 'post.' ) );
			$unit= M( 'reception_unit' );
			$user=M( 'user' );

			$AccountData["user_type"]=7;
			$AccountData["username"]=$postData["username"];
			$AccountData["password"]=md5( $postData["password"] );
			$AccountData["email"]=$postData["email"];
			$AccountData["phone_num"]=$postData["phone_num"];
			$AccountData["add_time"]=date( 'Y-m-d H:i:s', time() );
			$AccountData["modify_time"]=date( 'Y-m-d H:i:s', time() );
			$AccountData["is_verify"]=0;
			$AccountData["lock"]=0;

			if ( $user_id=$user->add( $AccountData ) ) {
				$unit->create();
				$unit->user_id=$user_id;

				if ( $unit->add() ) {
					$this->success( '接受企业注册成功!', "../../../../../", 5 );
				}
				else {
					$user->where( 'user_id=$user_id' )->delete();
					$this->error( '账户创建失败:企业信息写入失败', "../../../../../", 5 );
				}
			}

			else {
				$this->error( '账户创建失败:账户信息写入失败', "../../../../../", 5 );
			}
			break;

		default:
			$this->error( '页面不存在' );
			break;
		}
	}
}
?>
