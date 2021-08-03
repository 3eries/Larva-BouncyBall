//
//  AdsManager.m
//
//  Created by seongmin hwang on 2018. 4. 10..
//

#import "AdsManager.h"

#if SB_PLUGIN_USE_ADS

#import "AppController.h"
#import "RootViewController.h"

#include "../AdsHelper.hpp"
#include "../../../base/SBDirector.hpp"
#include "../../../platform/SBPlatformMacros.h"

USING_NS_CC;
USING_NS_SB;
using namespace std;

@interface AdsManager (Private)

- (id) init;
- (GADRequest*) createRequest;

- (void) initBanner:(const string&)unitId;
- (void) initInterstitial:(const string&)unitId;
- (void) initRewardedVideo:(const string&)unitId;

@end

@implementation AdsManager

+ (AdsManager*) getInstance {
    
    static AdsManager *instance = nil;
    static dispatch_once_t oncePredicate;
    
    dispatch_once(&oncePredicate, ^{
        instance = [[AdsManager alloc] init];
    });
    
    return instance;
}

- (id) init {
    
    if( self = [super init] ) {
        interstitialUnitId = nil;
        rewardedUnitId = nil;
        
        self.bannerView = nil;
        self.interstitialAd = nil;
        self.rewardedAd = nil;
        
        self.isInterstitialLoading = false;
        self.isRewardedLoading = false;
    }
    
    return self;
}

- (GADRequest*) createRequest {
    
    GADRequest *request = [GADRequest request];
    
    return request;
}

/**
 * 광고 모듈 초기화
 */
- (void) initAd:(const AdsConfig&)config {
    
    // Initialize the Google Mobile Ads SDK.
    // [GADMobileAds configureWithApplicationID:NS_STRING(config.appId.c_str())];
    [[GADMobileAds sharedInstance] startWithCompletionHandler:nil];
    
    // 광고 단위 초기화
    [self initBanner:config.bannerUnitId];
    [self initInterstitial:config.interstitialUnitId];
    [self initRewardedVideo:config.rewardedVideoUnitId];
}

#pragma mark- 배너

/**
 * 배너 광고 초기화
 */
