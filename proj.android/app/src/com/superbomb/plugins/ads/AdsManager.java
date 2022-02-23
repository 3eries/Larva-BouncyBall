package com.superbomb.plugins.ads;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;

import com.google.ads.mediation.admob.AdMobAdapter;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.FullScreenContentCallback;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.OnUserEarnedRewardListener;
import com.google.android.gms.ads.initialization.InitializationStatus;
import com.google.android.gms.ads.initialization.OnInitializationCompleteListener;
import com.google.android.gms.ads.interstitial.InterstitialAd;
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback;
import com.google.android.gms.ads.rewarded.RewardItem;
import com.google.android.gms.ads.rewarded.RewardedAd;
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;
import com.superbomb.math.Size;
import com.superbomb.plugins.PluginListener;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import java.util.ArrayList;

public class AdsManager implements PluginListener {
    private static AdsManager instance = null;

    public static AdsManager getInstance() {
        if( instance == null ) {
            instance = new AdsManager();
        }

        return instance;
    }

    private static final String TAG = "AdsManager";

    private static final AdSize BANNER_SIZE = new AdSize(AdSize.FULL_WIDTH, 50);

    private Activity context;

    private AdView bannerView;
    private Size bannerSizeInPixels;

    private String interstitialUnitId = "";
    private InterstitialAd interstitialAd;
    private boolean isInterstitialLoading;

    private String rewardedVideoUnitId = "";
    private RewardedAd rewardedVideoAd;
    private boolean isRewardedLoading;

    private AdsManager() {
    }

    @Override
    public void init(Cocos2dxActivity context) {
        this.context = context;
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
        if( bannerView != null ) bannerView.resume();
    }

    @Override
    public void onPause() {
        if( bannerView != null ) bannerView.pause();
    }

