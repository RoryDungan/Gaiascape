#include <iostream>
#include "algorithms/globalTerrainMaster.h"
#include <math.h>

globalTerrainMaster::globalTerrainMaster(unsigned int size)
{
    iNumberOfBlocks = pow(pow(4.0, size * 0.5), 2) + 1;
    // Rows and columns start at 0, which is unintuitive now but makes the code a hell of a lot easier to read later.
    // In this circumstance iRows and iColumns will always be the same. When this code is converted to its own
    // heightmap generator class, there may be some situation where the dimensions are different.
    iDimensions = pow(4.0, size * 0.5);

    // iQuadrants is the number of quadrants that are going to be generated
    // Or in other terms, the number of subdivisions.
    // iQuadrants = 1 will have 9 vertices, iQuadrants = 2 will have 25 vertices [I THINK]
    iQuadrants = size;
    fStaggerValue = iNumberOfBlocks / 20;

    // Create a vector for the numberOfBlocks
    int iBlockID = 0;

    std::cout << std::endl;

    for(unsigned int y = 0; y < (iDimensions + 1); y++)
    {
        for (unsigned int x = 0; x < (iDimensions + 1); x++)
        {
            terrainBlock* tempPointer = new terrainBlock(x,y,iBlockID);
            terrainBlocks.push_back(tempPointer); // Program each column along the row
            iBlockID++;
        }
    }

    if (!run())
    {
        std::cout << "An exception has occurred!\n";
    }
}

globalTerrainMaster::~globalTerrainMaster()
{
    //dtor
}

bool globalTerrainMaster::run()
{
    // Generate terrain!

    if(
    genHeight(0) &&
    genTemp(0) &&
    genRain(0) &&
    genDrain() &&
    genSalin() &&
    genBiomes() &&
    genMountains() &&
    runRivers() &&
    genRivers() &&
    genPlantlife() &&

    displayMap())
    return true;
    else
    return false;
}

void globalTerrainMaster::genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant)
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

bool globalTerrainMaster::genHeight(int heightValue)
{
    // Calls the heightmap function. This technically can be somewhere else.
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

    return true;
}

bool globalTerrainMaster::genTemp(int temperValue)
{
    return true; // EMPTY
}

bool globalTerrainMaster::genRain(int rainValue)
{
    return true; // EMPTY
}

bool globalTerrainMaster::genDrain()
{
    return true; // EMPTY
}

bool globalTerrainMaster::genSalin()
{
    return true; // EMPTY
}

bool globalTerrainMaster::genBiomes()
{
    return true; // EMPTY
}

bool globalTerrainMaster::genMountains()
{
    return true; // EMPTY
}

bool globalTerrainMaster::runRivers()
{
    return true; // EMPTY
}

bool globalTerrainMaster::genRivers()
{
    return true; // EMPTY
}

bool globalTerrainMaster::genPlantlife()
{
    return true; // EMPTY
}

terrainBlock* globalTerrainMaster::getByID(int ID)
{
    return terrainBlocks[ID];
}

terrainBlock* globalTerrainMaster::getByLoc(unsigned int x, unsigned int y)
{
    for(unsigned int i = 0; i < terrainBlocks.size(); i++)
    {
        if(terrainBlocks[i]->iGlobalX == x)
            if(terrainBlocks[i]->iGlobalY == y)
                return terrainBlocks[i];
    }
    return NULL;
}

bool globalTerrainMaster::displayMap()
{
    // THIS ENTIRE FUNCTION IS ONLY NECCESSARY FOR TESTING PURPOSES AND SUCH SHOULD BE DELETED AS SOON AS IT IS
    // IMPLEMENTED VISUALLY
    // Non-visual versions of this program might be on Wraith's laptop.
    // Display the heightmap with numbers
    unsigned int iTempY = 0;
    unsigned int iBlocksProcessed = 0;
    while(iBlocksProcessed < terrainBlocks.size())
    {
        if (terrainBlocks[iBlocksProcessed]->iGlobalY > iTempY)
        {
            iTempY = terrainBlocks[iBlocksProcessed]->iGlobalY;
            std::cout << "\n";
        }
        // For display purposes only, calculating the digits in a number so that the map is easier to read.
        int digits = 0;
        int number = terrainBlocks[iBlocksProcessed]->getHeight();
        if ( number <= 0) digits = 1;
        while (number)
        {
            number /= 10;
            digits++;
        }
        if (digits == 1) std::cout << terrainBlocks[iBlocksProcessed]->getHeight() << "   ";
        if (digits == 2) std::cout << terrainBlocks[iBlocksProcessed]->getHeight() << "  ";
        if (digits == 3) std::cout << terrainBlocks[iBlocksProcessed]->getHeight() << " ";
        iBlocksProcessed++;
    }
    std::cout << "\n";
    std::cout << "iNumberOfBlocks = " << iNumberOfBlocks << "\n";
    return true;
}