- (void) initBanner:(const string&)unitId {
    
    NSLog(@"initBanner unitId: %s", unitId.c_str());
    
    if( unitId == "" ) {
        return;
    }
    
//    GADAdSize bannerSize =
//                    !SBDirector::isPadResolution() ? kGADAdSizeBanner : kGADAdSizeSmartBannerPortrait;
    GADAdSize bannerSize = kGADAdSizeSmartBannerPortrait;
    
    self.bannerView = [[GADBannerView alloc] initWithAdSize:bannerSize];
    self.bannerView.adUnitID = NS_STRING(unitId.c_str());
    self.bannerView.hidden = YES;
    self.bannerView.translatesAutoresizingMaskIntoConstraints = NO; // example
    self.bannerView.delegate = self;
    self.bannerView.rootViewController = ROOT_VIEW_CONTROLLER;
    
    UIView *view = ROOT_VIEW_CONTROLLER.view;
    [view addSubview:self.bannerView];
    
    // set frame
    /*
    CGRect frame = bannerView.frame;
    frame.origin.x = 0;
    frame.origin.y = view.frame.size.height - frame.size.height;
    
    bannerView.frame = frame;
    */
    
    // example
    if( @available(iOS 11.0, *) ) {
        // In iOS 11, we need to constrain the view to the safe area.
        // Position the banner. Stick it to the bottom of the Safe Area.
        // Make it constrained to the edges of the safe area.
        UILayoutGuide *guide = view.safeAreaLayoutGuide;
        
        // align screen bottom
        [NSLayoutConstraint activateConstraints:@[
                                                  [guide.leftAnchor constraintEqualToAnchor:self.bannerView.leftAnchor],
                                                  [guide.rightAnchor constraintEqualToAnchor:self.bannerView.rightAnchor],
                                                  [guide.bottomAnchor constraintEqualToAnchor:self.bannerView.bottomAnchor]
                                                  ]];
        // align screen top
        /*
        [NSLayoutConstraint activateConstraints:@[
                                                  [guide.leftAnchor constraintEqualToAnchor:self.bannerView.leftAnchor],
                                                  [guide.rightAnchor constraintEqualToAnchor:self.bannerView.rightAnchor],
                                                  [guide.topAnchor constraintEqualToAnchor:self.bannerView.topAnchor]
                                                  ]];
         */
        
    } else {
        // In lower iOS versions, safe area is not available so we use
        // bottom layout guide and view edges.
        [view addConstraint:[NSLayoutConstraint constraintWithItem:self.bannerView
                                                         attribute:NSLayoutAttributeLeading
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:view
                                                         attribute:NSLayoutAttributeLeading
                                                        multiplier:1
                                                          constant:0]];
        [view addConstraint:[NSLayoutConstraint constraintWithItem:self.bannerView
                                                         attribute:NSLayoutAttributeTrailing
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:view
                                                         attribute:NSLayoutAttributeTrailing
                                                        multiplier:1
                                                          constant:0]];
        // align screen bottom
        [view addConstraint:[NSLayoutConstraint constraintWithItem:self.bannerView
                                                         attribute:NSLayoutAttributeBottom
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:ROOT_VIEW_CONTROLLER.bottomLayoutGuide
                                                         attribute:NSLayoutAttributeTop
                                                        multiplier:1
                                                          constant:0]];
        
        // align screen top
        /*
        [view addConstraint:[NSLayoutConstraint constraintWithItem:self.bannerView
                                                         attribute:NSLayoutAttributeTop
                                                         relatedBy:NSLayoutRelationEqual
                                                            toItem:ROOT_VIEW_CONTROLLER.topLayoutGuide
                                                         attribute:NSLayoutAttributeBottom
                                                        multiplier:1
                                                          constant:0]];
         */
    }
    
    /*
    [view addConstraints:@[
                           [NSLayoutConstraint constraintWithItem:bannerView
                                                        attribute:NSLayoutAttributeBottom
                                                        relatedBy:NSLayoutRelationEqual
                                                           toItem:self.bottomLayoutGuide
                                                        attribute:NSLayoutAttributeTop
                                                       multiplier:1
                                                         constant:0],
                           [NSLayoutConstraint constraintWithItem:bannerView
                                                        attribute:NSLayoutAttributeCenterX
                                                        relatedBy:NSLayoutRelationEqual
                                                           toItem:view
                                                        attribute:NSLayoutAttributeCenterX
                                                       multiplier:1
                                                         constant:0]
                           ]];
    */
    
    // load
    [self loadBanner];
}

#pragma mark- GADBannerViewDelegate

/**
 * OnAdLoaded
 * Tells the delegate that an ad request successfully received an ad. The delegate may want to add
 * the banner view to the view hierarchy if it hasn't been added yet.
 */
- (void) bannerViewDidReceiveAd:(nonnull GADBannerView *)bannerView {
    
    AdsHelper::getInstance()->getBanner()->onAdLoaded();
}

/**
 * OnAdFailedToLoad
 * Tells the delegate that an ad request failed. The failure is normally due to network
 * connectivity or ad availablility (i.e., no fill).
 */
- (void) bannerView:(nonnull GADBannerView *)bannerView
didFailToReceiveAdWithError:(nonnull NSError *)error {
    
    NSLog(@"Failed to load banner ad with error: %@", [error localizedDescription]);

    int errorCode = [[NSNumber numberWithInteger:error.code] intValue];
    AdsHelper::getInstance()->getBanner()->onAdFailedToLoad(errorCode);
}

/**
 * Tells the delegate that an impression has been recorded for an ad.
 */
- (void) bannerViewDidRecordImpression:(nonnull GADBannerView *)bannerView {
    
    NSLog(@"bannerViewDidRecordImpression");
}

