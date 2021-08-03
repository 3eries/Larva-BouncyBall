//
//  SBStringUtils.cpp
//  SBLib
//
//  Created by hwangseongmin on 2015. 5. 13..
//
//

#include "SBStringUtils.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>

USING_NS_SB;
USING_NS_CC;
using namespace std;

////////////////////////////////////////////
// toString
////////////////////////////////////////////

/**
 * float 값을 string으로 변환, integer 값인 경우 소수점 값은 제거
 */
string SBStringUtils::floatToString(const string &foramt, float num) {
    
    if( isInteger(num) ) {
        return toString((UInt64)num);
    }
    
    return STR_FORMAT(foramt.c_str(), num);
}

string SBStringUtils::floatToString(int decimalPointDigit, float num) {
    
    if( isInteger(num) ) {
        return toString((UInt64)num);
    }
    
    if( decimalPointDigit == -1 ) {
        return toString(num);
    }
    
    UInt64 digit = (UInt64)pow(10, decimalPointDigit);
    UInt64 i = num * digit;
    
    return toString((float)i / digit);
}

/**
 * 3자리 기준으로 ',' 를 추가 한다.
 * ex) 1000 -> 1,000
 */
string SBStringUtils::toCommaString(UInt64 number) {
    
    string commaString;
    int cnt = 0;
    
    do {
        commaString.insert(0, 1, char('0' + number % 10));
        number /= 10;
        if( ++cnt == 3 && number ) {
            commaString.insert(0, 1, ',');
            cnt = 0;
        }
        
    } while(number);
    
    return commaString;
}

/**
 *  oldChar -> newChar로 변환
 */
string SBStringUtils::replaceAll(string str, string oldStr, string newStr) {
    
    size_t pos = str.find(oldStr);
    size_t oldCharLength = strlen(oldStr.c_str());
    size_t newCharLength = strlen(newStr.c_str());
    
    while (pos < string::npos) {
        str.replace(pos, oldCharLength, newStr);
        pos = str.find(oldStr, pos + newCharLength);
    }
    
    return str.c_str();
}

/**
 * 문자열 포함 여부
 */
bool SBStringUtils::isContains(const string &str, const string &findStr) {

    return str.find(findStr) != string::npos;
}

/**
 *  대문자로 변환
 */
const char* SBStringUtils::toUpper(string str) {
    
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str.c_str();
}

/**
 *  소문자로 변환
 */
const char* SBStringUtils::toLower(string str) {
    
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str.c_str();
}

////////////////////////////////////////////
// trim
////////////////////////////////////////////

string SBStringUtils::trimLeft(const string str) {
    
    string::size_type n = str.find_first_not_of(" \t\v\n");
    return n == string::npos ? str : str.substr(n, str.length());
}

string SBStringUtils::trimRight(const string str) {
    
    string::size_type n = str.find_last_not_of(" \t\v\n");
    return n == string::npos ? str : str.substr(0, n + 1);
}

string SBStringUtils::trim(const string str) {
    
    return trimLeft(trimRight(str));
}

string SBStringUtils::insert(vector<string> arr, string insertStr) {
    
    string result = "";
    
    for( int i = 0; i < arr.size(); ++i ) {
        result += arr[i];
        
        if( i < arr.size() - 1 ) {
            result += insertStr;
        }
    }
    
    return result;
}

vector<string> SBStringUtils::split(string str, char separators) {
    
    vector<string> result;
    
    istringstream f(str);
    string s;
    
    while( getline(f, s, separators) ) {
        result.push_back(s);
    }
    
    return result;
}

/**
 * 텍스트 파일을 읽는다
 */
string SBStringUtils::readTextFile(const string &filePath) {
    
    auto utils = FileUtils::getInstance();
    
    Data data = utils->getDataFromFile(filePath);
    string str((const char *)data.getBytes(), data.getSize());
    
    return str;
}

/**
 *  Document에 텍스트 파일을 생성한다, 파일 경로에서 Document 경로는 미포함
 */
void SBStringUtils::writeTextFile(string filePath, string str) {
    
    ofstream w(filePath); // 저장할 파일 열기
    w << str;             // 파일 저장
    w.close();            // 파일 닫기
}

/**
 * 모든 문자가 integer 인지 체크
 */
bool SBStringUtils::isInteger(string str) {
    
    auto pred = [](char c) {
        
        return !isdigit(c);
    };
    
    return ( !str.empty() &&
            (find_if(str.begin(), str.end(), pred) == str.end()) );
}

/**
 * float 값이 integer 인지 체크
 */
bool SBStringUtils::isInteger(float num) {
    
    return ((UInt64)(num * 10) % 10 == 0);
}

vector<string> SBStringUtils::toArray(string str) {
    
    const char *text = str.c_str();
    
    int startPos = 0;
    int endPos = 0;
    char ch = *text;
    vector<string> strings;
    
    while (ch) {
        if (0x80 != (0xC0 & ch) &&
            (endPos != 0)) {
            
            string tmp = str.substr(startPos, endPos - startPos);
            strings.push_back(tmp);
            
            startPos = endPos;
        }
        
        ++text;
        ++endPos;
        
        ch = *text;
        
        // 마지막 글자
        if(!ch) {
            string tmp = str.substr(startPos, endPos - startPos);
            strings.push_back(tmp);
            
            break;
        }
    }
    
    return strings;
}

int SBStringUtils::calcCharCount(string str) {
    
    const char *text = str.c_str();
    
    int n = 0;
    char ch = 0;
    
    while ((ch = *text)) {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch)) {
            ++n;
        }
        
        ++text;
    }
    
    return n;
}

string SBStringUtils::toString(const vector<string> &vec) {
    
    string str = "";
    
    for( string s : vec ) {
        str += s;
    }
    
    return str;
}

string SBStringUtils::toString(vector<char> *vec) {
    
    string result(vec->begin(),vec->end());
    return result;
}

string SBStringUtils::toString(const Color3B &color) {
    
    return STR_FORMAT("%d,%d,%d", color.r, color.g, color.b);
}

/**
 * @param str ex)255,255,255
 */
Color3B SBStringUtils::toColor3B(const string &str) {
    
    string colorStr = SBStringUtils::replaceAll(str, " ", "");
    auto arr = SBStringUtils::split(colorStr, ',');
    
    if( arr.size() != 3 ) {
        return Color3B::WHITE;
    }
    
    return Color3B(TO_INTEGER(arr[0]), TO_INTEGER(arr[1]), TO_INTEGER(arr[2]));
}

Texture2D* SBStringUtils::toTexture(string result) {
    
    vector<unsigned char> buffer(result.begin(), result.end());
    
    Image *img = new Image();
    img->initWithImageData(&(buffer.front()), buffer.size());
    
    Texture2D* texture = new Texture2D();
    texture->initWithImage(img);
    img->release();
    
    texture->autorelease();
    
    return texture;
}

string SBStringUtils::encodeHTMLEntities(string str) {
    
    static map<string,string> htmlEscapeMap;
    htmlEscapeMap["&"] = "&amp;";
    htmlEscapeMap["'"] = "&apos;";
    htmlEscapeMap["<"] = "&lt;";
    htmlEscapeMap[">"] = "&rt;";
    
    for( auto it = htmlEscapeMap.begin(); it != htmlEscapeMap.end(); ++it ) {
        str = replaceAll(str, it->first, it->second);
    }
    
    return str;
}


