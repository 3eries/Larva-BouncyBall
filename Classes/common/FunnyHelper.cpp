//
//  FunnyHelper.cpp
//
//  Created by seongmin hwang on 2018. 7. 6..
//

#include "FunnyHelper.hpp"

using namespace std;

// 명언 목록
static const vector<FamousSaying> FAMOUS_SAYING_LIST = {
    FamousSaying("Simple is the Best.", "B.S.J"),
    FamousSaying("We are professional.", "JSW"),
    FamousSaying("Like a professional.", "J.S.W"),
    FamousSaying("To be, or not to be,\nthat is the question.", "William Shakespeare"),
    FamousSaying("Simplicity is\nthe ultimate sophistication.", "Leonardo da Vinci"),
    FamousSaying("Float like a butterfly,\nsting like a bee.", "Muhammad Ali"),
    FamousSaying("I think, therefore I am.", "Rene Descartes"),
};

FamousSaying FunnyHelper::getRandomFamousSaying() {
    
    int ran = arc4random() % FAMOUS_SAYING_LIST.size();
    return FAMOUS_SAYING_LIST[ran];
}
