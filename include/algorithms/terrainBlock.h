#ifndef TERRAINBLOCK_H
#define TERRAINBLOCK_H


class TerrainBlock
{
    public:
        TerrainBlock(unsigned int x, unsigned int y, unsigned int blockID);
        virtual ~TerrainBlock();

        unsigned int iGlobalX;
        unsigned int iGlobalY;
        unsigned int iBlockID;


        void setHeight(int height);
        int  getHeight();
        bool isHeightSet();
    protected:
    private:
        int          iHeight;
        bool         bHeightSet;
};

#endif // TERRAINBLOCK_H
