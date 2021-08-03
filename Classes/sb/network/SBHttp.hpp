//
//  SBHttp.hpp
//
//  Created by hwangseongmin on 2017. 9. 20..
//
//

#ifndef SBHttp_hpp
#define SBHttp_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "network/HttpClient.h"

#include "../base/SBTypes.hpp"
#include "../base/SBMacros.h"

class SBHttp {
};

class SBHttpRequest : public cocos2d::network::HttpRequest {
    
public:
    typedef std::map<std::string, std::string> Parameters;
    typedef std::function<void(cocos2d::network::HttpResponse*, std::string)> OnSucceedListener;
    typedef std::function<void(cocos2d::network::HttpResponse*)> OnErrorListener;
    
public:
    SBHttpRequest();
    virtual ~SBHttpRequest();
    
public:
    void setParameters(const std::string &params);
    void setParameters(Parameters params);
    
    void send();
    
public:
    static std::string toString(Parameters params);
    
protected:
    CC_SYNTHESIZE(OnSucceedListener, onSucceedListener, OnSucceedListener);
    CC_SYNTHESIZE(OnErrorListener, onErrorListener, OnErrorListener);
};

#endif /* SBHttp_hpp */
