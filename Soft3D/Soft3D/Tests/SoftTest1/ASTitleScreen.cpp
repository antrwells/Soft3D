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
void ASTitleScreen::Init() {

	m_Draw = new QuickDraw;
	m_Title = new Texture2D("data/image/title/mainbg.png");

	auto song = AudioSystem::ThisAudio->LoadSound("data/music/title.mp3");
	auto chan = AudioSystem::ThisAudio->PlaySound(song);
	m_SongChan = chan;
}

void ASTitleScreen::Update() {

	if (alpha < 1.0f) {
		alpha = alpha + 0.005f;
	}
	if (m_SongChan->Playing() == false || gameInput::anyKeyPressed)
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