/**
 * OnAdOpened
 * Tells the delegate that a full screen view will be presented in response to the user clicking on
 * an ad. The delegate may want to pause animations and time sensitive interactions.
 */
- (void) bannerViewWillPresentScreen:(nonnull GADBannerView *)bannerView {
    NSLog(@"bannerViewWillPresentScreen");
    
    AdsHelper::getInstance()->getBanner()->onAdOpened();
}

/**
 * Tells the delegate that the full screen view will be dismissed.
 */
- (void) bannerViewWillDismissScreen:(nonnull GADBannerView *)bannerView {
    NSLog(@"bannerViewWillDismissScreen");
}

/**
 * OnAdClosed
 * Tells the delegate that the full screen view has been dismissed. The delegate should restart
 * anything paused while handling bannerViewWillPresentScreen:.
 */
- (void) bannerViewDidDismissScreen:(nonnull GADBannerView *)bannerView {
    NSLog(@"bannerViewDidDismissScreen");
    
    AdsHelper::getInstance()->getBanner()->onAdClosed();
    [self loadBanner];
}

#pragma mark- 전면

/**
 * 전면 광고 초기화
 */
- (void) initInterstitial:(const string&)unitId {
    
    if( unitId == "" ) {
        return;
    }
    
    interstitialUnitId = NS_STRING(unitId.c_str());
    [self loadInterstitial];
}

#pragma mark- GADFullScreenContentDelegate

/**
 * Tells the delegate that an impression has been recorded for the ad.
 */
- (void) adDidRecordImpression:(nonnull id<GADFullScreenPresentingAd>)ad {
    
    NSLog(@"adDidRecordImpression");
}

/**
 * Tells the delegate that the ad failed to present full screen content.
 */
- (void) ad:(nonnull id<GADFullScreenPresentingAd>)ad
didFailToPresentFullScreenContentWithError:(nonnull NSError *)error {
    
    NSLog(@"ad:didFailToPresentFullScreenContentWithError");
}

/**
 * OnAdOpened
 * Tells the delegate that the ad presented full screen content.
 */
- (void) adDidPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {

    NSLog(@"adDidPresentFullScreenContent");
    
    if( ad == self.interstitialAd ) {
        AdsHelper::getInstance()->getInterstitial()->onAdOpened();
    }
    else if( ad == self.rewardedAd ) {
        // SBAudioEngine::pauseAll();
        AdsHelper::getInstance()->getRewardedVideo()->onAdOpened();
    }
}

/**
 * Tells the delegate that the ad will dismiss full screen content.
 */
- (void) adWillDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    
    NSLog(@"adWillDismissFullScreenContent");
}

/**
 * Tells the delegate that the ad dismissed full screen content.
 */
- (void) adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    
    NSLog(@"adDidDismissFullScreenContent");
    
    if( ad == self.interstitialAd ) {
        AdsHelper::getInstance()->getInterstitial()->onAdClosed();
        self.interstitialAd = nil;
        
        [self loadInterstitial];
    }
    else if( ad == self.rewardedAd ) {
        AdsHelper::getInstance()->getRewardedVideo()->onAdClosed();
        self.rewardedAd = nil;
        
        [self loadRewardedVideo];
    }
}

#pragma mark- 보상형

/**
 * 보상형 비디오 초기화
 */
- (void) initRewardedVideo:(const string&)unitId {
    
    if( unitId == "" ) {
        return;
    }
    
    rewardedUnitId = NS_STRING(unitId.c_str());
    
    [self loadRewardedVideo];
}

#pragma mark- 광고 유닛 컨트롤

- (bool) loadBanner {
    
    NSLog(@"AdsManager::loadBanner bannerView: %d", self.bannerView != nil);
    
    if( self.bannerView == nil ) {
        return false;
    }
    
    GADRequest *request = [self createRequest];
    [self.bannerView loadRequest:request];
    
    return true;
}

