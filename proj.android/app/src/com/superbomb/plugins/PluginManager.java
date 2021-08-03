package com.superbomb.plugins;

import android.app.Activity;
import android.content.Intent;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.util.ArrayList;

public class PluginManager {

    private static PluginManager instance;
    public static PluginManager getInstance() {
        if( instance == null ) {
            instance = new PluginManager();
        }

        return instance;
    }

    private Activity context;
    private final ArrayList<PluginListener> listeners;

    private PluginManager() {
        listeners = new ArrayList<>();
    }

    public static void init(Cocos2dxActivity context) {

        PluginManager mgr = getInstance();
        mgr.context = context;
        // mgr.listeners.add(Analytics.getInstance());
        // mgr.listeners.add(GameHelper.getInstance());
        // mgr.listeners.add(AdsManager.getInstance());
        // mgr.listeners.add(IAPHelper.getInstance());

        for( PluginListener listener : mgr.listeners ) {
            listener.init(context);
        }
    }

    public static void onStart() {

        PluginManager mgr = getInstance();

        for( PluginListener listener : mgr.listeners ) {
            listener.onStart();
        }
    }

    public static void onStop() {

        PluginManager mgr = getInstance();

        for( PluginListener listener : mgr.listeners ) {
            listener.onStop();
        }
    }

    public static void onResume() {

        PluginManager mgr = getInstance();

        for( PluginListener listener : mgr.listeners ) {
            listener.onResume();
        }
    }

    public static void onPause() {

        PluginManager mgr = getInstance();

        for( PluginListener listener : mgr.listeners ) {
            listener.onPause();
        }
    }

    public static void onDestroy() {

        PluginManager mgr = getInstance();

        for( PluginListener listener : mgr.listeners ) {
            listener.onDestroy();
        }
    }

    public static boolean onBackPressed() {

        PluginManager mgr = getInstance();
        boolean result = false;

        for( PluginListener listener : mgr.listeners ) {
            if( listener.onBackPressed() ) {
                result = true;
            }
        }

        return result;
    }

    public static boolean onActivityResult(int requestCode, int resultCode, Intent data) {

        PluginManager mgr = getInstance();
        boolean result = false;

        for( PluginListener listener : mgr.listeners ) {
            if( listener.onActivityResult(requestCode, resultCode, data) ) {
                result = true;
            }
        }

        return result;
    }
}
