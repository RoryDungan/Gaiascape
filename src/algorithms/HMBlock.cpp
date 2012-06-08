#include <iostream>
#include "algorithms/HMBlock.h"

HMBlock::HMBlock(unsigned int x, unsigned int y, unsigned int blockID)
{
    iGlobalX = x;
    iGlobalY = y;
    iHeight = 0; // The process for determining height is in another function
    bHeightSet = false;
    iBlockID = blockID;
}

HMBlock::~HMBlock()
{
    //dtor
}

void HMBlock::setHeight(float height)
{
    if(!bHeightSet)
    {
        iHeight = height;
        bHeightSet = true;
    }
    else
    {
        // std::cout << "Terrain block " << iGlobalX << " " << iGlobalY << " has already been declared once - cancelling overwrite.\n";
    }
}
float HMBlock::getHeight()           { return iHeight; }
bool HMBlock::isHeightSet()        { return bHeightSet; }
