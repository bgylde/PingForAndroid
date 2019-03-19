package com.sohu.opengl;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

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
                            NativeMethodHelper.ping("www.qq.com");
                        }
                    }).start();

                    break;
                case R.id.opengl:
                    startOpenglActivity();
                    break;
            }
        }
    };

    private void startOpenglActivity() {
        Intent intent = new Intent(this, RendererActivity.class);
        startActivity(intent);
    }

    private static final String TAG = "PING";
    private void ping() {
        Process process = null;
        BufferedReader successReader = null;
        BufferedReader errorReader = null;

        DataOutputStream dos = null;
        try {
            process = Runtime.getRuntime().exec("traceroute www.baidu.com");
            // dos = new DataOutputStream(process.getOutputStream());
            Log.i(TAG, "====receive====:");

            // dos.write(command.getBytes());
            // dos.writeBytes("\n");
            // dos.flush();
            // dos.writeBytes("exit\n");
            // dos.flush();

            // status = process.waitFor();
            InputStream in = process.getInputStream();

            OutputStream out = process.getOutputStream();
            // success

            successReader = new BufferedReader(
                    new InputStreamReader(in));

            // error
            errorReader = new BufferedReader(new InputStreamReader(
                    process.getErrorStream()));

            String lineStr;

            while ((lineStr = successReader.readLine()) != null) {
                Log.i(TAG, "====receive====:" + lineStr);
//                Message msg = handler1.obtainMessage();
//                msg.obj = lineStr + "\r\n";
//                msg.what = 10;
//                msg.sendToTarget();
//                result = result + lineStr + "\n";
//                if (lineStr.contains("packet loss")) {
//                    Log.i(tag, "=====Message=====" + lineStr.toString());
//                    int i = lineStr.indexOf("received");
//                    int j = lineStr.indexOf("%");
//                    Log.i(tag,
//                            "====丢包率====:"
//                                    + lineStr.substring(i + 10, j + 1));//
//                    lost = lineStr.substring(i + 10, j + 1);
//                }
//                if (lineStr.contains("avg")) {
//                    int i = lineStr.indexOf("/", 20);
//                    int j = lineStr.indexOf(".", i);
//                    Log.i(tag,
//                            "====平均时延:===="
//                                    + lineStr.substring(i + 1, j));
//                    delay = lineStr.substring(i + 1, j);
//                    delay = delay + "ms";
//                }
                // tv_show.setText("丢包率:" + lost.toString() + "\n" +
                // "平均时延:"
                // + delay.toString() + "\n" + "IP地址:");// +
                // getNetIpAddress()
                // + getLocalIPAdress() + "\n" + "MAC地址:" +
                // getLocalMacAddress() + getGateWay());
                //sleep(delaytime * 1000);
            }
            // tv_show.setText(result);

            while ((lineStr = errorReader.readLine()) != null) {
                Log.i(TAG, "==error======" + lineStr);
                // tv_show.setText(lineStr);
            }

        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (dos != null) {
                    dos.close();
                }
                if (successReader != null) {
                    successReader.close();
                }
                if (errorReader != null) {
                    errorReader.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }

            if (process != null) {
                process.destroy();
            }
        }
    }

    private void socket() {
        try {
            Socket socket = new Socket("www.baidu.com", 443);
            OutputStream os = socket.getOutputStream();
            PrintWriter pw = new PrintWriter(os);
            pw.write("name: admin; pwd: admin");
            pw.flush();
            socket.shutdownOutput();

            InputStream is = socket.getInputStream();
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            String info = null;
            while ((info = br.readLine()) != null) {
                Log.d(TAG, info);
                System.out.println("Hello, I am Client, Server say: " + info);
            }

            br.close();
            is.close();
            pw.close();
            os.close();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
