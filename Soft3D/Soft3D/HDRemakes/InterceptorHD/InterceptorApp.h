#pragma once
#include "SoftApp.h"
#include "QuickDraw.h"


class InterceptorApp :
    public SoftApp
{
public:

    InterceptorApp() : SoftApp(1024, 768, "F18 Interceptor HD-Remake")
    {

    }
    void Init();
    void Update(float et);
    void Render();

private:

    QuickDraw* m_Draw;

};

