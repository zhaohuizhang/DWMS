<?php
/**
 *
 */
class LoginDistrictAction extends CommonAction{
	public function homepage() {
		if ( session( 'user_type' ) == 4 ) {
			layout( './Common/frame' );
			$this->display( './Public/html/Content/District/homepage/district_index.html' );
		}else {
			$this->redirect( 'Home/Index/index' );
		}
	}
}

?>