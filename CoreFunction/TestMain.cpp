#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include "AudioCapture.h"
#include "AudioProcess.h"
#include "ControlKeyboard.h"

using namespace std;
using namespace csric;

extern "C" __declspec(dllexport) int FreqAnalyze(int* nHistograms, float** value);

__declspec(dllexport) int FreqAnalyze(int* nHistograms, float** value) {
	AudioProcess audioProcess;
	AudioCapture audioCapture;
	ControlKeyboard controlKeyboard;

	bool bDone = false;
	audioCapture.SetUpAudioCapture(&audioProcess);
	//ListenLoop
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nPastTime;
	LARGE_INTEGER nNowTime;
	//keyboard ListenLoop
	LARGE_INTEGER kPastTime;

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&kPastTime);

	*nHistograms = audioProcess.nHistograms;
	*value = new float[audioProcess.nHistograms];
	float* slowValue = new float[audioProcess.nHistograms];
	memset(slowValue, 0, sizeof(float) * *nHistograms);
	while (1) {
		QueryPerformanceCounter(&nPastTime);//获取初始时间
		audioCapture.ListenOnce(&audioProcess);
		audioProcess.CalculateLogFreq(*value);
		//在延时到达设定之前保持循环阻塞
		//不行，这种方式对cpu占用太大了,还是用Sleep()吧..
		/*
		do {
			QueryPerformanceCounter(&nNowTime);
		} while (
			(double)(nNowTime.QuadPart - nPastTime.QuadPart)
			/ (double)nFreq.QuadPart * 1000
			< FRAME_TIME
			);
			*/
		QueryPerformanceCounter(&nNowTime);
		if ((nNowTime.QuadPart - kPastTime.QuadPart) * 1000 / nFreq.QuadPart > KEYBOARD_FRAME_TIME){
			kPastTime = nNowTime;
			for (int i = 0; i < *nHistograms; i++) {
				slowValue[i] = (*value)[i]*0.85 > slowValue[i] ? (*value)[i] : slowValue[i] * SLOW_RATIO;
			}
			controlKeyboard.SetFreqVisualizer_SingleKey(slowValue, *nHistograms);
		}
		QueryPerformanceCounter(&nNowTime);
		int delay = FRAME_TIME - (nNowTime.QuadPart - nPastTime.QuadPart) * 1000 / nFreq.QuadPart;
		if (delay > 0) {
			Sleep(delay);
		}
	}
	delete[] * value;
	return 0;
}

int main() {
	int nHistograms;
	float* value;
	FreqAnalyze(&nHistograms, &value);

	return 0;
}