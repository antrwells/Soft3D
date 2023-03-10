#pragma once
#include <AppState.h>
class nodeEntity;
class sceneGraph;
class nodeCamera;
class pixelMap;
class QuickDraw;
class Texture2D;
class nodeLight;

class ASTest3D :
    public AppState
{
public:

    void Init();
    void Update();
    void Render();

private:


    nodeEntity* ent1;
    sceneGraph* g1;
    nodeCamera* cam;
    pixelMap* tex1;

    QuickDraw* m_Draw;
    Texture2D* m_Title;
    nodeLight* l1;

};

