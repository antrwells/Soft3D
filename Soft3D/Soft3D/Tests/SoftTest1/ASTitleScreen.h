#pragma once
#include "AppState.h"
class nodeEntity;
class sceneGraph;
class nodeCamera;
class pixelMap;
class QuickDraw;
class Texture2D;
class Channel;

class ASTitleScreen : public AppState
{

public:
    
    void Init();
    void Update();
    void RenderAfter3D();

private:
    QuickDraw* m_Draw;
    Texture2D* m_Title;
    float alpha = 0.0f;
    Channel* m_SongChan = nullptr;
    int showTime = 0;
    bool waitkey = false;
};


