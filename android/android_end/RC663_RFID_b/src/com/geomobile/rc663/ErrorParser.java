package com.geomobile.rc663;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class ErrorParser {
	public static void parse(ScanActivity activity, String value) {
		try {
			JSONObject jObject = new JSONObject(value);
			String errmsg = "出现错误\n";
			if(jObject.get("error") instanceof JSONArray) {
				JSONArray jArr = (JSONArray)jObject.get("error");
				for(int i = 0; i < jArr.length(); i++) {
					JSONObject jj = jArr.getJSONObject(i);
					errmsg += jj.getString("des");
					try {
						errmsg += ": " + jj.getString("rfid");
					} catch (JSONException e) {
						
					}
					
					errmsg += "\n";
				}
			} else {
				errmsg += ((JSONObject)(jObject.get("error"))).getString("des");
				try {
					errmsg += ": " + ((JSONObject)(jObject.get("error"))).getString("rfid");
				} catch (JSONException e) {
					
				}
			}
			activity.alertMessage(errmsg);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			if(value.contains("error")) {
				activity.alertMessage("未知错误" + value);
			} else {
				activity.alertMessage("上传成功" + value);
			}
		}
	}
}
