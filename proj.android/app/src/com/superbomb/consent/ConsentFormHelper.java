package com.superbomb.consent;

import android.app.Activity;

import androidx.annotation.Nullable;

//import com.google.android.ump.ConsentDebugSettings;
//import com.google.android.ump.ConsentForm;
//import com.google.android.ump.ConsentInformation;
//import com.google.android.ump.ConsentRequestParameters;
//import com.google.android.ump.FormError;
//import com.google.android.ump.UserMessagingPlatform;
//
//import org.cocos2dx.lib.Cocos2dxActivity;
//
//public class ConsentFormHelper {
//    private static ConsentInformation consentInformation;
//    private static ConsentForm consentForm;
//
//    public static void show() {
//        final Activity context = (Activity) Cocos2dxActivity.getContext();
//        context.runOnUiThread(new Runnable() {
//            @Override
//            public void run() {
//                // Force a geography for GDPR
//                /*
//                ConsentDebugSettings debugSettings = new ConsentDebugSettings.Builder(context)
//                        .setDebugGeography(ConsentDebugSettings.DebugGeography.DEBUG_GEOGRAPHY_EEA)
//                        .addTestDeviceHashedId("1BF60EA93EF22304AE5AFF19442DB1C4")
//                        .build();
//                 */
//
//                // Set tag for underage of consent. false means users are not underage.
//                ConsentRequestParameters params = new ConsentRequestParameters
//                        .Builder()
//                        .setTagForUnderAgeOfConsent(false)
//                        // .setConsentDebugSettings(debugSettings) // Force a geography for GDPR
//                        .build();
//
//                consentInformation = UserMessagingPlatform.getConsentInformation(context);
//                consentInformation.requestConsentInfoUpdate(context, params,
//                        new ConsentInformation.OnConsentInfoUpdateSuccessListener() {
//                            @Override
//                            public void onConsentInfoUpdateSuccess() {
//                                // The consent information state was updated.
//                                // You are now ready to check if a form is available.
//                                if (consentInformation.isConsentFormAvailable()) {
//                                    loadForm();
//                                }
//                            }
//                        },
//                        new ConsentInformation.OnConsentInfoUpdateFailureListener() {
//                            @Override
//                            public void onConsentInfoUpdateFailure(FormError formError) {
//                                // Handle the error.
//                            }
//                        });
//            }
//        });
//    }
//
//    public static void loadForm() {
//        final Activity context = (Activity) Cocos2dxActivity.getContext();
//        context.runOnUiThread(new Runnable() {
//            @Override
//            public void run() {
//                UserMessagingPlatform.loadConsentForm(context,
//                        new UserMessagingPlatform.OnConsentFormLoadSuccessListener() {
//                            @Override
//                            public void onConsentFormLoadSuccess(ConsentForm consentForm) {
//                                ConsentFormHelper.consentForm = consentForm;
//
//                                if(consentInformation.getConsentStatus() == ConsentInformation.ConsentStatus.OBTAINED) {
//                                    nativeOnClickOk();
//                                }  else if(consentInformation.getConsentStatus() == ConsentInformation.ConsentStatus.REQUIRED) {
//                                    consentForm.show(context, new ConsentForm.OnConsentFormDismissedListener() {
//                                        @Override
//                                        public void onConsentFormDismissed(@Nullable FormError formError) {
//                                            // Handle dismissal by reloading form.
//                                            loadForm();
//                                        }
//                                    });
//                                }
//                            }
//                        },
//                        new UserMessagingPlatform.OnConsentFormLoadFailureListener() {
//                            @Override
//                            public void onConsentFormLoadFailure(FormError formError) {
//                                /// Handle Error.
//                            }
//                        }
//                );
//            }
//        });
//    }
//
//    public static native void nativeOnClickPrivacyPolicy();
//    public static native void nativeOnClickOk();
//}
