//
//  SBSecurity.cpp
//
//  Created by seongmin hwang on 2018. 9. 19..
//

#include "SBSecurity.hpp"

#include "md5.h"

USING_NS_CC;
using namespace std;

/**
 *  MD5 hash 값 생성
 */
string SBSecurity::createMD5(const string &str, const string &prefix, const string &suffix) {
    
    string data = prefix + str + suffix;
    
    md5_byte_t digest[16];
    char hex_output[16 * 2 + 1];
    md5_state_t state;
    
    md5_init(&state);
    md5_append(&state, (const md5_byte_t *)data.c_str(), data.length());
    md5_finish(&state, digest);
    
    for( int di = 0; di < 16; ++di ) {
        sprintf(hex_output + di * 2, "%02x", digest[di]);
    }
    
    return hex_output;
}

