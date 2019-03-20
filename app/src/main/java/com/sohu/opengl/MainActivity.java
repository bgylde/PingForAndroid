package com.sohu.opengl;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.sohu.opengl.utils.DnsLookup;
import com.sohu.opengl.utils.LogUtils;

public class MainActivity extends AppCompatActivity {

    private final String TAG = "MainActivity";
    private Button ping;
    private Button dns_ping;
    private TextView text;
    private EditText domain_edit;

    private final byte PING_RESUT = 0x01;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        initListener();
    }

    private void initView() {
        ping = (Button) findViewById(R.id.ping);
        dns_ping = (Button) findViewById(R.id.dns_ping);
        text = (TextView) findViewById(R.id.text);
        domain_edit = (EditText) findViewById(R.id.domain);
    }

    private void initListener() {
        ping.setOnClickListener(listener);
        dns_ping.setOnClickListener(listener);
    }

    private View.OnClickListener listener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.ping:
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            String res = NativeMethodHelper.ping(domain_edit.getText().toString());
                            showResult(res);
                        }
                    }).start();
                    break;
                case R.id.dns_ping:
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            String ip = DnsLookup.resolveHostToIp(domain_edit.getText().toString(), 3000);
                            if (ip != null) {
                                String res = NativeMethodHelper.ping(ip);
                                showResult(res);
                            } else {
                                LogUtils.d(TAG, "dns resolve error.");
                            }
                        }
                    }).start();
                    break;
            }
        }
    };

    private void showResult(String result) {
        Message msg = handler.obtainMessage();
        msg.what = PING_RESUT;
        msg.obj = result;
        handler.sendMessage(msg);
    }

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
