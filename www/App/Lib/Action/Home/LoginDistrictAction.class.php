<?php
/**
 *
 */
class LoginDistrictAction extends CommonAction{
	public function district() {
		if ( session( 'user_type' ) == 4 ) {
			$this->display();
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}
}

?>