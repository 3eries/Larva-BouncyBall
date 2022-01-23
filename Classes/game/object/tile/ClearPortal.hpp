//
//  ClearPortal.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/01/23.
//

#ifndef ClearPortal_hpp
#define ClearPortal_hpp

#include <stdio.h>

#include "Flag.hpp"

class ClearPortal: public Flag {
public:
    static ClearPortal* create(const TileData &data);
    
    ClearPortal(const TileData &data);
    virtual ~ClearPortal();

    virtual bool init() override;
    
public:
    void setStar(int star);
    
    virtual void onContactFlag(Ball *ball, GameTile *tile) override;
    
private:
    SB_SYNTHESIZE_BOOL(opened, Opened);
};

#endif /* ClearPortal_hpp */