- (bool) loadInterstitial {
    
    NSLog(@"AdsManager::loadInterstitial");

    if( !interstitialUnitId || [interstitialUnitId isEqualToString:@""] ) {
        return false;
    }
    
    if( self.isInterstitialLoading ) {
        return false;
    }
    
    self.isInterstitialLoading = true;
    
    [GADInterstitialAd loadWithAdUnitID:interstitialUnitId
                                request:[self createRequest]
                      completionHandler:^(GADInterstitialAd *ad, NSError *error) {
        
        self.isInterstitialLoading = false;
        
        if( error ) {
            NSLog(@"Failed to load interstitial ad with error: %@", [error localizedDescription]);
            int errorCode = [[NSNumber numberWithInteger:error.code] intValue];
            AdsHelper::getInstance()->getInterstitial()->onAdFailedToLoad(errorCode);
            return;
        }
        
        self.interstitialAd = ad;
        self.interstitialAd.fullScreenContentDelegate = self;

        AdsHelper::getInstance()->getInterstitial()->onAdLoaded();
    }];
    
    return true;
}

- (bool) loadRewardedVideo {
    
    NSLog(@"AdsManager::loadRewardedVideo");
    
    if( !rewardedUnitId || [rewardedUnitId isEqualToString:@""] ) {
        return false;
    }
    
    if( self.isRewardedLoading ) {
        return false;
    }
    
    self.isRewardedLoading = true;
    
    [GADRewardedAd loadWithAdUnitID:rewardedUnitId
                            request:[self createRequest]
                  completionHandler:^(GADRewardedAd *ad, NSError *error) {
        
        self.isRewardedLoading = false;
        
        if( error ) {
            NSLog(@"Failed to load rewarded ad with error: %@", [error localizedDescription]);
            
            int errorCode = [[NSNumber numberWithInteger:error.code] intValue];
            AdsHelper::getInstance()->getRewardedVideo()->onAdFailedToLoad(errorCode);
            
            return;
        }
        
        self.rewardedAd = ad;
        self.rewardedAd.fullScreenContentDelegate = self;
        
        AdsHelper::getInstance()->getRewardedVideo()->onAdLoaded();
    }];
    
    return true;
}

/**
 * 배너 광고 노출
 */
- (void) showBanner {
    
    if( self.bannerView ) {
        self.bannerView.hidden = NO;
    }
    
    /*
    // 광고 로드됨
    if( banner->isLoaded() ) {
        bannerView.hidden = NO;
    }
    // 광고 로드 필요
    else {
        NSLog(@"The banner wasn't loaded yet.");
        [self loadBanner];
    }
    */
}

/**
 * 배너 광고 숨김
 */
- (void) hideBanner {
    
    if( self.bannerView ) {
        self.bannerView.hidden = YES;
    }
}

/**
 * 전면 광고 노출
 */
- (void) showInterstitial {
    
    // 광고 로딩됨, 광고 노출
    if( self.interstitialAd ) {
        [self.interstitialAd presentFromRootViewController:ROOT_VIEW_CONTROLLER];
    }
    // 광고 로딩
    else {
        NSLog(@"The interstitial wasn't loaded yet.");
        [self loadInterstitial];
    }
}

/**
 * 보상형 비디오 노출
 */
- (void) showRewardedVideo {
    
    // 광고 로딩됨, 광고 노출
    if( self.rewardedAd ) {
        [self.rewardedAd presentFromRootViewController:ROOT_VIEW_CONTROLLER
                              userDidEarnRewardHandler:^{
            
            GADAdReward *reward = self.rewardedAd.adReward;
            AdsHelper::getInstance()->getRewardedVideo()->onRewarded([reward.type UTF8String],
                                                                     [reward.amount intValue]);
        }];
        
    }
    // 광고 로딩
    else {
        NSLog(@"The rewarded video wasn't loaded yet.");
        [self loadRewardedVideo];
    }
}

- (float) getBannerWidth {
    return self.bannerView.frame.size.width;
}

- (float) getBannerHeight {
    return self.bannerView.frame.size.height;
}

@end

#endif // SB_PLUGIN_USE_ADS
