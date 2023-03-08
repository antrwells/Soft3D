#include "ASTitleScreen.h"
#include "Importer.h"
#include "sceneGraph.h"
#include "nodeEntity.h"
#include "gameInput.h"
#include "pixelMap.h"
#include "AudioSystem.h"
#include "QuickDraw.h"
#include "Texture2D.h"
#include "ASTitleScreen.h"
#include "SoftApp.h"
#include "Channel.h"
#include "ASCredits.h"
#include <time.h>
void ASTitleScreen::Init() {

	m_Draw = new QuickDraw;
	m_Title = new Texture2D("data/image/title/mainbg.png");

	auto song = AudioSystem::ThisAudio->LoadSound("data/music/title.mp3");
	auto chan = AudioSystem::ThisAudio->PlaySound(song);
	m_SongChan = chan;
	showTime = clock() + 2500;
}

void ASTitleScreen::Update() {

	if (alpha < 1.0f) {
	//	alpha = alpha + 0.005f;
	}

	int ct = clock();
	if (ct > showTime) {
		alpha = alpha + 0.15f;
		if (alpha > 1) alpha = 1.0;
	}

	if (gameInput::anyKeyPressed)
	{
		waitkey = true;
	}
	else {
		if (waitkey) {
			SoftApp::m_This->PopState();
			m_SongChan->Stop();
			SoftApp::m_This->PushState(new ASCredits);
		}
	}

	if (m_SongChan->Playing() == false)
	{
		m_SongChan->Stop();
		SoftApp::m_This->PopState();
		SoftApp::m_This->PushState(new ASCredits);
	}

}

void ASTitleScreen::RenderAfter3D() {

	m_Draw->Begin();

	m_Draw->DrawRectRot(m_Title, Rect(0, 0, SoftApp::m_This->getWinWidth(), SoftApp::m_This->getWinHeight()), Color(1, 1, 1, alpha), nullptr, 0, 1.0f);

	m_Draw->End();

}