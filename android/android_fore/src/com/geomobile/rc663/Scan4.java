package com.geomobile.rc663;

import android.os.Bundle;

public class Scan4 extends Scan3 {

	@Override
    public void onCreate(Bundle savedInstanceState) {
		this.myTitle = "入库扫描";
		super.onCreate(savedInstanceState);
		this.myURL = getString(R.string.url_prefix) + "wasteIn";
	}
}
