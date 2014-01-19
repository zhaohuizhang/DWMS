<?php
/**
 *
 */
class LoginReceptionAction extends CommonAction{
	public function reception() {
		if ( session( 'user_type' ) == 7 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}
}

?>