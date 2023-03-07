#pragma once 
#include "node.h"
class nodeCamera :
    public node
{
public:

    matrix4 getWorldMat() {

        return node::getWorldMat().inverse();

    }

private:

};

