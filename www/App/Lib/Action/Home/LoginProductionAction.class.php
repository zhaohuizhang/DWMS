<?php
/**
 *
 */
class LoginProductionAction extends CommonAction{
	public function homepage() {
		if ( session( 'user_type' ) == 5 ) {
			layout( './Common/frame' );
			$this->display( './Public/html/Content/Production/homepage/production_index.html' );
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}
}

?>