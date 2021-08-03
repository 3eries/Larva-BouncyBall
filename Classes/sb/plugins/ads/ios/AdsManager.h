//
//  AdsManager.h
//
//  Created by seongmin hwang on 2018. 4. 10..
//

#ifndef AdsManager_h
#define AdsManager_h

#include "../../SBPluginConfig.h"

#if SB_PLUGIN_USE_ADS

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import <GoogleMobileAds/GoogleMobileAds.h>

#include "cocos2d.h"
#include "../AdListener.hpp"

@interface AdsManager : NSObject<GADBannerViewDelegate, GADFullScreenContentDelegate> {
    NSString *interstitialUnitId;
    NSString *rewardedUnitId;
}

@property(nonatomic, strong) GADBannerView* _Nullable bannerView;
@property(nonatomic, strong) GADInterstitialAd* _Nullable interstitialAd;
@property(nonatomic, strong) GADRewardedAd* _Nullable rewardedAd;

@property bool isInterstitialLoading;
@property bool isRewardedLoading;

+ (AdsManager*_Nonnull) getInstance;

- (void) initAd:(const superbomb::AdsConfig&)config;

- (bool) loadBanner;
- (bool) loadInterstitial;
- (bool) loadRewardedVideo;

- (void) showBanner;
- (void) hideBanner;

- (void) showInterstitial;
- (void) showRewardedVideo;

- (float) getBannerWidth;
- (float) getBannerHeight;

// GADBannerViewDelegate
//// Ad Request Lifecycle Notifications
- (void) bannerViewDidReceiveAd:(nonnull GADBannerView *)bannerView;
- (void) bannerView:(nonnull GADBannerView *)bannerView
didFailToReceiveAdWithError:(nonnull NSError *)error;
- (void) bannerViewDidRecordImpression:(nonnull GADBannerView *)bannerView;

//// Click-Time Lifecycle Notifications
- (void) bannerViewWillPresentScreen:(nonnull GADBannerView *)bannerView;
- (void) bannerViewWillDismissScreen:(nonnull GADBannerView *)bannerView;
- (void) bannerViewDidDismissScreen:(nonnull GADBannerView *)bannerView;

// GADFullScreenContentDelegate
- (void) adDidRecordImpression:(nonnull id<GADFullScreenPresentingAd>)ad;
- (void) ad:(nonnull id<GADFullScreenPresentingAd>)ad
didFailToPresentFullScreenContentWithError:(nonnull NSError *)error;

- (void) adDidPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad;
- (void) adWillDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad;
- (void) adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad;
@end

#endif // SB_PLUGIN_USE_ADS

#endif /* AdsManager_h */
