<?php
/**
 *
 */
class LoginProvinceAction extends CommonAction{
	public function province() {
		if ( session( 'user_type' ) == 2 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}
}

?>