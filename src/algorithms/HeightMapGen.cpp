#include <iostream>
#include "algorithms/HeightMapGen.h"
#include <math.h>

HeightMapGen::HeightMapGen(float talos, unsigned int size)
{
    iNumberOfBlocks = pow(pow(4.0, size * 0.5), 2) + 1;
    fTalos = talos; // A good default is 4/iNumberOfBlocks, however this will change depending on how eroded the terrain should be.
    // Rows and columns start at 0, which is unintuitive now but makes the code a hell of a lot easier to read later.
    // In this circumstance iRows and iColumns will always be the same. When this code is converted to its own
    // heightmap generator class, there may be some situation where the dimensions are different.
    iDimensions = pow(4.0, size * 0.5);

    // iQuadrants is the number of quadrants that are going to be generated
    // Or in other terms, the number of subdivisions.
    // iQuadrants = 1 will have 9 vertices, iQuadrants = 2 will have 25 vertices [I THINK]
    iQuadrants = size;
    fStaggerValue = iNumberOfBlocks / 20;

    // The active ID that all blocks are assigned. Incremented later in the for loop.
    int iBlockID = 0;

    for(short unsigned int y = 0; y < (iDimensions + 1); y++)
    {
        for (short unsigned int x = 0; x < (iDimensions + 1); x++)
        {
            HMBlock* tempPointer = new HMBlock(x,y,iBlockID);
            HMBlocks.push_back(tempPointer); // Program each column along the row
            iBlockID++;
        }
    }

    // ------------------------------
    // Start the heightmap generator
    // ------------------------------

    // NW corner
    getByLoc(0, 0)->setHeight(Random::getSingleton().getRand(5, 10));

    // NE corner
    getByLoc(iDimensions, 0)->setHeight(Random::getSingleton().getRand(5, 10));

    // SE corner
    getByLoc(iDimensions, iDimensions)->setHeight(Random::getSingleton().getRand(5, 10));

    // SW corner
    getByLoc(0, iDimensions)->setHeight(Random::getSingleton().getRand(5, 10));

    // ToDo: Enable multithreading. This may require a small amount of hard coding.
    // The easiest way to do this is to essentially copy all that is in the genQuadrant function, and when calling
    // the next set of functions, assign a seperate thread for each of the four functions.
    genQuadrant(0, 0, iDimensions, iDimensions, 1, iQuadrants);
}

void HeightMapGen::genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant)
{
    // Generate a quadrant by giving each of the corners
    // Relative x and y are integers which give the x and y coordinates confined to that quadrant.
    // Therefore (iRelativeX/2) + xNW = Halfway along x
    int iRelativeX = xSE - xNW;
    int iRelativeY = ySE - yNW;

    // Calculating the midpoints, i.e.
    /*
      --+--
      |.|.|
      +-+-+
      |.|.|
      --+--
      where all + signs are calculated.
      */

    // Center
    getByLoc((iRelativeX/2) + xNW, (iRelativeY/2) + yNW)->setHeight(((getByLoc(xNW, yNW)->getHeight() + getByLoc(xSE, yNW)->getHeight() + getByLoc(xSE, ySE)->getHeight() + getByLoc(xNW, ySE)->getHeight())/4)
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration));
    // N
    getByLoc((iRelativeX/2) + xNW, yNW)->setHeight(((getByLoc(xNW, yNW)->getHeight() + getByLoc(xSE, yNW)->getHeight() + getByLoc((iRelativeX/2) + xNW, (iRelativeY/2) + yNW)->getHeight())/3)
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration));

    // E
    getByLoc(xSE, (iRelativeY/2) + yNW)->setHeight(((getByLoc(xSE, yNW)->getHeight() + getByLoc(xSE, ySE)->getHeight() + getByLoc((iRelativeX/2) + xNW, (iRelativeY/2) + yNW)->getHeight())/3)
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration));

    // S
    getByLoc((iRelativeX/2) + xNW, ySE)->setHeight(((getByLoc(xNW, ySE)->getHeight() + getByLoc((iRelativeX/2) + xNW, (iRelativeY/2) + yNW)->getHeight() + getByLoc(xSE, ySE)->getHeight())/3)
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration));

    // W
    getByLoc(xNW, (iRelativeY/2) + yNW)->setHeight(((getByLoc(xNW, yNW)->getHeight() + getByLoc(xNW, ySE)->getHeight() + getByLoc((iRelativeX/2) + xNW, (iRelativeY/2) + yNW)->getHeight())/3)
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration));

    if (quadrant != 1)
    {
        // This then carries on the process of generating quadrants, providing there are still quadrants to generate.
        // Because quadrants count down, we know we do not need to continue to the loop if we are on the last
        // quadrant.
        // Essentially, you simply repeat the process found here in which you find the midpoints by using the midpoints
        // generated here as the corners in the next call of this function.

        // NW
        genQuadrant(xNW, yNW, (iRelativeX/2) + xNW, (iRelativeY/2) + yNW, iteration + 1, quadrant - 1);

        // NE
        genQuadrant((iRelativeX/2) + xNW, yNW, xSE, (iRelativeY/2) + yNW, iteration + 1, quadrant - 1);

        // SE
        genQuadrant((iRelativeX/2) + xNW, (iRelativeY/2) + yNW, xSE, ySE, iteration + 1, quadrant - 1);

        // SW
        genQuadrant(xNW, (iRelativeY/2) + yNW, (iRelativeX/2) + xNW, ySE, iteration + 1, quadrant - 1);
    }
}

void erode(short int c)
{
    // Talos = The maximum angle allowed for slopes to be before thermal erosion begins taking place.
    // In most cases, we want this to be 4/N, where N = the number of blocks in the terrain set.
    // c = A variable I don't fully understand.

    // This uses Thermal Erosion
    // In the future, should probably modify it so that we can also have Rainfall erosion.
    // First we travel through all of the different HMBlocks and calculate the difference in height to
    // all of their neighbours, if they exist.
    // d1 = h - h1
    // where h = the height of the inspected block, h1 = the height of the neigbouring block at position 1, and d1
    // is the difference in height between inspected block 1 and the inspected block.
    // If the tile is higher than its neighbour, it will be positive, otherwise negative.

    // Grab the first terrain block in the array
    // If the neighbour at NW exists
    //
}

HMBlock* HeightMapGen::getByID(short unsigned int ID)
{
    return HMBlocks[ID];
}

HMBlock* HeightMapGen::getByLoc(short unsigned int x, short unsigned int y)
{
    for(unsigned int i = 0; i < HMBlocks.size(); i++)
    {
        if(HMBlocks[i]->iGlobalX == x)
            if(HMBlocks[i]->iGlobalY == y)
                return HMBlocks[i];
    }
    return NULL;
}

short unsigned int HeightMapGen::retrieveDimensions()
{
    return iDimensions;
}

std::vector<HMBlock*> HeightMapGen::retrieveBlocks()
{
    return HMBlocks;
}
