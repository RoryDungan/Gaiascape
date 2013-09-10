#include <iostream>
#include "algorithms/terrainBlock.h"

terrainBlock::terrainBlock(unsigned int x, unsigned int y, unsigned int blockID)
{
    iGlobalX = x;
    iGlobalY = y;
    iHeight = 0; // The process for determining height is in another function
    bHeightSet = false;
    iBlockID = blockID;
}

terrainBlock::~terrainBlock()
{
    //dtor
}

void terrainBlock::setHeight(int height)
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
int terrainBlock::getHeight()           { return iHeight; }
bool terrainBlock::isHeightSet()        { return bHeightSet; }
