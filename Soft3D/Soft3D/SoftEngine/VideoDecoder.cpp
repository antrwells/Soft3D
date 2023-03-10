#include "pch.h"
#include "VideoDecoder.h"
#include "VidNative.h"
#include <time.h>
#include "Texture2D.h"
//#include "Application.h"
const char* currentPath;

void VideoDecoder::DecodeThreadBody() {



		if (!IsPaused()) {

			if (GotAudioTime() == false) {

				if (AudioHasBegun())
				{
					GotAudioTime();
					SetAudioStartTime(clock());
				}

			}
			else {

				int curTime = clock();
				int timed = curTime - GetAudioStartTime();
				SetAudioClock(((double)timed) / 1000.0);
			}

		}


		if (Frames.size() < 2) {

			DecodeNextFrame();

		}
		

			if (Frames.size() == 0) {
				return;
				//break;
			}
			VideoFrame* frame = nullptr;
			while (true) {
				frame = Frames.front();
				//Frames.pop();
			//	decod//SetFrames(frames);
				if (frame != nullptr) {
					break;
				}
			}

			if (frame->DPTS < GetAudioClock()) {
				auto f = Frames.front();
				if (f->data != nullptr) {
					free(f->data);
					f->data = nullptr;
				}
				if (f == UpFrame) {
					UpFrame = nullptr;
				}
			//	delete(f);

				Frames.pop();

				
			}

		//auto frames = decoder->GetFrames();

		if (Frames.size() > 0)
		{

			VideoFrame* frame = nullptr;
			while (true) {
				frame = Frames.front();
				if (frame != nullptr) {
					break;
				}
			}

			double dt = frame->DPTS;

			SetTimeDelta(dt - GetAudioClock());

		}

		//frames = decoder->GetFrames();
		


		//Sleep(2);


}

void thr_decode(VideoDecoder* decoder)
{
	//decoder->DecodeThreadBody();

}

void VideoDecoder::DecodeNextFrame() {

	decodeNextFrame(pVideo);
	int fw, fh;
	fw = getFrameWidth(pVideo);
	fh = getFrameHeight(pVideo);

	if (fw <= 0 || fw>10000)
	{
		return;
	}

	genFrameData(pVideo);

	VideoFrame* frame = new VideoFrame;

	frame->W = fw;
	frame->H = fh;

	frame->DPTS = getDPTS(pVideo);
	

	if (frame->DPTS < CLOCK - 0.1)
	{
		delete frame;
		return;
	}
	frames++;
	//char* buf = (char*)malloc(fw * fh * 3);


	char* buf = getFrameData(pVideo);



	char* rbuf = (char*)malloc(fw * fh * 4);

	int i2 = 0;
	int ii = 0;
	for (int y = 0; y < fh; y++) {
		for (int x = 0; x < fw; x++)
		{
			rbuf[ii++] = buf[i2++];
			rbuf[ii++] = buf[i2++];
			rbuf[ii++] = buf[i2++];
			rbuf[ii++] = 255;

		}
	}
//	free(buf);
	frame->data = rbuf;
	frame->DDELAY = getDDelay(pVideo);
	frame->PICT = getPict(pVideo);
	frame->madeTime = clock();
	Frames.push(frame);
	allFrames.push_back(frame);

}

Texture2D* VideoDecoder::GetCurrentImage() {


	DecodeThreadBody();

	std::vector<VideoFrame*> newFrames;
	int time = clock();
	
	for (int i = 0; i < allFrames.size(); i++) {

		if (time > (allFrames[i]->madeTime + 2000))
		{
		//	delete allFrames[i];
		//	printf("Purged frame\n");
		}
		else {
		//	newFrames.push_back(allFrames[i]);
		}

	}

	//allFrames = newFrames;

	if (CurrentFrame == nullptr)
	{
		return nullptr;
	}
	
	
	if (CurrentFrame != UpFrame) {
		
		if (UpFrame != nullptr) {
			delete UpFrame;
		}
		if (UpTex != nullptr) {
		
			delete UpTex;
		}
		Texture2D* new_img = new Texture2D( CurrentFrame->W, CurrentFrame->H, CurrentFrame->data);
		if (CurrentFrame->data != nullptr) {
			free(CurrentFrame->data);
			CurrentFrame->data = nullptr;
			CurrentFrame->data = nullptr;
		}


		UpTex = new_img;
		UpFrame = CurrentFrame;
	}

//	UpFrame = CurrentFrame;


	return UpTex;
}

