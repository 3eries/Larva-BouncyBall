package com.superbomb.plugins.iap;

import android.content.Intent;
import androidx.annotation.Nullable;
import android.util.Log;

import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClient.BillingResponseCode;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.PurchasesUpdatedListener;
import com.android.billingclient.api.SkuDetails;
import com.android.billingclient.api.SkuDetailsResponseListener;
import com.superbomb.json.SBJSON;
import com.superbomb.plugins.PluginListener;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class IAPHelper implements PluginListener, BillingManager.BillingUpdatesListener {

    private static final String TAG = "IAP";

    private static IAPHelper instance = null;

    public static IAPHelper getInstance() {
        if( instance == null ) {
            instance = new IAPHelper();
        }

        return instance;
    }

    private Cocos2dxActivity context;
    private BillingManager billingManager;

    private static class Item {
        public String name;
        public String itemId;
        public boolean consumable;

        public Item() {
            consumable = true;
        }
    }

    private final List<Item> items;
    private List<SkuDetails> skuDetailsList;

    private IAPHelper() {
        items = new ArrayList<>();
    }

    public static Cocos2dxActivity getContext() {
        return instance.context;
    }

    public static BillingManager getBillingManager() {
        return instance.billingManager;
    }

    /**
     * 아이템 반환
     */
    public static List<Item> getItems() {
        return instance.items;
    }

    public static Item getItemById(String itemId) {

        List<Item> items = getItems();

        for( Item item : items ) {
            if( item.itemId.equals(itemId) ) {
                return item;
            }
        }

        return null;
    }

    public static Item findItem(List<Purchase> purchases, String itemId) {

        if( purchases == null ) {
            return null;
        }

        for( Purchase purchase : purchases ) {
            ArrayList<String> skus = purchase.getSkus();

            for( String sku : skus ) {
                Item item = getItemById(sku);

                if( item != null && item.itemId.equals(itemId) ) {
                    return item;
                }
            }
        }

        return null;
    }

    public static SkuDetails getSkuDetails(String sku) {

        List<SkuDetails> skuDetailsList = instance.skuDetailsList;

        if( skuDetailsList == null ) {
            return null;
        }

        for( SkuDetails details : skuDetailsList ) {
            if( details.getSku().equals(sku) ) {
                return details;
            }
        }

        return null;
    }

    @Override
    public void init(Cocos2dxActivity context) {

        this.context = context;
    }

    public void initBillingManager(String base64Key) {

        // Log.i(TAG, "initBillingManager: " + base64Key);

        billingManager = new BillingManager(context, base64Key, this);
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {

        if( billingManager != null ) {
            Log.i(TAG, "onResume isReady: " + billingManager.isBillingClientReady() + ", " + billingManager.isServiceConnected());
        } else {
            Log.i(TAG, "onResume billingManager is null");
        }

         /*
         if( billingManager != null && billingManager.getBillingClientResponseCode() == BillingResponse.OK ) {
             billingManager.queryPurchases();
         }
         */
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {

        if( billingManager != null ) {
            billingManager.destroy();
        }
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
     * IAP 모듈 초기화
     */
    public static void initIAP(String json) {

        Map<String, Object> jsonObj = (Map<String, Object>) SBJSON.parse(json);

        // items
        Map<String, Object> itemsObj = (Map<String, Object>) jsonObj.get("items");
        List<Item> items = getItems();

        Iterator<String> it = itemsObj.keySet().iterator();

        while( it.hasNext() ) {
            String key = it.next();
            Map<String, Object> obj = (Map<String, Object>) itemsObj.get(key);

            Item item = new Item();
            item.name = key;
            item.itemId = (String) obj.get("id");

            if( obj.containsKey("type") ) {
                String type = (String) obj.get("type");
                item.consumable = type.equals("consumable");
            }

            items.add(item);
        }

        // base64 key
        final String base64Key = (String) jsonObj.get("key");

        // BillingManager 초기화
        getContext().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                instance.initBillingManager(base64Key);
            }
        });
    }

    /**
     * 구매 승인
     * @param consumable false: 아이템 구매 승인 true: 아이템 소모
     */
    private void acknowledgePurchase(Purchase purchase, boolean consumable) {

        // 비소모성 아이템, 구매 승인
        if( !consumable ) {
            if( !purchase.isAcknowledged() ) {
                billingManager.acknowledgePurchase(purchase);
            }
        }
        // 소모성 아이템, 구매 즉시 소모
        else {
            billingManager.consumeAsync(purchase);
        }
    }

    /**
     * 인벤토리 업데이트
     */
    private void updateInventory(final PurchasesUpdatedListener listener) {

        context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                billingManager.queryPurchases(new PurchasesUpdatedListener() {
                    @Override
                    public void onPurchasesUpdated(BillingResult billingResult, @Nullable List<Purchase> purchases) {

                        int responseCode = billingResult.getResponseCode();
                        Log.i(TAG, "updateInventory onPurchasesUpdated responseCode: " + responseCode +
                                ", purchases: " + (purchases != null ? purchases.size() : 0));

                        // 요청 성공
                        if( responseCode == BillingResponseCode.OK && purchases != null ) {
                            for( Purchase purchase : purchases ) {
                                ArrayList<String> skus = purchase.getSkus();

                                for( String sku : skus ) {
                                    Item item = getItemById(sku);

                                    // 아이템 구매 승인
                                    if( item != null ) {
                                        acknowledgePurchase(purchase, item.consumable);
                                    }
                                }
                            }
                        }

                        if( listener != null ) {
                            listener.onPurchasesUpdated(billingResult, purchases);
                        }
                    }
                });
            }
        });
    }

    /**
     * 상품 정보를 받아옵니다
     */
    public void updateSkuDetails(final SkuDetailsResponseListener listener) {

        if( items.size() == 0 ) {
            return;
        }

        context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ArrayList<String> skuList = new ArrayList<>();

                for( Item item : items ) {
                    Log.i(TAG, "REQUEST SKU DETAIL: " + item.itemId);
                    skuList.add(item.itemId);
                }

                billingManager.querySkuDetailsAsync(BillingClient.SkuType.INAPP, skuList, new SkuDetailsResponseListener() {
                    @Override
                    public void onSkuDetailsResponse(BillingResult billingResult, List<SkuDetails> skuDetailsList) {

                        if( billingResult.getResponseCode() == BillingResponseCode.OK ) {
                            instance.skuDetailsList = skuDetailsList;
                        }

                        Log.i(TAG, "onSkuDetailsResponse responseCode: " + billingResult.getResponseCode());

                        if( skuDetailsList != null ) {
                            for( SkuDetails details : skuDetailsList ) {
                                Log.i(TAG, "SKU: " + details.getSku());
                            }
                        }

                        if( listener != null ) {
                            listener.onSkuDetailsResponse(billingResult, skuDetailsList);
                        }
                    }
                });
            }
        });
    }

    /**
     * 클라이언트 초기화 완료
     */
    @Override
    public void onBillingClientSetupFinished() {

        Log.i(TAG, "onBillingClientSetupFinished");

        context.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                nativeOnSetupFinished();
            }
        });

        // 인벤토리 업데이트
        updateInventory(null);

        // SKU 업데이트
        updateSkuDetails(null);

        // consume for test
        // consumeAll();
    }

    /**
     * 아이템 소모 완료
     */
    @Override
    public void onConsumeFinished(String token, BillingResult billingResult) {

        // Log.d(TAG, "Consumption finished. Purchase token: " + token + ", responseCode: " + responseCode);

        // 소모 성공
        if( billingResult.getResponseCode() == BillingResponseCode.OK ) {
        }
        // 소모 실패
        else {
        }
    }

    /**
     * 비소모성 아이템 구매 확인 완료
     */
    @Override
    public void onAcknowledgePurchaseFinished(Purchase purchase, BillingResult billingResult) {

        // 성공
        if( billingResult.getResponseCode() == BillingResponseCode.OK ) {
            Log.i(TAG, "onAcknowledgePurchaseFinished succeed!");
        }
        // 실패
        else {
            Log.i(TAG, "onAcknowledgePurchaseFinished fail! debug message: " + billingResult.getDebugMessage());
        }
    }

    /**
     * 아이템 결제 완료
     */
    @Override
    public void onPurchasesUpdated(BillingResult billingResult, List<Purchase> purchases) {

        final int responseCode = billingResult.getResponseCode();
        Log.i(TAG, "onPurchasesUpdated responseCode: " + responseCode +
                ", purchases: " + (purchases != null ? purchases.size() : 0));

        // 결제 성공
        if( responseCode == BillingResponseCode.OK && purchases != null ) {
            for( Purchase purchase : purchases ) {
                ArrayList<String> skus = purchase.getSkus();

                for( String sku : skus ) {
                    final Item item = getItemById(sku);
                    if( item == null ) {
                        Log.i(TAG, "onPurchasesUpdated invalid sku: " + sku);
                        continue;
                    }

                    // 아이템 구매 승인
                    acknowledgePurchase(purchase, item.consumable);

                    // 구매 완료 리스너 실행
                    context.runOnGLThread(new Runnable() {
                        @Override
                        public void run() {
                            nativeOnPurchased(item.itemId);
                        }
                    });
                }
            }
        }
        // 결제 취소
        else if( responseCode == BillingClient.BillingResponseCode.USER_CANCELED ) {
            context.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    nativeOnPurchaseCanceled();
                }
            });
        }
        // 결제 실패
        else {
            context.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    nativeOnPurchaseError("purchase error. [code : " + responseCode + "]");
                }
            });
        }

        Log.i(TAG, "end onPurchasesUpdated flow.");
    }

    /**
     * 아이템 결제
     */
    public static void purchase(final String itemId) {

        Log.i(TAG, "purchase itemId: " + itemId);

        final Item item = getItemById(itemId);

        // 유효하지 않은 아이템
        if( item == null ) {
            getContext().runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    nativeOnPurchaseError("invalid item");
                }
            });

            return;
        }

        getContext().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                // Step 1. 아이템 복원
                restore(new PurchasesUpdatedListener() {

                    @Override
                    public void onPurchasesUpdated(BillingResult billingResult, @Nullable List<Purchase> purchases) {

                        final int responseCode = billingResult.getResponseCode();

                        // 아이템 복원 요청 성공
                        if( responseCode == BillingResponseCode.OK ) {
                            // 결제 아이템이 복원된 아이템인지 체크
                            Item restoreItem = findItem(purchases, itemId);

                            // 복원된 아이템, 결제할 수 없음
                            if( restoreItem != null ) {
                                getContext().runOnGLThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        nativeOnPurchaseError("already owned.");
                                    }
                                });
                            }
                            // Step 2. 결제
                            else {
                                Log.i(TAG, "start purchase: " + itemId);

                                // SKU 체크
                                SkuDetails skuDetails = getSkuDetails(itemId);

                                if( skuDetails != null ) {
                                    getBillingManager().initiatePurchaseFlow(skuDetails);
                                } else {
                                    // SKU 업데이트
                                    instance.updateSkuDetails(new SkuDetailsResponseListener() {
                                        @Override
                                        public void onSkuDetailsResponse(BillingResult billingResult, List<SkuDetails> skuDetailsList) {

                                            if( billingResult.getResponseCode() == BillingResponseCode.OK ) {
                                                SkuDetails skuDetails = getSkuDetails(itemId);
                                                getBillingManager().initiatePurchaseFlow(skuDetails);
                                            } else {
                                                nativeOnPurchaseError("sku details error. [code : " + responseCode + "]");
                                            }
                                        }
                                    });
                                }
                            }
                        }
                        // 아이템 복원 요청 실패
                        else {
                            getContext().runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    nativeOnPurchaseError("restore error. [code : " + responseCode + "]");
                                }
                            });
                        }
                    }
                });
            }
        });
    }

    /**
     * 아이템 복원
     */
    public static void restore(final PurchasesUpdatedListener listener) {

        getInstance().updateInventory(new PurchasesUpdatedListener() {

            @Override
            public void onPurchasesUpdated(BillingResult billingResult, @Nullable List<Purchase> purchases) {

                int responseCode = billingResult.getResponseCode();
                final boolean result = (responseCode == BillingResponseCode.OK);

                if( result && purchases != null ) {
                    for( final Purchase purchase : purchases ) {
                        ArrayList<String> skus = purchase.getSkus();

                        for( String sku : skus ) {
                            getContext().runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    nativeOnRestored(sku);
                                }
                            });
                        }
                    }
                }

                getContext().runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        nativeOnRestoreFinished(result);
                    }
                });

                if( listener != null ) {
                    listener.onPurchasesUpdated(billingResult, purchases);
                }
            }
        });
    }

    public static void restore() {
        restore(null);
    }

    public static void consumeAll() {

        getInstance().updateInventory(new PurchasesUpdatedListener() {

            @Override
            public void onPurchasesUpdated(BillingResult billingResult, @Nullable List<Purchase> purchases) {

                int responseCode = billingResult.getResponseCode();
                final boolean result = (responseCode == BillingResponseCode.OK);

                if( result && purchases != null ) {
                    for( final Purchase purchase : purchases ) {
                        ArrayList<String> skus = purchase.getSkus();
                        for( String sku : skus ) {
                            Log.i(TAG, "consumeAll item: " + sku);
                        }

                        getInstance().billingManager.consumeAsync(purchase);
                    }
                }
            }
        });
    }

    public static boolean isReady() {

        BillingManager billingManager = getBillingManager();
        if( billingManager == null ) {
            return false;
        }

        return billingManager.isBillingClientReady() && billingManager.isServiceConnected();
    }

    public static native void nativeOnSetupFinished();

    public static native void nativeOnPurchased(String itemId);

    public static native void nativeOnPurchaseError(String errorMsg);

    public static native void nativeOnPurchaseCanceled();

    public static native void nativeOnRestored(String itemId);

    public static native void nativeOnRestoreFinished(boolean result);
}
