#include <iostream>
#include "algorithms\HeightMapGen.h"
#include <math.h>
#include <fstream>
#include <QImage>
#include <QByteArray>

HeightMapGen::HeightMapGen(float talos, unsigned int size)
{

}

void HeightMapGen::genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant)
{
    // Generate a quadrant by giving each of the corners
    // Relative x and y are integers which give the x and y coordinates confined to that quadrant.
    // Therefore (iRelativeX/2) + xNW = Halfway along x
    // x values are already multiplied by iDimensions

    int iHalfRelativeX = (xSE - xNW)/2 + xNW; // This is never not used, but it is pausible that vanilla iRelativeX/2
    int iHalfRelativeY = (ySE - yNW)/2 + yNW; // Same with iHalfRelativeX

    // Calculating the midpoints, i.e.
    /*
      --+--
      |.|.|
      +-+-+
      |.|.|
      --+--
      where all + signs are calculated.
      */

    // How to access a two dimensional array pointer
    // To get array[8][7] where we have array[x][y], x would equal 8 and y would equal 7.
    // For a pointer, this is essentially element 87, or *(pArray + iDimensions*x + y)

    // Center
    *(pHMBlocks + iHalfRelativeX + iHalfRelativeY) = (*(pHMBlocks + xNW + yNW) + *(pHMBlocks + xSE + yNW) + *(pHMBlocks + xSE + ySE) + *(pHMBlocks + xNW + ySE))/4
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // TODO: Check to make sure these haven't already been assigned, as there would be no point to assigning them again
    // N
    if(*(pHMBlocks + iHalfRelativeX + yNW) == 0)
        *(pHMBlocks + iHalfRelativeX + yNW) = (*(pHMBlocks + xNW + yNW) + *(pHMBlocks + xSE + yNW))/2
                                            + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // E
    if(*(pHMBlocks + xSE + iHalfRelativeY) == 0)
        *(pHMBlocks + xSE + iHalfRelativeY) = (*(pHMBlocks + xSE + yNW) + *(pHMBlocks + xSE + ySE))/2
                                            + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // S
    if(*(pHMBlocks + iHalfRelativeX + ySE) == 0)
        *(pHMBlocks + iHalfRelativeX + ySE) = (*(pHMBlocks + xNW + ySE) + *(pHMBlocks + xSE + ySE))/2
                                            + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // W
    if(*(pHMBlocks + xNW + iHalfRelativeY) == 0)
        *(pHMBlocks + xNW + iHalfRelativeY) = (*(pHMBlocks + xNW + yNW) + *(pHMBlocks + xNW + ySE))/2
                                            + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    if (quadrant != 1)
    {
        // This then carries on the process of generating quadrants, providing there are still quadrants to generate.
        // Because quadrants count down, we know we do not need to continue to the loop if we are on the last
        // quadrant.
        // Essentially, you simply repeat the process found here in which you find the midpoints by using the midpoints
        // generated here as the corners in the next call of this function.
        // Increase iteration for the new set of quadrants
        iteration *= 2;

        // NW
        genQuadrant(xNW, yNW, iHalfRelativeX, iHalfRelativeY, iteration, quadrant - 1);

        // NE
        genQuadrant(iHalfRelativeX, yNW, xSE, iHalfRelativeY, iteration, quadrant - 1);

        // SE
        genQuadrant(iHalfRelativeX , iHalfRelativeY, xSE, ySE, iteration, quadrant - 1);

        // SW
        genQuadrant(xNW, iHalfRelativeY, iHalfRelativeX, ySE, iteration, quadrant - 1);
    }
}

void HeightMapGen::erode(float c)
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

    // Use a for statement to call erodeBlock on every block in the array in order
    for(long unsigned int i = 0; i <= iFinalPoint; i++)
        erodeBlock(c, i);

}

