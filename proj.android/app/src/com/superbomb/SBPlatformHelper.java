package com.superbomb;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;

import org.cocos2dx.lib.Cocos2dxActivity;

public class SBPlatformHelper {
    public static boolean isNetworkOnline() {
        ConnectivityManager connectivityManager = (ConnectivityManager)Cocos2dxActivity.getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo network = connectivityManager.getActiveNetworkInfo();

        if( network == null ) {
            return false;
        }

        return network.isConnected();
    }
}
