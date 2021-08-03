//
//  SBDebug.cpp
//
//  Created by seongmin hwang on 20/12/2018.
//

#include "SBDebug.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN

string stringFormat(const char *format, va_list args) {
    
    string ret;
    
    char *buf = (char*)malloc(MAX_LOG_LENGTH);
    if( buf ) {
        vsnprintf(buf, MAX_LOG_LENGTH, format, args);
        ret = buf;
        free(buf);
    }
    
    return ret;
}

Log Log::d(const char *format, ...) {
    
    va_list args;
    va_start(args, format);
    string message = stringFormat(format, args);
    va_end(args);
    
    CCLOG("D/%s", message.c_str());
    
    return Log(Type::DEBUG, message);
}

Log Log::i(const char *format, ...) {
    
    va_list args;
    va_start(args, format);
    string message = stringFormat(format, args);
    va_end(args);
    
    CCLOG("I/%s", message.c_str());
    
    return Log(Type::INFO, message);
}

Log Log::w(const char *format, ...) {
    
    va_list args;
    va_start(args, format);
    string message = stringFormat(format, args);
    va_end(args);
    
    CCLOG("W/%s", message.c_str());
    
    return Log(Type::WARNING, message);
}

Log Log::e(const char *format, ...) {
    
    va_list args;
    va_start(args, format);
    string message = stringFormat(format, args);
    va_end(args);
    
    CCLOG("E/%s", message.c_str());
    
    return Log(Type::ERROR, message);
}

Log Log::showMessageBox() {
    
    string title = "";
    
    switch( type ) {
        case Type::DEBUG:       title = "DEBUG";            break;
        case Type::INFO:        title = "INFO";             break;
        case Type::WARNING:     title = "WARNING";          break;
        case Type::ERROR:       title = "ERROR";            break;
        default: break;
    }
    
    MessageBox(message.c_str(), title.c_str());
    
    return *this;
}

NS_SB_END
