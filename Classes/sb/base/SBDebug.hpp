//
//  SBDebug.hpp
//
//  Created by seongmin hwang on 20/12/2018.
//

#ifndef SBDebug_hpp
#define SBDebug_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "SBMacros.h"

// #define DEBUG_LOG(__msg__, __messageBox__)  SBDebug::log(__msg__, __messageBox__)

NS_SB_BEGIN

class Log {
public:
    enum class Type {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
    };
    
    Log(Type type, const std::string &message) : type(type), message(message) {};
    ~Log() {};
    
    static Log d(const char *format, ...);
    static Log i(const char *format, ...);
    static Log w(const char *format, ...);
    static Log e(const char *format, ...);
    
    Log showMessageBox();
    
private:
    Type type;
    std::string message;
};

NS_SB_END

#endif /* SBDebug_hpp */
