//
//  SBSecurity-ios.m
//
//  Created by seongmin hwang on 2018. 9. 19..
//

#include "SBSecurity.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "NSData+AES.h"
#import "NSData+Base64.h"
#import "NSString+Base64.h"

#include "../platform/SBPlatformMacros.h"

USING_NS_CC;
using namespace std;

/**
 *  AES256 암호화
 */
string SBSecurity::encryptAES256(const string &str, const string &key) {
 
    NSString *nsStr = NS_STRING(str.c_str());
    NSString *nsKey = NS_STRING(key.c_str());
    
    NSData *cipherData = [[nsStr dataUsingEncoding:NSUTF8StringEncoding] AES256EncryptWithKey:nsKey];
    NSString *base64Text = [cipherData base64EncodedString];
    
    return [base64Text UTF8String];
}

/**
 *  AES256 복호화
 */
string SBSecurity::decryptAES256(const string &str, const string &key) {
    
    string decodeString = "";
    
    NSString *nsStr = NS_STRING(str.c_str());
    NSString *nsKey = NS_STRING(key.c_str());
    
    NSData *cipherData = [nsStr base64DecodedData];
    NSString *plainText = [[NSString alloc] initWithData:[cipherData AES256DecryptWithKey:nsKey]
                                                encoding:NSUTF8StringEncoding];
    
    if( !plainText ) {
        return "";
    }
    
    [plainText autorelease];
    return [plainText UTF8String];
}

#endif // CC_PLATFORM_IOS