VideoFrame* VideoDecoder::GetCurrentFrame() {

	int tick = clock();
	if (LastTick == 0) LastTick = tick;

	double ts = ((double)(tick - LastTick)) / 1000.0;
	LastTick = tick;

	CLOCK += ts;
	double dt = CLOCK - TimeDelta;

	if (Frames.size() == 0) {
		return nullptr;
	}

	if (CurrentFrame == nullptr) {
		CurrentFrame = Frames.front();
		Frames.pop();
	}

	if (CurrentFrame->DPTS <= dt) {

	}
	else {
		return CurrentFrame;
	}

	if (CurrentFrame != nullptr) {

		if (Frames.size() > 0)
		{

			if (CurrentFrame->data != nullptr) {
				free(CurrentFrame->data);
				CurrentFrame->data = nullptr;
			}
			if (UpFrame == CurrentFrame) {
				UpFrame = nullptr;
			}
			//delete CurrentFrame;
			


			CurrentFrame = Frames.front();
			Frames.pop();

			double dd = CurrentFrame->DDELAY;
			dd += ((double)CurrentFrame->PICT * (CurrentFrame->DDELAY * 0.5));

		

		}

		return CurrentFrame;
	}
	if (Frames.size() == 0)
	{
		return nullptr;
	}
	
	if (CurrentFrame == nullptr) {
		CurrentFrame = Frames.front();
		Frames.pop();
	}
	return CurrentFrame;
}

VideoDecoder::VideoDecoder() {



}

bool VideoDecoder::OpenVideo(const char* path)
{
		
	currentPath = path;
	mGotAudioTime = false;

	initAudio();
	pVideo = initVideoNative(path);

	time_start = clock();

	//thread_Decode = std::thread(thr_decode, this);
	//thread_Decode.detach();

	return true;
}

bool VideoDecoder::AudioHasBegun() {

	if (audioHasBegun()==1) {
		return true;
	}
	return false;

}

void VideoDecoder::UpdateDecoder() {

	if (GotAudioTime() == false) {

		if (AudioHasBegun()) {
			GotAudioTime();
			mGotAudioTime = true;
			SetAudioStartTime(clock());
		}
		
		
	}
	else {
		int curTime = clock();
		int timed = curTime - GetAudioStartTime();
		SetAudioClock(((double)timed) / 1000.0);
		//printf("AC:%f ----\n", (float)AUDIOCLOCK);
	}

	//printf("Frames:%d\n", Frames.size());
	//printf("AC:%f\n", (float)GetAudioClock());

	if (Frames.size() < 3)
	{
		
		int i = 0;
		while (i < 3) {
			DecodeNextFrame();
			i++;
		}
		
	}

	if (Frames.size() > 0)
	{
		VideoFrame* frame = nullptr;
//		while (true) {
			while (true) {
				if (Frames.size() == 0)
				{
					break;
				}
				frame = Frames.front();
				//Frames.pop();
			//	decod//SetFrames(frames);
				if (frame != nullptr) {
					break;
				}
			}

			if (frame != nullptr) {
				if (frame->DPTS < GetAudioClock()) {
					if (Frames.size() > 0) {
						auto f = Frames.front();

						if (f->data != nullptr) {
							free(f->data);
							f->data = nullptr;
						}
						if (f == UpFrame) {
							UpFrame = nullptr;
						}
						//	delete(f);

						Frames.pop();
					}

				}
				else {
					//break;
				}
			}
			else {
				//break;
			}
		//}
	}
}
bool VideoDecoder::IsVideoDone() {

	return IVideoDone();

}



void VideoDecoder::Restart() {
	
	videoRestart(pVideo);
	SetAudioStartTime(clock());
	CLOCK = 0;
	LastTick = 0;
	TimeDelta = 0;
	time_start = clock();
	Frames.empty();
}

Texture2D* VideoDecoder::GetImage() {

	GetCurrentFrame();

	return GetCurrentImage();

	return nullptr;
}