#include "pch.h"
#include "VideoPlayerControl.h"
#include "VidNative.h"
#include "Texture2D.h"
#include "UI.h"

Texture2D* curFrame;

bool audioUp = false;

long timeStart = 0;

videoPB* vid;

VideoPlayerControl::VideoPlayerControl() {
	Frames.empty();
};

void VideoPlayerControl::Play() {
	Paused = false;
}

char* pbuf = NULL;
void VideoPlayerControl::DecodeFrame() {

	decodeNextFrame(vid);

	int fw = getFrameWidth(vid);
	int fh = getFrameHeight(vid);

	if (fw < 0 || fw>10000)
	{
		return;
	}
	printf("Decoded Frame. W:%d H:%d \n",fw,fh);



	VideoFrame * frame = new VideoFrame;
	frame->W = fw;
	frame->H = fh;
	Width = fw;
	Height = fh;
	genFrameData(vid);
	frame->DPTS = getDPTS(vid);
	printf("FrameDPTS:%f AC:%f \n", (float)frame->DPTS, (float) AUDIOCLOCK);
	if (frame->DPTS < AUDIOCLOCK - 0.1)
	{
		DecodeFrame();
		return;
	}


//	if (pbuf == NULL) {
		char* buf = (char*)malloc(fw * fh * 3);
		pbuf = buf;
//}


	getFrameData(vid, pbuf);
	frame->Data = (void*) pbuf;
	frame->DDelay = getDelay(vid);
	frame->pict = getPict(vid);

	//std::lock_guard<std::mutex> g1(frameG);
	//frameG.lock();
	//prin/tf("fram")
	VideoFrame* frm = frame;
	//printf("FRM: w%d h%d pts%d", frm->W, frm->H, (int)frm->DPTS);
	Frames.push(frame);
	//f///rameG.unlock();


}


void dec_thread(VideoPlayerControl* th)
{

	while (true) {

	//	printf("Decoding.\n");
		if (!th->Paused) {

			if (th->gotAudioTime == false) {
				if (audioHasBegun() == 1) {
					th->gotAudioTime = true;
					th->audioStartTime = (long)clock();
				}
			}
			else {

				long curTime = (long)clock();
				long timed = curTime - th->audioStartTime;
				th->AUDIOCLOCK = ((double)timed) / 1000.0;
			//	printf("Time:%d \n", (int)th->AUDIOCLOCK);
			}

			while (true) {

				if (th->Frames.size() == 0) {
		
					break;
				}

			
				th->frameG.lock(); 
				VideoFrame* frm = NULL;
				while (true) {
					if (th->Frames.size() > 0) {
						
						frm = th->Frames.front();
					//	printf("FRM: w%d h%d pts%d", frm->W, frm->H, (int)frm->DPTS);
						if (frm != NULL) break;
					}
				}
				//th->frameG.unlock();

				if (frm->DPTS < th->AUDIOCLOCK) {

					//std::lock_guard<std::mutex> guard(th->frameG);
					
					//th->frameG.lock();
					if (th->Frames.size() > 0) {
				
						th->Frames.pop();

					}

					//th->frameG.unlock();

				}
				else {
					th->frameG.unlock();
					break;
				}
				th->frameG.unlock();


			}

			if (th->Frames.size() > 0)
			{


				//std::lock_guard<std::mutex> g1(th->frameG);
				th->frameG.lock();

				VideoFrame* frm = th->Frames.front();
				double ft = frm->DPTS;
				th->frameG.unlock();
				th->timeDelta = ft - th->AUDIOCLOCK;

			}


			//std::lock_guard<std::mutex> g2(th->frameG);
			th->frameG.lock();
			if (th->Frames.size() < 800)
			{
				th->DecodeFrame();
			}
			th->frameG.unlock();

		}

	}

//	printf("Leaving decode thread.\n");
}

Texture2D* VideoPlayerControl::GetCurrentImage() {

	int used = 0;
	if (CurrentFrame ==NULL) {

		return NULL;
	}
	if (CurrentTex == NULL) CurrentTex = new Texture2D(Width, Height, false);

	if (CurrentFrame == UpFrame) {
		return CurrentTex;
	}
	CurrentTex->LoadSub((char*)CurrentFrame->Data);
	UpFrame = CurrentFrame;
	return CurrentTex;
}

VideoFrame* VideoPlayerControl::GetCurrentFrame() {

	long ctick = (long)clock();

	if (LastTick == 0) {

		LastTick = ctick;
	}

	if (Paused) {

		LastTick = ctick;
		return CurrentFrame;
	
	}

	double ts = (double)(ctick - LastTick) / 1000.0;

	LastTick = ctick;

	double dt = CLOCK - timeDelta;


	if (Frames.size() <4) {
		DecodeFrame();
	//	printf("Decoding frame.\n-----");
	}

	
		if (gotAudioTime == false) {
			if (audioHasBegun() == 1) {
				gotAudioTime = true;
				audioStartTime = (long)clock();
			}
		}
		else {

			long curTime = (long)clock();
			long timed = curTime - audioStartTime;
			AUDIOCLOCK = ((double)timed) / 1000.0;
			//printf("Time:%d \n", (int)AUDIOCLOCK);
		}

	if (CurrentFrame != NULL) {
		
		
		//printf("CFTime:%f VS AC:%f",(float)CurrentFrame->DPTS, (float)AUDIOCLOCK);
		if (CurrentFrame->DPTS < AUDIOCLOCK)
		{
			//printf("Freeing frame:\n<<<<<<<<<<<<<<<<<<<");
			free(CurrentFrame->Data);
			delete CurrentFrame;
			//free(Current)

			CurrentFrame = NULL;
		}
	}
	if (Frames.size() == 0) return NULL;

	if (CurrentFrame == NULL) {

		if (Frames.size() == 0) return NULL;
	//	frameG.lock();
		CurrentFrame = Frames.front();
		Frames.pop();
		//printf("Set Current Frame!!!!!!!!");
	//	frameG.unlock();
		double dd = (double)CurrentFrame->DDelay;
		dd += ((double)CurrentFrame->pict * (CurrentFrame->DDelay * 0.5));
		timeDelay = dd;

		dd = dd * 50;
		return CurrentFrame;


	}
	if (Frames.size() == 0) {

		return NULL;
	}

	if (CurrentFrame == NULL) {

		//frameG.lock();
		CurrentFrame = Frames.front();
		Frames.pop();
		//frameG.unlock();

	}
	return CurrentFrame;

}

void VideoPlayerControl::Pause() {

	Paused = true;

}


void VideoPlayerControl::SetVideo(const char* path) {

	//return true;
	if (!audioUp) {
		initAudio();
	}

	vid = initVideoNative(path);


	timeStart = (long)clock();
	Frames.empty();

//	std::thread decode_thread(dec_thread, this);
//	decode_thread.detach();

}

Texture2D* ctex=NULL;

void VideoPlayerControl::Render() {

	UI::RenderText(20, 20, "Testing Video.\n", 1, 1, 1, 1);
	CurrentFrame = GetCurrentFrame();
	Texture2D* curImg = GetCurrentImage();
	if (curImg != NULL) {
		ctex = curImg;
		int vv = 0;
	}
	if (ctex != NULL) {
		UI::DrawTexture(ctex, GetX(), GetY(), GetW(), GetH(), 1, 1, 1, 1);
	}
	



}