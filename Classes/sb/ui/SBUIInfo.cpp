//
//  SBUIInfo.cpp
//  Snow Bros-mobile
//
//  Created by seongmin hwang on 2018. 3. 7..
//

#include "SBUIInfo.hpp"

USING_NS_CC;
using namespace std;

SBUIInfo::SBUIInfo() :
tag(Node::INVALID_TAG),
zOrder(DEFAULT_ZORDER),
anchorPoint(Vec2::ZERO),
pos(Vec2::ZERO),
file(""),
visible(true)
{}

SBUIInfo::SBUIInfo(int _tag, int _zOrder, Vec2 _anchorPoint, Vec2 _pos, const string &_file) :
tag(_tag),
zOrder(_zOrder),
anchorPoint(_anchorPoint),
pos(_pos),
file(_file),
visible(true)
{}

SBUIInfo::SBUIInfo(int _tag, int _zOrder) :
tag(_tag),
zOrder(_zOrder),
anchorPoint(Vec2::ZERO),
pos(Vec2::ZERO),
file(""),
visible(true)
{}

SBUIInfo::SBUIInfo(int _tag, const string &_file) :
tag(_tag),
zOrder(DEFAULT_ZORDER),
anchorPoint(Vec2::ZERO),
pos(Vec2::ZERO),
file(_file),
visible(true)
{}

SBUIInfo::SBUIInfo(int _tag, Vec2 _anchorPoint, Vec2 _pos, const string &_file, bool _visible) :
tag(_tag),
zOrder(DEFAULT_ZORDER),
anchorPoint(_anchorPoint),
pos(_pos),
file(_file),
visible(_visible)
{}

SBUIInfo::SBUIInfo(int _tag, Vec2 _anchorPoint, Vec2 _pos, const string &_file) :
tag(_tag),
zOrder(DEFAULT_ZORDER),
anchorPoint(_anchorPoint),
pos(_pos),
file(_file),
visible(true)
{}

SBUIInfo::SBUIInfo(Vec2 _anchorPoint, Vec2 _pos, const string &_file) :
tag(Node::INVALID_TAG),
zOrder(DEFAULT_ZORDER),
anchorPoint(_anchorPoint),
pos(_pos),
file(_file),
visible(true)
{}

SBUIInfo::SBUIInfo(const string &_file) :
tag(Node::INVALID_TAG),
zOrder(DEFAULT_ZORDER),
anchorPoint(Vec2::ZERO),
pos(Vec2::ZERO),
file(_file),
visible(true)
{}

void SBUIInfo::apply(Node *target) {
    
    if( tag != Node::INVALID_TAG )      target->setTag(tag);
    if( zOrder != DEFAULT_ZORDER )      target->setLocalZOrder(zOrder);
    
    target->setAnchorPoint(anchorPoint);
    target->setPosition(pos);
    target->setVisible(visible);
}


