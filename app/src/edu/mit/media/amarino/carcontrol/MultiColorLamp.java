/*
  MultiColorLamp - Example to use with Amarino
  Copyright (c) 2009 Bonifaz Kaufmann. 
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
package edu.mit.media.amarino.carcontrol;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import at.abraxas.amarino.Amarino;

public class MultiColorLamp extends Activity implements OnClickListener,OnTouchListener {
	
	//private static final String TAG = "MultiColorLamp";
	private static final String DEVICE_ADDRESS = "00:00:12:09:36:28";
	
	final int DELAY = 150;
	int red, green, blue;
	long lastChange;
	
	private Button btn_left;
	private Button btn_right;
	private Button btn_up;
	private Button btn_down;
	private TextView tv_show;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

		Amarino.connect(this, DEVICE_ADDRESS);
		
		findview();
		initListener();
		TouchListener();		
    }

    private void findview() {
    	btn_left = (Button) findViewById(R.id.btn_left);
    	btn_right = (Button) findViewById(R.id.btn_right);
    	btn_up = (Button) findViewById(R.id.btn_up);
    	btn_down = (Button) findViewById(R.id.btn_down);
    	tv_show = (TextView) findViewById(R.id.tv_show);
	}
    
    private void initListener() {
    	btn_left.setOnClickListener(this);
    	btn_up.setOnClickListener(this);
    	btn_down.setOnClickListener(this);
    	btn_right.setOnClickListener(this);
	}
    private void TouchListener() {
    	btn_left.setOnTouchListener(this);
    	btn_up.setOnTouchListener(this);
    	btn_down.setOnTouchListener(this);
    	btn_right.setOnTouchListener(this);
	}
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.btn_up:
			tv_show.setText("上");
			//Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 'w');
			break;
		case R.id.btn_down:
			tv_show.setText("下");
			//Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 's');
			break;
		case R.id.btn_left:
			tv_show.setText("左");
			//Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 'a');
			break;
		case R.id.btn_right:
			tv_show.setText("右");
			//Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 'd');
			break;

		default:
			break;
		}
	}

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		// TODO 自动生成的方法存根
		if(v.getId()==R.id.btn_up){
			if(event.getAction()==MotionEvent.ACTION_UP){
				Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 't');
			}
			if(event.getAction()==MotionEvent.ACTION_DOWN){
				Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 'w');
			}
		}
		
		if(v.getId()==R.id.btn_down){
			if(event.getAction()==MotionEvent.ACTION_UP){
				Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 't');
				//Toast.makeText(MainActivity.this, "第一个程序", Toast.LENGTH_SHORT).show();
				Toast.makeText(MultiColorLamp.this, "抬起", Toast.LENGTH_SHORT).show();
			}
			if(event.getAction()==MotionEvent.ACTION_DOWN){
				Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 's');
				Toast.makeText(MultiColorLamp.this, "按下", Toast.LENGTH_SHORT).show();
			}
		}
		
		if(v.getId()==R.id.btn_left){
			if(event.getAction()==MotionEvent.ACTION_UP){
				Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 't');
			}
			if(event.getAction()==MotionEvent.ACTION_DOWN){
				Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 'a');
			}
		}
		
		if(v.getId()==R.id.btn_right){
			if(event.getAction()==MotionEvent.ACTION_UP){
				Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 't');
			}
			if(event.getAction()==MotionEvent.ACTION_DOWN){
				Amarino.sendDataToArduino(this, DEVICE_ADDRESS, 'c', 'd');
			}
		}
		return false;
	}

	
	
}