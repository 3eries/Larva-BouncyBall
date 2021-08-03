package com.superbomb.plugins.firebase;

public class FirebaseManager {

    private static FirebaseManager instance;
    public static FirebaseManager getInstance() {
        if( instance == null ) {
            instance = new FirebaseManager();
        }

        return instance;
    }

    private FirebaseManager() {

    }
}
