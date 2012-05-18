#ifndef HMBLOCK_H
#define HMBLOCK_H


class HMBlock
{
    public:
        HMBlock(unsigned int x, unsigned int y, unsigned int blockID);
        virtual ~HMBlock();

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

#endif // HMBLOCK_H
