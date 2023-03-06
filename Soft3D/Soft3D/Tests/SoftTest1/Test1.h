#pragma once
#include <SoftApp.h>

class nodeEntity;
class sceneGraph;

class Test1 :
    public SoftApp
{
public:

    Test1() : SoftApp(1024, 769, "Soft3D - Test 1")
    {

    };

    void Init();
    void Render();

private:

    nodeEntity* ent1;
    sceneGraph* g1;

};

