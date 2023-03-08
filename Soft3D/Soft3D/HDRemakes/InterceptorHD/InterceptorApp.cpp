#include "InterceptorApp.h"
#include "AudioSystem.h"
#include "QuickDraw.h"

void InterceptorApp::Init()
{

	auto s = AudioSystem::ThisAudio->LoadSound("data/music/title.mp3");
	AudioSystem::ThisAudio->PlaySound(s);
	m_Draw = new QuickDraw;

}

void InterceptorApp::Update(float et)
{

}

void InterceptorApp::Render() {

}