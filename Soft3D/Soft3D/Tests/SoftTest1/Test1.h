#pragma once
#include <SoftApp.h>

class nodeEntity;
class sceneGraph;
class nodeCamera;
class pixelMap;
class QuickDraw;
class Texture2D;

class Test1 :
    public SoftApp
{
public:

    Test1() : SoftApp(1280,720, "Soft3D - Test 1")
    {

    };

    void Init();
    void Update(float et);
    void Render();
    void RenderAfter3D();

private:

    nodeEntity* ent1;
    sceneGraph* g1;
    nodeCamera* cam;
    pixelMap* tex1;

    QuickDraw* m_Draw;
    Texture2D* m_Title;

};

