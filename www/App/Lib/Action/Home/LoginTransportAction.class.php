<?php
/**
 *
 */
class LoginTransportAction extends CommonAction{
	public function transport() {
		if ( session( 'user_type' ) == 6 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}
}

?>