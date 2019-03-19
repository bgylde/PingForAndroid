package com.sohu.opengl;

import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    private Button ping;
    private Button opengl;
    private TextView text;

    private static final byte PING_RESUT = 0x01;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        initListener();
    }

    private void initView() {
        ping = (Button) findViewById(R.id.ping);
        opengl = (Button) findViewById(R.id.opengl);
        text = (TextView) findViewById(R.id.text);
    }

    private void initListener() {
        ping.setOnClickListener(listener);
        opengl.setOnClickListener(listener);
    }

    private View.OnClickListener listener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.ping:
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            String res = NativeMethodHelper.ping("www.baidu.com");
                            Message msg = handler.obtainMessage();
                            msg.what = PING_RESUT;
                            msg.obj = res;
                            handler.sendMessage(msg);
                        }
                    }).start();

                    break;
                case R.id.opengl:
                    break;
            }
        }
    };

    private Handler handler = new Handler(Looper.getMainLooper(), new Handler.Callback() {
        @Override
        public boolean handleMessage(Message msg) {
            switch (msg.what) {
                case PING_RESUT:
                    text.setText((String)msg.obj);
                    break;
            }

            return false;
        }
    });
}
