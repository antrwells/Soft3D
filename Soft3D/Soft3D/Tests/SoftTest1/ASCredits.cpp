#include "ASCredits.h"
#include "kFont.h"
#include "QuickDraw.h"
#include "SoftApp.h"
#include <time.h>
#include "AudioSystem.h"
#include "Channel.h"
#include <random>
#include "gameInput.h"
#include "ASMainMenu.h"
#include "mathhelper.h"


void ASCredits::Init() {

	m_Bleep = AudioSystem::ThisAudio->LoadSound("data/sound/bleep1.wav");
	m_Font = new kFont("data/fonts/aqua.pf");
	m_Draw = new QuickDraw;
	m_BG = new Texture2D("data/image/bg/bg1.jpg");
	m_Font->setScale(1.4f);
	//mVideo = new VideoDecoder;
	//mVideo->OpenVideo("data/vid/bg/bg1.mov");


	m_Creds.push_back("F/A-18 INTERCEPTOR (tm)");
	m_CredsCol.push_back(color(0, 0.3, 1));
	m_Creds.push_back("(C)1988 INTELLISOFT");
	m_CredsCol.push_back(color(0, 0.3, 1));
	m_Creds.push_back("");
	m_CredsCol.push_back(color(0, 0, 0));
	m_Creds.push_back("DESIGNED AND PROGRAMMED BY:");
	m_CredsCol.push_back(color(1, 0, 0));
	m_Creds.push_back("BOB DINNERMAN");
	m_CredsCol.push_back(color(0.8, 0.8, 0.8));
	m_Creds.push_back("");
	m_CredsCol.push_back(color(0, 0, 0));
	m_Creds.push_back("DESIGN ASSISTANCE BY:");
	m_CredsCol.push_back(color(1, 0, 0));
	m_Creds.push_back("MOSES MA");
	m_CredsCol.push_back(color(0.8, 0.8, 0.8));

	m_Creds.push_back("");
	m_CredsCol.push_back(color(0, 0, 0));
	m_Creds.push_back("SOUNDS AND MUSIC BY:");
	m_CredsCol.push_back(color(1, 0, 0));
	m_Creds.push_back("DAVE WARHOL");
	m_CredsCol.push_back(color(0.8, 0.8, 0.8));

	m_Creds.push_back("");
	m_CredsCol.push_back(color(0, 0, 0));
	m_Creds.push_back("PUBLISHED BY:");
	m_CredsCol.push_back(color(1, 0, 0));
	m_Creds.push_back("ELECTRONIC ARTS, SAN MATEO, CA.");
	m_CredsCol.push_back(color(0.8, 0.8, 0.8));
	m_Creds.push_back("");
	m_CredsCol.push_back(color(0, 0, 0));
	m_Creds.push_back("OpenSource HD REMAKE VERSION BY:");
	m_CredsCol.push_back(color(1, 0.5, 0));
	m_Creds.push_back("ANTHONY WELLS");
	m_CredsCol.push_back(color(0, 1, 1));
	m_Creds.push_back("");
	m_CredsCol.push_back(color(0, 0, 0));
	m_Creds.push_back("PRESS ANY KEY");
	m_CredsCol.push_back(color(0, 1, 0));

	nextCred();

	
}


void ASCredits::nextCred() {

	curCred++;
	curChar = 0;
	nextChar = clock() + 250;
	startY += 35;
	auto chan = AudioSystem::ThisAudio->PlaySound(m_Bleep);
	float p = 1.0f;

	chan->SetPitch(p);


}

void ASCredits::Update() {

	if (gameInput::anyKeyPressed)
	{
		waitKey = true;
	}
	else {
		if (waitKey) {
			SoftApp::m_This->PopState();
			SoftApp::m_This->PushState(new ASMainMenu);
		}
	}

}

void ASCredits::RenderAfter3D() {


	m_Draw->Begin();

	m_Draw->DrawRectRot(m_BG, Rect(0, 0, SoftApp::m_This->getWinWidth(), SoftApp::m_This->getWinHeight()), Color(1, 1, 1, 1), nullptr, 0, 1.0f);

	m_Draw->End();


	glClear(GL_DEPTH_BUFFER_BIT);

	if (curCred > 0)
	{
		int dy = topY;
		for (int i = 0; i < curCred; i++) {
			std::string ps = m_Creds[i];
			color ps_c = m_CredsCol[i];
			int dx = 0;
			dy = dy + 35;
			dx = SoftApp::m_This->getWinWidth() / 2;
			int tw = m_Font->getWidth(ps.c_str());
			dx = dx - tw / 2;
			float tr, tg, tb;
			tr = ps_c.r;
			tg = ps_c.g;
			tb = ps_c.b;
			m_Font->drawText(ps.c_str(), dx-50, dy,tr,tg,tb, 1, m_Draw);
		}


	}

	if (curCred > -1) {
		if (curCred == m_Creds.size()) {
			return;
		}
		if (curChar == m_Creds[curCred].length()) {
			nextCred();
			return;

		}
				
		int cY = startY;
		int dx = 0;

		int time = clock();
		if (time > nextChar) {
			nextChar = time + 35;
			curChar++;
			auto chan = AudioSystem::ThisAudio->PlaySound(m_Bleep);
			float p = 1.0f;
			p = randomFloatInRange(0.98, 1.02);
			chan->SetPitch(p);
		}

		std::string cur = m_Creds[curCred];
		color cur_col = m_CredsCol[curCred];

		dx = SoftApp::m_This->getWinWidth() / 2;
		int tw = m_Font->getWidth(cur.c_str());
		dx = dx - tw / 2;
		std::string ps = "";
		if (curChar < cur.length()) {
			 ps = cur.substr(0, curChar + 1);
		}
		else {
			ps = cur;
		}
		float tr, tg, tb;
		tr = cur_col.r;
		tg = cur_col.g;
		tb = cur_col.b;


		m_Font->drawText(ps.c_str(), dx-50, cY,tr,tg,tb, 1, m_Draw);

	}
}