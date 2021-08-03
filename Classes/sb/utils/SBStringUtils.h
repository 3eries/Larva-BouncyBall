//
//  SBStringUtils.h
//  SBLib
//
//  Created by hwangseongmin on 2015. 5. 13..
//
//

#ifndef __SBLib__SBStringUtils__
#define __SBLib__SBStringUtils__

#include <stdio.h>

#include <vector>
#include <sstream>

#include "cocos2d.h"

#include "../base/SBMacros.h"
#include "../base/SBTypes.hpp"

class SBStringUtils {
public:
    static std::string floatToString(const std::string &foramt, float num);
    static std::string floatToString(int decimalPointDigit, float num);
    static std::string toCommaString(superbomb::UInt64 number);
    
    template <typename T>
    static std::string toString(T number) {
        
        std::stringstream ss;
        ss << number;
        
        return ss.str();
    }
    
    template <typename T>
    static T toNumber(std::string str) {
        
        std::stringstream ss(str);
        T result;
        
        return ss >> result ? result : 0;
    }
    
    static constexpr size_t hashStr(const char *str) {
        return *str ? static_cast<unsigned int>(*str) + 33 * hashStr(str+1) : 5381;
    }
    
    static std::string replaceAll(std::string str, std::string oldStr, std::string newStr);
    static bool isContains(const std::string &str, const std::string &findStr);
    
    static const char* toUpper(std::string str);
    static const char* toLower(std::string str);
    
    static std::string trimLeft(const std::string str);
    static std::string trimRight(const std::string str);
    static std::string trim(const std::string str);
    
    static std::string insert(std::vector<std::string> arr, std::string insertStr);
    
    static std::vector<std::string> split(std::string str, char separators);
    
    static std::string readTextFile(const std::string &filePath);
    static void writeTextFile(std::string filePath, std::string str);
    
    static bool isInteger(std::string str);
    static bool isInteger(float num);
    
    static std::vector<std::string> toArray(std::string str);
    static int calcCharCount(std::string str);
    
    static std::string toString(const std::vector<std::string> &vec);
    static std::string toString(std::vector<char> *vec);
    static std::string toString(const cocos2d::Color3B &color);
    
    static cocos2d::Color3B toColor3B(const std::string &str);
    static cocos2d::Texture2D* toTexture(std::string result);
    
// html
public:
    static std::string encodeHTMLEntities(std::string str);
};

#endif /* defined(__SBLib__SBStringUtils__) */
