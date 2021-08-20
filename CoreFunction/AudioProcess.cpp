#include <stdio.h>
#include <math.h>
#include "AudioProcess.h"
using namespace csric;
#define PI 3.14159265

HRESULT AudioProcess::SetFormat(WAVEFORMATEX* pwfx) {
	waveFormatex = *pwfx;
	printf("wFormatTag = %d\n", pwfx->wFormatTag);
	printf("nChannels = %d\n", pwfx->nChannels);
	printf("nSamplesPerSec = %d\n", pwfx->nSamplesPerSec);
	printf("nAvgBytesPerSec = %d\n", pwfx->nAvgBytesPerSec);
	printf("nBlockAlign = %d\n", pwfx->nBlockAlign);
	printf("wBitsPerSample = %d\n", pwfx->wBitsPerSample);
	printf("cbSize = %d\n\n", pwfx->cbSize);
	
	//for CalculateLogFreq()
	density = FREQ_DENSITY;
	octaves = FREQ_OCTAVES;
	nHistograms = density * octaves;
	Q = 1.0f / (powf(2.0, 1.0 / density) - 1);

	//set window length
	windows = new int[nHistograms];
	for (int i = 0; i < nHistograms; i++) {
		windows[i] = (int)(1 + Q * waveFormatex.nSamplesPerSec / (freqMin * powf(2.0, (float)i / (float)density)));
		printf("windows[%d]: %d\n", i, windows[i]);
	}

	//misc
	addWeight = new float[nHistograms];
	for (int i = 0; i < nHistograms; i++) {
		int fix = (70 + nHistograms / 5.0) * 3.0;
		if (i < nHistograms * 3 / 5) {
			float phase = i;
			addWeight[i] = (1.4 - 0.7 * cosf(2 * PI * phase / (nHistograms * 0.9))) * fix;
		}
		else{
			addWeight[i] = (2.1) * fix;
		}
	}
	return 0;
}
HRESULT AudioProcess::EnqueueFrames(BYTE* data, UINT32 numFramesAvailable) {
	if (!numFramesAvailable)return 0;
	if (waveFormatex.nChannels == 2) {
		if (waveFormatex.nBlockAlign == 8) {//IEEE format float
			if (!data) {
				for (UINT32 i = 0; i < numFramesAvailable; i++) {
					UINT32 pos = (i + queueHead) % queueMaxLength;
					lPointDataQueue[pos] = 0;
					rPointDataQueue[pos] = 0;
				}
			}
			else {
				for (UINT32 i = 0; i < numFramesAvailable; i++) {
					unsigned char* p = (unsigned char*)lPointDataQueue;
					UINT32 pos = (i + queueHead) % queueMaxLength;
					p[4 * pos + 0] = data[8 * i + 0];
					p[4 * pos + 1] = data[8 * i + 1];
					p[4 * pos + 2] = data[8 * i + 2];
					p[4 * pos + 3] = data[8 * i + 3];
					p = (unsigned char*)rPointDataQueue;
					p[4 * pos + 0] = data[8 * i + 4];
					p[4 * pos + 1] = data[8 * i + 5];
					p[4 * pos + 2] = data[8 * i + 6];
					p[4 * pos + 3] = data[8 * i + 7];
				}
			}
		}
	}
	queueHead += numFramesAvailable;
	queueHead %= queueMaxLength;
	return 1;
}

void AudioProcess::PrintStream() {
	if (waveFormatex.nChannels == 2) {
		if (waveFormatex.nBlockAlign == 8) {
			float leftMax = 0;
			float rightMax = 0;
			for (UINT32 i = 0; i < 512; i++) {
				int pos = (queueMaxLength + queueHead - 512 + i) % queueMaxLength;
				leftMax = fabs(lPointDataQueue[pos]) > leftMax ? fabs(lPointDataQueue[pos]) : leftMax;
				rightMax = fabs(rPointDataQueue[pos]) > rightMax ? fabs(rPointDataQueue[pos]) : rightMax;
			}
			int l = int(leftMax * 32);
			int r = int(rightMax * 32);
			int i;
			for (i = 0; i < 32 - l; i++)printf(" ");
			for (; i < 32; i++)printf("_");
			printf("|");
			for (i = 0; i < r; i++)printf("_");
			printf("\n");
		}
	}
}

float LegendaryRSqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5f;
	x2 = number * 0.5f;
	y = number;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));
	return y;
}


/*
	ver 1.1
	Constant Q Transform
	使用Blackman窗进行加权
	ref:
		https://www.jianshu.com/p/53c93947c417
*/
void AudioProcess::CalculateLogFreq(float* value) {
	//memset(value, 0, nHistograms * sizeof(float));
	double coef_1 = 2 * PI * Q;
	for (int k = nHistograms - 1; k >= 0; k--) {
		int repeatTime = 1 + FRAME_TIME * waveFormatex.nSamplesPerSec / windows[k] / 2000;
		int window = windows[k] * repeatTime;
		double coef_2 = coef_1 / windows[k];
		double cosPart = 0;
		double sinPart = 0;
		double windowCoef = 2.0 * PI / ((double)window - 1);
		for (int m = 0; m < window; m++) {
			int pos = (queueMaxLength - window + queueHead + m) % queueMaxLength;
			double coef_3 = ((double)lPointDataQueue[pos] + (double)rPointDataQueue[pos]) * //X(m)
				(0.42 - 0.50 * cos(windowCoef * m)
					+ 0.16 * (windowCoef * m));  //W_Nk(m), Blackman
			cosPart += coef_3 * cos(coef_2 * m);
			sinPart += coef_3 * sin(coef_2 * m);
		}
		double temp = 1 / LegendaryRSqrt(cosPart * cosPart + sinPart * sinPart);
		temp /= window;
		if (temp < VALUE_THRESHOLD) temp = 0;
		value[k] = (float)temp * addWeight[k];
		if (value[k] > 99.9f)value[k] = 99.9f;
	}
}


/*
		ver 1.0
		density = 4
		36 histograms
		32Hz, , , ,64Hz, , , ,128Hz, , , , ... ,8192Hz, , , .
	*/
/*
void AudioProcess::CalculateLogFreq(float* value) {
	

	float trueBoost = 1 + highBoost / density;
	memset(value, 0, nHistograms * sizeof(float));
	
	for (int i = 0; i < octaves; i++) {
		float coef_1 = powf(2.0, (float)i) * 2.0 * PI;
		for (int j = 0; j < density; j++) {
			float v1 = 0;
			float v2 = 0;
			float coef_2 = coef_1 / (float)bins[j];
			for (int k = 0; k <  bins[j]; k++) {
				int pos = (queueMaxLength -k + queueHead) % queueMaxLength;
				float angle = coef_2 * k;
				float x = lPointDataQueue[pos] + rPointDataQueue[pos];
				v1 += x * cosf(angle);
				v2 += x * sinf(angle);
			}
			v1 /= bins[j];
			v2 /= bins[j];
			value[density * i + j] = sqrtf(v1 * v1 + v2 * v2) * powf(trueBoost, density * i + j);
		}
	}
	
}*/


