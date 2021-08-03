//
//  SBJniHelper.cpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "SBJniHelper.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

jobject JniHelper::createMapObject(JNIEnv *env, map<string, string> params) {
    
    jclass classID = env->FindClass("java/util/Hashtable");
    jmethodID initMethodID = env->GetMethodID(classID, "<init>","()V");
    jobject objMap = env->NewObject(classID, initMethodID, "");
    
    if( params.size() > 0 ) {
        jmethodID putMethodID = env->GetMethodID(classID, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        
        for( auto it = params.begin(); it != params.end(); ++it ) {
            jstring key = env->NewStringUTF(it->first.c_str());
            jstring value = env->NewStringUTF(it->second.c_str());
            
            env->CallObjectMethod(objMap, putMethodID, key, value);
            
            env->DeleteLocalRef(key);
            env->DeleteLocalRef(value);
        }
    }
    env->DeleteLocalRef(classID);
    
    return objMap;
}

NS_SB_END;

#endif // CC_PLATFORM_ANDROID