    @Override
    public void onDestroy() {
        if( bannerView != null ) bannerView.destroy();
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    @Override
    public boolean onActivityResult(int requestCode, int resultCode, Intent data) {
        return false;
    }

    /**
     * AdRequest.Builder 생성
     */
    private static AdRequest.Builder createRequestBuilder() {

        return new AdRequest.Builder();
    }

    /**
     * 광고 모듈 초기화
     */
    public static void initAd(final String appId,
                              final String bannerUnitId,
                              final String interstitialUnitId,
                              final String rewardedVideoUnitId) {

        Log.i(TAG, "initAd appId: " + appId);

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                final AdsManager mgr = instance;
                final Activity ctx = mgr.context;

                MobileAds.initialize(ctx, new OnInitializationCompleteListener() {
                    @Override
                    public void onInitializationComplete(InitializationStatus initializationStatus) {
                    }
                });

                mgr.initBanner(bannerUnitId);
                mgr.initInterstitial(interstitialUnitId);
                mgr.initRewardedVideo(rewardedVideoUnitId);
            }
        });
    }

    /**
     * 배너 광고 초기화
     */
    private void initBanner(String unitId) {

        Log.i(TAG, "initBanner unitId: " + unitId);

        if( TextUtils.isEmpty(unitId) ) {
            return;
        }

        bannerView = new AdView(context);
        bannerView.setAdUnitId(unitId);
        bannerView.setAdSize(BANNER_SIZE);
        bannerView.setVisibility(View.GONE);

        bannerSizeInPixels = new Size();
        bannerSizeInPixels.setWidth(BANNER_SIZE.getWidthInPixels(context));
        bannerSizeInPixels.setHeight(BANNER_SIZE.getHeightInPixels(context));

        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        layoutParams.gravity = (Gravity.CENTER | Gravity.BOTTOM);

        context.addContentView(bannerView, layoutParams);

        bannerView.setAdListener(new AdListener() {

            @Override
            public void onAdLoaded() {
                Cocos2dxHelper.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        nativeBannerOnAdLoaded();
                    }
                });
            }

            @Override
            public void onAdFailedToLoad(final LoadAdError loadAdError) {
                Cocos2dxHelper.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        nativeBannerOnAdFailedToLoad(loadAdError.getCode());
                    }
                });
            }

            @Override
            public void onAdOpened() {
                Cocos2dxHelper.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        nativeBannerOnAdOpened();
                    }
                });
            }

            @Override
            public void onAdClosed() {
                Log.i(TAG, "banner onAdClosed");

                Cocos2dxHelper.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        nativeBannerOnAdClosed();
                    }
                });

                loadBanner();
            }
        });

        loadBanner();
    }

    public static native void nativeBannerOnAdLoaded();

    public static native void nativeBannerOnAdFailedToLoad(int errorCode);

    public static native void nativeBannerOnAdOpened();

    public static native void nativeBannerOnAdClosed();

    /**
     * 전면 광고 초기화
     */
    private void initInterstitial(String unitId) {

        Log.i(TAG, "initInterstitial unitId: " + unitId);

        if( TextUtils.isEmpty(unitId) ) {
            return;
        }

        this.interstitialUnitId = unitId;

        loadInterstitial();
    }

    public static native void nativeInterstitialOnAdLoaded();

    public static native void nativeInterstitialOnAdFailedToLoad(int errorCode);

    public static native void nativeInterstitialOnAdOpened();

    public static native void nativeInterstitialOnAdClosed();

    /**
     * 보상형 비디오 초기화
     */
    private void initRewardedVideo(String unitId) {

        Log.i(TAG, "initRewardedVideo unitId: " + unitId);

        if( TextUtils.isEmpty(unitId) ) {
            return;
        }

        this.rewardedVideoUnitId = unitId;

        loadRewardedVideo();
    }

    public static native void nativeRewardedVideoOnAdLoaded();

    public static native void nativeRewardedVideoOnAdFailedToLoad(int errorCode);

    public static native void nativeRewardedVideoOnAdOpened();

    public static native void nativeRewardedVideoOnAdClosed();

    public static native void nativeRewardedVideoOnRewarded(String type, int rewardAmount);

    /**
     * 배너 로드
     */
    public static boolean loadBanner() {

        final AdsManager mgr = instance;
        final AdView bannerView = mgr.bannerView;

        if( bannerView == null ) {
            Log.i(TAG, "banner load error: banner is null.");
            return false;
        }

        if( bannerView.isLoading() ) {
            Log.i(TAG, "banner is loading.");
            return false;
        }

        runOnUiThread(new Runnable() {

            @Override
            public void run() {

                Log.i(TAG, "banner load");

                AdRequest adRequest = createRequestBuilder().build();
                bannerView.loadAd(adRequest);
            }
        });

        return true;
    }

    /**
     * 전면 광고 로드
     */
    public static boolean loadInterstitial() {

        Log.d(TAG, "loadInterstitial");

        final AdsManager mgr = instance;

        if( TextUtils.isEmpty(mgr.interstitialUnitId) ) {
            return false;
        }

        if( mgr.interstitialAd != null ) {
            Log.d(TAG, "Interstitial was loaded.");
            return false;
        }

        if( mgr.isInterstitialLoading ) {
            Log.d(TAG, "Interstitial was loading.");
            return false;
        }

        mgr.isInterstitialLoading = true;

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AdRequest adRequest = createRequestBuilder().build();

                InterstitialAd.load(mgr.context, mgr.interstitialUnitId, adRequest, new InterstitialAdLoadCallback() {
                    @Override
                    public void onAdLoaded(InterstitialAd interstitialAd) {
                        mgr.isInterstitialLoading = false;
                        mgr.interstitialAd = interstitialAd;

                        Cocos2dxHelper.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                nativeInterstitialOnAdLoaded();
                            }
                        });
                    }

                    @Override
                    public void onAdFailedToLoad(final LoadAdError loadAdError) {
                        mgr.isInterstitialLoading = false;
                        mgr.interstitialAd = null;

                        Cocos2dxHelper.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                nativeInterstitialOnAdFailedToLoad(loadAdError.getCode());
                            }
                        });

                    }
                });
            }
        });

        return true;
    }

    /**
     * 보상형 동영상 로드
     */
    public static boolean loadRewardedVideo() {

        Log.d(TAG, "loadRewardedVideo");

        final AdsManager mgr = instance;

        if( TextUtils.isEmpty(mgr.rewardedVideoUnitId) ) {
            return false;
        }

        if( mgr.rewardedVideoAd != null ) {
            Log.d(TAG, "Rewarded was loaded.");
            return false;
        }

        if( mgr.isRewardedLoading ) {
            Log.d(TAG, "Rewarded was loading.");
            return false;
        }

        mgr.isRewardedLoading = true;

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AdRequest adRequest = createRequestBuilder().build();

                RewardedAd.load(mgr.context, mgr.rewardedVideoUnitId, adRequest, new RewardedAdLoadCallback() {
                    @Override
                    public void onAdLoaded(RewardedAd rewardedAd) {
                        mgr.isRewardedLoading = false;
                        mgr.rewardedVideoAd = rewardedAd;

                        Cocos2dxHelper.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                nativeRewardedVideoOnAdLoaded();
                            }
                        });
                    }

                    @Override
                    public void onAdFailedToLoad(final LoadAdError loadAdError) {
                        mgr.isRewardedLoading = false;
                        mgr.rewardedVideoAd = null;

                        Cocos2dxHelper.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                nativeRewardedVideoOnAdFailedToLoad(loadAdError.getCode());
                            }
                        });
                    }
                });
            }
        });

        return true;
    }

    /**
     * 배너 광고 노출
     */
    public static void showBanner() {

        runOnUiThread(new Runnable() {

            @Override
            public void run() {

                final AdView bannerView = instance.bannerView;

                if( bannerView != null ) {
                    bannerView.setVisibility(View.VISIBLE);
                }

                /*
                // 광고 로딩됨, 광고 노출
                if( mgr.isBannerLoaded ) {
                    bannerView.setVisibility(View.VISIBLE);
                }
                // 광고 로딩
                else {
                    Log.d(TAG, "The banner wasn't loaded yet.");
                    loadBanner();
                }
               */
            }
        });
    }

    /**
     * 배너 광고 숨김
     */
    public static void hideBanner() {

        runOnUiThread(new Runnable() {

            @Override
            public void run() {

                final AdView bannerView = instance.bannerView;

                if( bannerView != null ) {
                    bannerView.setVisibility(View.GONE);
                }
            }
        });
    }

    /**
     * 전면 광고 노출
     */
    public static void showInterstitial() {

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                final AdsManager mgr = instance;

                if( mgr.interstitialAd == null ) {
                    Log.d(TAG, "showInterstitial ad wasn't ready yet.");
                    loadInterstitial();
                    return;
                }

                mgr.interstitialAd.setFullScreenContentCallback(new FullScreenContentCallback() {

                    @Override
                    public void onAdShowedFullScreenContent() {
                        mgr.interstitialAd = null;

                        Cocos2dxHelper.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                nativeInterstitialOnAdOpened();
                            }
                        });
                    }

                    @Override
                    public void onAdDismissedFullScreenContent() {
                        Cocos2dxHelper.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                nativeInterstitialOnAdClosed();
                            }
                        });

                        loadInterstitial();
                    }
                });

                mgr.interstitialAd.show(mgr.context);
            }
        });
    }

    /**
     * 보상형 비디오 노출
     */
    public static void showRewardedVideo() {

        runOnUiThread(new Runnable() {

            @Override
            public void run() {
                final AdsManager mgr = instance;

                if( mgr.rewardedVideoAd == null ) {
                    Log.d(TAG, "showRewardedVideo ad wasn't ready yet.");
                    loadRewardedVideo();
                    return;
                }

                mgr.rewardedVideoAd.setFullScreenContentCallback(new FullScreenContentCallback() {
                    @Override
                    public void onAdShowedFullScreenContent() {
                        mgr.rewardedVideoAd = null;

                        Cocos2dxHelper.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                nativeRewardedVideoOnAdOpened();
                            }
                        });
                    }

                    @Override
                    public void onAdDismissedFullScreenContent() {
                        Cocos2dxHelper.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                nativeRewardedVideoOnAdClosed();
                            }
                        });

                        loadRewardedVideo();
                    }
                });

                mgr.rewardedVideoAd.show(mgr.context, new OnUserEarnedRewardListener() {
                    @Override
                    public void onUserEarnedReward(final RewardItem rewardItem) {
                        Cocos2dxHelper.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                nativeRewardedVideoOnRewarded(rewardItem.getType(), rewardItem.getAmount());
                            }
                        });
                    }
                });
            }
        });
    }

    public static float getBannerWidth() {
        return instance.bannerSizeInPixels.getWidth();
    }

    public static float getBannerHeight() {
        return instance.bannerSizeInPixels.getHeight();
    }

    public static void runOnUiThread(Runnable r) {
        instance.context.runOnUiThread(r);
    }
}