void HeightMapGen::erodeBlock(float c, long unsigned int block)
{
    // Erode an individual block, and if this passes material down, then calculate erosion for that as well.
    // dNW, dNE etc. = difference in heights at point NW, NE etc. Set as 0 so if the points don't exist they don't cause
    // errors later.
    float dNW = 0;
    float dNE = 0;
    float dSW = 0;
    float dSE = 0;
    // dTotal = The total of all the d's that are above Talus.
    float dTotal = 0;
    // dMax = The highest d value
    float dMax = 0;
    // Define height values
    // Check if the point exists by seeing if is above the max or below the min
    // NW
    if(block - iDimensions - 1 >= 0 && block - iDimensions - 1 <= iFinalPoint)
    {
        // di = h - hi
        dNW = *(pHMBlocks + block) - *(pHMBlocks + block - iDimensions - 1);
        if(dNW > fTalos)
        {
            // Skipping the calculations because we know this if we are at this point dTotal and dMax haven't been set
            dTotal = dNW;
            dMax = dNW;
        }
        else
            dNW = 0; // Which later means we will not change this block
    }
    // NE
    if(block + iDimensions - 1 >= 0 && block + iDimensions - 1 <= iFinalPoint)
    {
        // di = h - hi
        dNE = *(pHMBlocks + block) - *(pHMBlocks + block + iDimensions - 1);
        if(dNE > fTalos)
        {
            dTotal += dNE;
            if(dNE > dMax)
                dMax = dNE;
        }
        else
            dNE = 0; // Which later means we will not change this block
    }
    // SE
    if(block + iDimensions + 1 >= 0 && block + iDimensions + 1 <= iFinalPoint)
    {
        // di = h - hi
        dSE = *(pHMBlocks + block) - *(pHMBlocks + block + iDimensions + 1);
        if(dSE > fTalos)
        {
            dTotal += dSE;
            if(dSE > dMax)
                dMax = dSE;
        }
        else
            dSE = 0; // Which later means we will not change this block
    }
    // SW
    if(block - iDimensions + 1 >= 0 && block - iDimensions + 1 <= iFinalPoint)
    {
        // di = h - hi
        dSW = *(pHMBlocks + block) - *(pHMBlocks + block - iDimensions + 1);
        if(dSW > fTalos)
        {
            dTotal += dSW;
            if(dSW > dMax)
                dMax = dSW;
        }
        else
            dSW = 0; // Which later means we will not change this block
    }

    // Go to each of the corners and apply the equation
    // hi += c(dmax - talos) * di/dtotal
    // NW
    if(dNW != 0)
        *(pHMBlocks + block - iDimensions - 1) += c * (dMax - fTalos) * (dNW/dTotal);
    // NE
    if(dNE != 0)
        *(pHMBlocks + block + iDimensions - 1) += c * (dMax - fTalos) * (dNE/dTotal);
    // SE
    if(dSE != 0)
        *(pHMBlocks + block + iDimensions + 1) += c * (dMax - fTalos) * (dSE/dTotal);
    // SW
    if(dSW != 0)
        *(pHMBlocks + block - iDimensions + 1) += c * (dMax - fTalos) * (dSW/dTotal);
    // ToDo; remove material transported?
    // Now that the heights have been calculated, we can now check to see if those points now have overflowing angles
    /*if(dNW != 0)
        erodeBlock(c, block - iDimensions - 1);
    if(dNE != 0)
        erodeBlock(c, block + iDimensions - 1);
    if(dSE != 0)
        erodeBlock(c, block + iDimensions + 1);
    if(dSW != 0)
        erodeBlock(c, block - iDimensions + 1);*/
}

short unsigned int HeightMapGen::retrieveDimensions()
{
    return iDimensions;
}

