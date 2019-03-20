package com.sohu.ping.utils;

import java.net.InetAddress;
import java.net.UnknownHostException;

/**
 * Created by wangyan on 2019/3/20
 */
public class DnsLookup {

    private static final String TAG = "DnsLookup";

    public static String resolveHostToIp(String host, long timeout) {
        try {
            DnsRunnable dnsRunnable = new DnsRunnable(host);
            Thread thread = new Thread(dnsRunnable);
            thread.start();
            thread.join(timeout);
            return dnsRunnable.getIP();
        } catch (InterruptedException e) {
            LogUtils.e(TAG, e);
        } catch (Exception e) {
            LogUtils.e(TAG, e);
        }

        return null;
    }


    private static class DnsRunnable implements Runnable {
        private InetAddress addr;
        private String hostname;

        private DnsRunnable(String hostname) {
            this.hostname = hostname;
        }

        @Override
        public void run() {
            long startTime = System.currentTimeMillis();
            try {
                InetAddress add = InetAddress.getByName(hostname);
                set(add);
            } catch (UnknownHostException e) {
                LogUtils.e(TAG, e);
            }

            LogUtils.d(TAG, hostname + " cost time: " + (System.currentTimeMillis() - startTime));
        }

        private synchronized void set(InetAddress addr) {
            this.addr = addr;
        }

        private synchronized String getIP() {
            if (null != this.addr) {
                return addr.getHostAddress();
            }

            return null;
        }
    }
}
