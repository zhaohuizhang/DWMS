package com.geomobile.rc663;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.util.Log;

public class ScanActivity extends Activity {
	@Override
	public void onBackPressed() {
	    this.finish();
	}
	
	public void alertMessage(String msg)
    {
    	Log.d("ALERT", "alert");
    	AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("提示")
        .setMessage(msg)
        .setCancelable(false)
        .setNegativeButton("确定",new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                dialog.cancel();
            }
        });
        builder.create().show();
    }
}
