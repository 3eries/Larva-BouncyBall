package com.superbomb.plugins.gps;

//import android.app.Activity;
//import android.app.AlertDialog;
//import android.content.Intent;
//import android.util.Log;
//
//import androidx.annotation.NonNull;
//
//import com.google.android.gms.auth.api.Auth;
//import com.google.android.gms.auth.api.signin.GoogleSignIn;
//import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
//import com.google.android.gms.auth.api.signin.GoogleSignInClient;
//import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
//import com.google.android.gms.auth.api.signin.GoogleSignInResult;
//import com.google.android.gms.games.AchievementsClient;
//import com.google.android.gms.games.EventsClient;
//import com.google.android.gms.games.Games;
//import com.google.android.gms.games.LeaderboardsClient;
//import com.google.android.gms.games.PlayersClient;
//import com.google.android.gms.tasks.OnCompleteListener;
//import com.google.android.gms.tasks.OnFailureListener;
//import com.google.android.gms.tasks.OnSuccessListener;
//import com.google.android.gms.tasks.Task;
//import com.superbomb.plugins.PluginListener;
//import com.superbomb.series.Thirteen.R;
//
//import org.cocos2dx.lib.Cocos2dxActivity;
//
//public class GameHelper implements PluginListener {
//
//    private static final String TAG = "GPS_GAME";
//
//    private static GameHelper instance;
//    public static GameHelper getInstance() {
//        if( instance == null ) {
//            instance = new GameHelper();
//        }
//
//        return instance;
//    }
//
//    public static final int RC_SIGN_IN          = 419;
//    public static final int RC_LEADERBOARD_UI   = 420;
//    public static final int RC_ACHIEVEMENT_UI   = 421;
//
//    private Activity context;
//    private GoogleSignInClient signInClient;
//
//    private GameHelper() {
//
//    }
//
//    public static Activity getContext() {
//        return getInstance().context;
//    }
//
//    public static GoogleSignInClient getSignInClient() {
//        return getInstance().signInClient;
//    }
//
//    public static AchievementsClient getAchievementsClient() {
//        return Games.getAchievementsClient(getContext(), GoogleSignIn.getLastSignedInAccount(getContext()));
//    }
//
//    public static LeaderboardsClient getLeaderboardsClient() {
//        return Games.getLeaderboardsClient(getContext(), GoogleSignIn.getLastSignedInAccount(getContext()));
//    }
//
//    public static EventsClient getEventsClient() {
//        return Games.getEventsClient(getContext(), GoogleSignIn.getLastSignedInAccount(getContext()));
//    }
//
//    public static PlayersClient getPlayersClient() {
//        return Games.getPlayersClient(getContext(), GoogleSignIn.getLastSignedInAccount(getContext()));
//    }
//
//    @Override
//    public void init(Cocos2dxActivity context) {
//
//        this.context = context;
//        this.signInClient = GoogleSignIn.getClient(context,  GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN);
//    }
//
//    @Override
//    public void onStart() {
//    }
//
//    @Override
//    public void onStop() {
//    }
//
//    @Override
//    public void onResume() {
//    }
//
//    @Override
//    public void onPause() {
//    }
//
//    @Override
//    public void onDestroy() {
//    }
//
//    @Override
//    public boolean onBackPressed() {
//        return false;
//    }
//
//    @Override
//    public boolean onActivityResult(int requestCode, int resultCode, Intent data) {
//
//        if( requestCode == RC_SIGN_IN ) {
////            Task<GoogleSignInAccount> task =  GoogleSignIn.getSignedInAccountFromIntent(data);
////
////            try {
////                GoogleSignInAccount account = task.getResult(ApiException.class);
////                onConnected(account);
////            } catch (ApiException apiException) {
////                String message = apiException.getMessage();
////                if (message == null || message.isEmpty()) {
////                    message = getString(R.string.signin_other_error);
////                }
////
////                onDisconnected();
////
////                new AlertDialog.Builder(this)
////                        .setMessage(message)
////                        .setNeutralButton(android.R.string.ok, null)
////                        .show();
////            }
//
//            GoogleSignInResult result = Auth.GoogleSignInApi.getSignInResultFromIntent(data);
//
//            if( result.isSuccess() ) {
//                // The signed in account is stored in the result.
//                GoogleSignInAccount signedInAccount = result.getSignInAccount();
//                onConnected(signedInAccount);
//
//            } else {
//                onDisconnected();
//
//                /*
//                String message = result.getStatus().getStatusMessage();
//                if (message == null || message.isEmpty()) {
//                    message = getString(R.string.signin_other_error);
//                }
//                */
//
//                /*
//                new AlertDialog.Builder(getContext()).setMessage(R.string.error_signin)
//                        .setNeutralButton(android.R.string.ok, null).show();
//                */
//            }
//        }
//
//        return false;
//    }
//
//    /**
//     * 구글 게임 클라이언트 연결
//     */
//    private void onConnected(GoogleSignInAccount googleSignInAccount) {
//
//        Log.d(TAG, "onConnected(): connected to Google APIs");
//
////        achievementsClient = Games.getAchievementsClient(context, googleSignInAccount);
////        leaderboardsClient = Games.getLeaderboardsClient(context, googleSignInAccount);
////        eventsClient = Games.getEventsClient(context, googleSignInAccount);
////        playersClient = Games.getPlayersClient(context, googleSignInAccount);
//    }
//
//    /**
//     * 구글 게임 클라이언트 연결 해제
//     */
//    private void onDisconnected() {
//
//        Log.d(TAG, "onDisconnected()");
//
////        achievementsClient = null;
////        leaderboardsClient = null;
////        eventsClient = null;
////        playersClient = null;
//    }
//
//    private static void handleException(Exception e, String details) {
//
//        int status = 0;
//
//        /*
//        if (e instanceof ApiException) {
//            ApiException apiException = (ApiException) e;
//            status = apiException.getStatusCode();
//        }
//        */
//
//        String message = getContext().getString(R.string.google_api_error, details, status, e);
//
//        new AlertDialog.Builder(getContext())
//                .setMessage(message)
//                .setNeutralButton(android.R.string.ok, null)
//                .show();
//    }
//
//    /**
//     * 로그인
//     */
//    public static void signIn(boolean showLoginUI) {
//
//        Log.i(TAG, "signIn");
//
//        final Activity context = getContext();
//        final GoogleSignInClient signInClient = getSignInClient();
//
//        if( showLoginUI ) {
//            Intent intent = signInClient.getSignInIntent();
//            context.startActivityForResult(intent, RC_SIGN_IN);
//
//        } else {
//            signInClient.silentSignIn().addOnCompleteListener(context,
//                    new OnCompleteListener<GoogleSignInAccount>() {
//                        @Override
//                        public void onComplete(@NonNull Task<GoogleSignInAccount> task) {
//                            if( task.isSuccessful() ) {
//                                // The signed in account is stored in the task's result.
//                                GoogleSignInAccount signedInAccount = task.getResult();
//                                getInstance().onConnected(signedInAccount);
//                            } else {
//                                // Player will need to sign-in explicitly using via UI
//                                getInstance().onDisconnected();
//                            }
//                        }
//                    });
//        }
//    }
//
//    /**
//     * 로그아웃
//     */
//    public static void signOut() {
//
//        if( !isSignedIn() ) {
//            return;
//        }
//
//        getSignInClient().signOut().addOnCompleteListener(getContext(),
//                new OnCompleteListener<Void>() {
//                    @Override
//                    public void onComplete(@NonNull Task<Void> task) {
//                        // at this point, the user is signed out.
//                    }
//                });
//    }
//
//    /**
//     * 로그인 여부
//     */
//    public static boolean isSignedIn() {
//        return GoogleSignIn.getLastSignedInAccount(getContext()) != null;
//    }
//
//    /**
//     * 플레이어 아이디
//     */
//    public static String getPlayerId() {
//        if( !isSignedIn() ) {
//            return "";
//        }
//
//        return getPlayersClient().getCurrentPlayerId().getResult();
//    }
//
//    /**
//     * 모든 리더보드 UI 출력
//     */
//    public static void showAllLeaderboards() {
//
//        Log.i(TAG, "showAllLeaderboards");
//
//        if( !isSignedIn() ) {
//            return;
//        }
//
//        getLeaderboardsClient().getAllLeaderboardsIntent()
//                .addOnSuccessListener(new OnSuccessListener<Intent>() {
//                    @Override
//                    public void onSuccess(Intent intent) {
//                        getContext().startActivityForResult(intent, RC_LEADERBOARD_UI);
//                    }
//                })
//                .addOnFailureListener(new OnFailureListener() {
//                    @Override
//                    public void onFailure(@NonNull Exception e) {
//                        handleException(e, getContext().getString(R.string.error_leaderboard));
//                    }
//                });
//    }
//
//    /**
//     * 특정 리더보드 UI 출력
//     */
//    public static void showLeaderboard(String leaderboardName) {
//
//        if( !isSignedIn() ) {
//            return;
//        }
//
//        getLeaderboardsClient().getLeaderboardIntent(leaderboardName)
//                .addOnSuccessListener(new OnSuccessListener<Intent>() {
//                    @Override
//                    public void onSuccess(Intent intent) {
//                        getContext().startActivityForResult(intent, RC_LEADERBOARD_UI);
//                    }
//                })
//                .addOnFailureListener(new OnFailureListener() {
//                    @Override
//                    public void onFailure(@NonNull Exception e) {
//                        handleException(e, getContext().getString(R.string.error_leaderboard));
//                    }
//                });
//    }
//
//    /**
//     * 점수 등록
//     */
//    public static void submitScore(String leaderboardName, int score) {
//
//        Log.i(TAG, "submitScore: " + leaderboardName + ", score: " + score);
//
//        if( !isSignedIn() ) {
//            return;
//        }
//
//        getLeaderboardsClient().submitScore(leaderboardName, score);
//    }
//
//    /**
//     * 업적 UI 출력
//     */
//    public static void showAchievements() {
//
//        if( !isSignedIn() ) {
//            return;
//        }
//
//        getAchievementsClient().getAchievementsIntent()
//                .addOnSuccessListener(new OnSuccessListener<Intent>() {
//                    @Override
//                    public void onSuccess(Intent intent) {
//                        getContext().startActivityForResult(intent, RC_ACHIEVEMENT_UI);
//                    }
//                })
//                .addOnFailureListener(new OnFailureListener() {
//                    @Override
//                    public void onFailure(@NonNull Exception e) {
//                        handleException(e, getContext().getString(R.string.error_achievement));
//                    }
//                });
//    }
//
//    public static void loadAchievements(boolean forceReload) {
//
//        if( !isSignedIn() ) {
//            return;
//        }
//    }
//
//    /**
//     * 업적 해제
//     */
//    public static void unlockAchievement(String achievementName) {
//
//        if( !isSignedIn() ) {
//            return;
//        }
//
//        getAchievementsClient().unlock(achievementName);
//    }
//
//    public static void incrementAchievement(String achievementName, int increment) {
//
//        if( !isSignedIn() ) {
//            return;
//        }
//
//        getAchievementsClient().increment(achievementName, increment);
//    }
//
//    public static void reveal(String achievementName) {
//
//        if( !isSignedIn() ) {
//            return;
//        }
//    }
//
//    public static void setSteps(String achievementName, double steps) {
//
//        if( !isSignedIn() ) {
//            return;
//        }
//    }
//}
