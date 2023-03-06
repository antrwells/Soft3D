#pragma once
#include <SoftApp.h>

class nodeEntity;

class Test1 :
    public SoftApp
{
public:

    Test1() : SoftApp(1024, 769, "Soft3D - Test 1")
    {

    };

    void Init();

private:

    nodeEntity* ent1;

};

