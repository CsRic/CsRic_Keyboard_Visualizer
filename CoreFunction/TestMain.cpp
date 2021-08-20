#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include <thread>
#include "AudioCapture.h"
#include "AudioProcess.h"
#include "ControlKeyboard.h"

using namespace std;
using namespace csric;


int G_nHistograms;
float* G_slowValue;
AudioProcess audioProcess;
AudioCapture audioCapture;
ControlKeyboard controlKeyboard;

//bass detect
float G_bassPeak;

void ConstrolKeyboardFunction() {
	//ListenLoop
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nPastTime;
	LARGE_INTEGER nNowTime;
	QueryPerformanceFrequency(&nFreq);
	while (1) {
		QueryPerformanceCounter(&nPastTime);//获取初始时间
		//controlKeyboard.SetFreqVisualizer_SingleKey(G_slowValue, G_nHistograms);
		controlKeyboard.SetFreqVisualizer_FullHorizonal(G_slowValue, G_nHistograms);
		controlKeyboard.IterateColorFrame(0.01 * G_bassPeak, 2);
		controlKeyboard.IterateColorFrame(-0.05 * G_bassPeak, 1);
		
		QueryPerformanceCounter(&nNowTime);
		int delay = KEYBOARD_FRAME_TIME - (nNowTime.QuadPart - nPastTime.QuadPart) * 1000 / nFreq.QuadPart;
		if (delay > 0) {
			Sleep(delay);
		}
	}
}

extern "C" __declspec(dllexport) int FreqAnalyze(int* nHistograms, float** value);
__declspec(dllexport) int FreqAnalyze(int* nHistograms, float** value) {
	bool bDone = false;
	audioCapture.SetUpAudioCapture(&audioProcess);
	//ListenLoop
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nPastTime;
	LARGE_INTEGER nNowTime;

	QueryPerformanceFrequency(&nFreq);
	*nHistograms = audioProcess.nHistograms;
	G_nHistograms = *nHistograms;
	*value = new float[audioProcess.nHistograms];
	G_slowValue = new float[audioProcess.nHistograms];
	memset(G_slowValue, 0, sizeof(float) * *nHistograms);
	std::thread thread1(ConstrolKeyboardFunction);
	while (1) {
		QueryPerformanceCounter(&nPastTime);//获取初始时间
		audioCapture.ListenOnce(&audioProcess);
		audioProcess.CalculateLogFreq(*value);
		for (int i = 0; i < *nHistograms; i++) {
			G_slowValue[i] = (*value)[i]*0.85 > G_slowValue[i] ? (*value)[i] : G_slowValue[i] * SLOW_RATIO;
		}
		G_bassPeak = 0;
		for (int i = 0; i < *nHistograms / 7; i++) {
			G_bassPeak += (*value)[i];
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
	G_bassPeak = 0;
	FreqAnalyze(&nHistograms, &value);
	return 0;
}