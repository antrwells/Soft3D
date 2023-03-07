#pragma once
#include <SoftApp.h>

class nodeEntity;
class sceneGraph;
class nodeCamera;
class pixelMap;

class Test1 :
    public SoftApp
{
public:

    Test1() : SoftApp(1024,768, "Soft3D - Test 1")
    {

    };

    void Init();
    void Render();

private:

    nodeEntity* ent1;
    sceneGraph* g1;
    nodeCamera* cam;
    pixelMap* tex1;
};

