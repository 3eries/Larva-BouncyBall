package com.superbomb;

import android.app.AlertDialog;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

public class SBPlatformHelper {

    public static boolean isNetworkOnline() {
        ConnectivityManager connectivityManager = (ConnectivityManager)Cocos2dxActivity.getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo network = connectivityManager.getActiveNetworkInfo();

        if( network == null ) {
            return false;
        }

        return network.isConnected();
    }

    public static void showNetworkErrorPopup() {
        Cocos2dxHelper.getActivity().runOnUiThread(() -> {
            new AlertDialog.Builder(Cocos2dxActivity.getContext())
                .setCancelable(false)
                .setTitle("No internet Connetion")
                .setMessage("Please play to online")
                .setPositiveButton("OK", (dialog, which) -> {
                    nativeOnExitApp();
                })
                .show();
        });
    }

    public static native void nativeOnExitApp();
}
