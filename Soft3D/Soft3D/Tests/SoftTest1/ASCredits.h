#pragma once
#include <AppState.h>
#include <vector>
#include <string>
#include "SoftTypes.h"
class kFont;
class Sound;
class QuickDraw;

class ASCredits :
    public AppState
{
public:

    void Init();
    void Update();
    void RenderAfter3D();
    void nextCred();
private:

    kFont* m_Font;
    QuickDraw* m_Draw;
    std::vector<std::string> m_Creds;
    std::vector<color> m_CredsCol;
    int curCred = -1;
    int curChar = 0;
    int nextChar = 0;
    int startY = 20;
    int topY = 20;
    Sound* m_Bleep = nullptr;
    bool waitKey = false;
};

