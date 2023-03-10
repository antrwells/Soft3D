#pragma once
#include "UIControl.h"
#include <queue>
#include "Texture2D.h"
#include <mutex>
#include <thread>
class VideoFrame {
public:
	Texture2D* tex;
	long PTS, PKPTS, DTS;
	int W, H;
	void* Data;
	long clock;
	double DPTS;
	double DDelay;
	long pict;
	int CacheID;


};

class VideoPlayerControl : public UIControl
{
public:
	VideoPlayerControl();

	void SetVideo(const char* path);
	void Play();
	void Stop();
	void Render();
	void StopAudio();
//	void Stop();
	void Pause();
	void DecodeFrame();
	Texture2D* GetCurrentImage();
	VideoFrame* GetCurrentFrame();


	bool gotAudioTime = false;
	double timeDelta = 0.0;
	bool Paused = false;
	long audioStartTime = 0;
	double CLOCK = 0.0;
	double AUDIOCLOCK = 0.0;
	std::queue<VideoFrame*> Frames;
	VideoFrame* CurrentFrame = NULL;
	int Width, Height;
	Texture2D* CurrentTex;
	VideoFrame* UpFrame = NULL;
	long LastTick = 0;
	double timeDelay = 0.0;
	std::mutex frameG;
};




