<?php
/**
 *
 */
class ProductionManifestAction extends CommonAction{
	// -------- 转移联单->侧边栏 --------
	public function manifest_sidebar() {
		layout( './Common/frame' );
		$this->display( './Public/html/Content/Production/manifest/manifest_sidebar.html' );
	}

	// 转移联单->转移联单申请
	public function transfer_manifest_request() {
		$tmp_content=$this->fetch( './Public/html/Content/Production/manifest/transfer_manifest_request.html' );
		$this->ajaxReturn( $tmp_content );
	}

	// 转移联单->转移联单查询
	public function transfer_manifest_query() {
		$tmp_content=$this->fetch( './Public/html/Content/Production/manifest/transfer_manifest_query.html' );
		$this->ajaxReturn( $tmp_content );
	}
}
?>
