package com.remote.yingwen.carsochet;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class LoadingActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_loading);
		new Thread(){

			@Override
			public void run() {
				try {
					sleep(2000);
					Intent intent = new Intent(LoadingActivity.this,MainActivity.class);
					startActivity(intent);
					LoadingActivity.this.finish();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			
		}.start();
	}
	
}
