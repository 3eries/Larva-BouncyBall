//
//  SBSecurity.hpp
//
//  Created by seongmin hwang on 2018. 9. 19..
//

#ifndef SBSecurity_hpp
#define SBSecurity_hpp

#include <stdio.h>

#include "cocos2d.h"

class SBSecurity {
public:
    static std::string encryptAES256(const std::string &str, const std::string &key);
    static std::string decryptAES256(const std::string &str, const std::string &key);
    
    static std::string createMD5(const std::string &str,
                                 const std::string &prefix = "",
                                 const std::string &suffix = "");
};

#endif /* SBSecurity_hpp */
