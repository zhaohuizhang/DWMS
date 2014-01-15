<?php
/**
 *
 */
class RegisterAction extends Action{
	public function index($id=0){
		
switch ($id) {				
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
}
?>