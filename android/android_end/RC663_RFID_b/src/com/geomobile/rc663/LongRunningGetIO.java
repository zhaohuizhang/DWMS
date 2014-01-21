package com.geomobile.rc663;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;

import android.os.AsyncTask;
import android.util.Log;

public class LongRunningGetIO extends AsyncTask <Void, Void, String> {
	String url = "";
	IOCallback callback;
	List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
	public LongRunningGetIO(String url, List<NameValuePair> values, IOCallback callback) {
		Log.d("URL", url);
		if(values != null) this.nameValuePairs = values;
		this.url = url;
		this.callback = callback;
	}
	
	protected String getASCIIContentFromEntity(HttpEntity entity) throws IllegalStateException, IOException {
		InputStream in = entity.getContent();
		StringBuffer out = new StringBuffer();
		int n = 1;
		while (n>0) {
			byte[] b = new byte[4096];
			n =  in.read(b);
			if (n>0) out.append(new String(b, 0, n));
		}
		return out.toString();
	}
	@Override
	protected String doInBackground(Void... params) {
		HttpClient httpClient = new DefaultHttpClient();
		HttpContext localContext = new BasicHttpContext();
		HttpPost httpPost = new HttpPost(this.url);
		String text = null;
		try {
			httpPost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
			HttpResponse response = httpClient.execute(httpPost, localContext);
			HttpEntity entity = response.getEntity();
			text = getASCIIContentFromEntity(entity);
		} catch (Exception e) {
			return e.getLocalizedMessage();
		}
		return text;
	}


	protected void onPostExecute(String results) {
		if (results!=null) {
			//EditText et = (EditText)findViewById(R.id.my_edit);
			//TextView main_info = (TextView)findViewById(R.id.textView_15693_info);
			//main_info.setText(results);
			results = results.replaceAll("\ufeff", "");
			Log.d("RESULT", results);
			callback.httpRequestDidFinish(1, results);
		} else {
			callback.httpRequestDidFinish(0, "ERROR");
		}
	}
}