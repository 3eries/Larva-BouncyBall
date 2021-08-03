//
//  SBHttp.cpp
//
//  Created by hwangseongmin on 2017. 9. 20..
//
//

#include "SBHttp.hpp"

#include "../utils/SBStringUtils.h"

USING_NS_CC;
using namespace cocos2d::network;
using namespace std;

SBHttpRequest::SBHttpRequest() : HttpRequest(),
onSucceedListener(nullptr),
onErrorListener(nullptr) {
    
    setResponseCallback([=](HttpClient *client, HttpResponse *response) {
        
        // error
        if( !response->isSucceed() ) {
            if( onErrorListener ) {
                onErrorListener(response);
            }
            return;
        }
        
        // success
        if( onSucceedListener ) {
            vector<char> *buffer = response->getResponseData();
            string result = SBStringUtils::toString(buffer);
            
            onSucceedListener(response, result);
        }
    });
}

SBHttpRequest::~SBHttpRequest() {
    
}

void SBHttpRequest::setParameters(const string &params) {
    
    switch( getRequestType() ) {
        case HttpRequest::Type::GET: {
            string url = string(getUrl()) + "?" + params;
            setUrl(url);
        } break;
            
        case HttpRequest::Type::POST: {
            setRequestData(params.c_str(), strlen(params.c_str()));
        } break;
            
        case HttpRequest::Type::PUT:        break;
        case HttpRequest::Type::DELETE:     break;
        case HttpRequest::Type::UNKNOWN:    break;
        default: break;
    }
}

void SBHttpRequest::setParameters(Parameters params) {
    
    setParameters(toString(params));
}

void SBHttpRequest::send() {
    
    HttpClient::getInstance()->send(this);
}

string SBHttpRequest::toString(Parameters params) {
    
    string param;
    int i = 0;
    
    for( auto it = params.begin(); it != params.end(); it++ ) {
        
        string key = it->first;
        string value = it->second;
        
        param.append(key.c_str());
        param.append("=");
        param.append(value.c_str());
        
        if( i < params.size()-1 ) {
            param.append("&");
        }
        
        ++i;
    }
    
    return param;
}