void HeightMapGen::retrieveHeightmap(float talos, unsigned int size, float* heightmapArray)
{
    iNumberOfBlocks = pow(pow(4.0, size * 0.5), 2) + 1;
    fTalos = talos; // A good default is 4/iNumberOfBlocks, however this will change depending on how eroded the terrain should be.
    // Rows and columns start at 0, which is unintuitive now but makes the code a hell of a lot easier to read later.
    // In this circumstance iRows and iColumns will always be the same. When this code is converted to its own
    // heightmap generator class, there may be some situation where the dimensions are different.
    iDimensions = pow(4.0, size * 0.5) + 1;

    // iQuadrants is the number of quadrants that are going to be generated
    // Or in other terms, the number of subdivisions.
    // iQuadrants = 1 will have 9 vertices, iQuadrants = 2 will have 25 vertices [I THINK]
    iQuadrants = size;
    fStaggerValue = iNumberOfBlocks*255;

    pHMBlocks = heightmapArray;

    // FinalX is the far right x point, in terms of a pointer.
    iFinalX = (iDimensions - 1)*iDimensions;
    iFinalPoint = iFinalX + iDimensions - 1;

    // NW corner
    *pHMBlocks = Random::getSingleton().getRand(5, 10);
    std::cout << "NW corner = " << *pHMBlocks << "\n";

    // NE corner
    *(pHMBlocks + iFinalX) = Random::getSingleton().getRand(5, 10);
    std::cout << "NE corner = " << *(pHMBlocks + iFinalX) << "\n";

    // SE corner
    *(pHMBlocks + iFinalX + iDimensions - 1) = Random::getSingleton().getRand(5, 10);
    std::cout << "SE corner = " << *(pHMBlocks + iFinalX + iDimensions - 1) << "\n";

    // SW corner
    *(pHMBlocks + iDimensions - 1) = Random::getSingleton().getRand(5, 10);
    std::cout << "SW corner = " << *(pHMBlocks + iDimensions - 1) << "\n";

    genQuadrant(0, 0, iFinalX, iDimensions - 1, 1, iQuadrants);

    // -----------------------
    // Set up the return value
    // -----------------------

    // A heightmap requires values ranging from 0 and 1. For now, everything is rated in proportion to the largest height
    // in the vector, so the highest point will be 1 and the lowest point will be 0
    // Step 1 = Get the largest and smallest height
    long signed int iMaxHeight;
    long signed int iMinHeight;

    // Setting a default iMaxHeight and iMinHeight to the northwest corner
    iMaxHeight = *pHMBlocks;
    iMinHeight = *pHMBlocks;

    // Go through the blocks and find out what is the highest and what is the lowest block
    for(long unsigned int i = 0; i <= iFinalX + iDimensions - 1; i++)
    {
        if(*(pHMBlocks + i) < iMinHeight)
            iMinHeight = *(pHMBlocks + i);
        if(*(pHMBlocks + i) > iMaxHeight)
            iMaxHeight = *(pHMBlocks + i);

    }

    // Advised for CPU optimisation
    short signed int iRelativeMaxHeight = iMaxHeight - iMinHeight;

    // Go through the blocks and scale them relative to the maximum or minimum
    // The percentage range can be found by first subtracting the miminum from the number
    // And then dividing that by iRelativeMaxHeight
    for(long unsigned int i = 0; i <= iFinalX + iDimensions - 1; i++)
    {
        // Scale it up to the point we want (by making it between 0 and 1 and then multiplying it by the range we want
        // Scaling disabled temporarily
        *(pHMBlocks + i) = ((*(pHMBlocks + i) - iMinHeight)/iRelativeMaxHeight);
    }

    // -------
    // Erosion
    // -------
    erode(-0.5);

    // Scale it up for the image
    /*for(long unsigned int i = 0; i <= iFinalX + iDimensions - 1; i++)
    {
        // Scale it up to the point we want (by making it between 0 and 1 and then multiplying it by the range we want
        // Scaling disabled temporarily
        *(pHMBlocks + i) *= 255;
    }*/

    writeMap();
}

void HeightMapGen::writeMap()
{
    std::ofstream myfile;
    myfile.open("map.txt");
    short int coutValue = 0;
    for(int x = 0; x < iDimensions; x++)
    {
        for(int y = 0; y < iDimensions; y++)
        {
            coutValue = *(pHMBlocks + x*iDimensions + y) * 100;
            // cout << coutValue << " ";
            myfile << coutValue << " ";
        }
        myfile << "\n";
    }
    myfile.close();
}
