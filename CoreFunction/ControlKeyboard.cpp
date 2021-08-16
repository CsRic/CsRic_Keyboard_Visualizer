#include "ControlKeyboard.h"
#include <array>
#include <chrono>
#include <conio.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <time.h>
#include <thread>
using namespace csric;
using namespace std;

KeyNPositionX key_n_position_x[87] = {
//第一行
{RZKEY::RZKEY_ESC,0},
{RZKEY::RZKEY_F1,2},
{RZKEY::RZKEY_F2,3},
{RZKEY::RZKEY_F3,4},
{RZKEY::RZKEY_F4,5},
{RZKEY::RZKEY_F5,6.5},
{RZKEY::RZKEY_F6,7.5},
{RZKEY::RZKEY_F7,8.5},
{RZKEY::RZKEY_F8,9.5},
{RZKEY::RZKEY_F9,11},
{RZKEY::RZKEY_F10,12},
{RZKEY::RZKEY_F11,13},
{RZKEY::RZKEY_F12,14},
{RZKEY::RZKEY_PRINTSCREEN,15.5},
{RZKEY::RZKEY_SCROLL,16.5},
{RZKEY::RZKEY_PAUSE,17.5},
//第二行
{RZKEY::RZKEY_OEM_1,0},
{RZKEY::RZKEY_1,1},
{RZKEY::RZKEY_2,2},
{RZKEY::RZKEY_3,3},
{RZKEY::RZKEY_4,4},
{RZKEY::RZKEY_5,5},
{RZKEY::RZKEY_6,6},
{RZKEY::RZKEY_7,7},
{RZKEY::RZKEY_8,8},
{RZKEY::RZKEY_9,9},
{RZKEY::RZKEY_0,10},
{RZKEY::RZKEY_OEM_2,11},
{RZKEY::RZKEY_OEM_3,12},
{RZKEY::RZKEY_BACKSPACE,13.5},
{RZKEY::RZKEY_INSERT,15.5},
{RZKEY::RZKEY_HOME,16.5},
{RZKEY::RZKEY_PAGEUP,17.5},
//第三行
{RZKEY::RZKEY_TAB,0.3},
{RZKEY::RZKEY_Q,1.5},
{RZKEY::RZKEY_W,2.5},
{RZKEY::RZKEY_E,3.5},
{RZKEY::RZKEY_R,4.5},
{RZKEY::RZKEY_T,5.5},
{RZKEY::RZKEY_Y,6.5},
{RZKEY::RZKEY_U,7.5},
{RZKEY::RZKEY_I,8.5},
{RZKEY::RZKEY_O,9.5},
{RZKEY::RZKEY_P,10.5},
{RZKEY::RZKEY_OEM_4,11.5},
{RZKEY::RZKEY_OEM_5,12.5},
{RZKEY::RZKEY_OEM_6,13.6},
{RZKEY::RZKEY_DELETE,15.5},
{RZKEY::RZKEY_END,16.5},
{RZKEY::RZKEY_PAGEDOWN,17.5},
//第四行
{RZKEY::RZKEY_CAPSLOCK,0.35},
{RZKEY::RZKEY_A,1.7},
{RZKEY::RZKEY_S,2.7},
{RZKEY::RZKEY_D,3.7},
{RZKEY::RZKEY_F,4.7},
{RZKEY::RZKEY_G,5.7},
{RZKEY::RZKEY_H,6.7},
{RZKEY::RZKEY_J,7.7},
{RZKEY::RZKEY_K,8.7},
{RZKEY::RZKEY_L,9.7},
{RZKEY::RZKEY_OEM_7,10.7},
{RZKEY::RZKEY_OEM_8,11.7},
{RZKEY::RZKEY_ENTER,13.5},
//第五行
{RZKEY::RZKEY_LSHIFT,0.5},
{RZKEY::RZKEY_Z,2.2},
{RZKEY::RZKEY_X,3.2},
{RZKEY::RZKEY_C,4.2},
{RZKEY::RZKEY_V,5.2},
{RZKEY::RZKEY_B,6.2},
{RZKEY::RZKEY_N,7.2},
{RZKEY::RZKEY_M,8.2},
{RZKEY::RZKEY_OEM_9,9.2},
{RZKEY::RZKEY_OEM_10,10.2},
{RZKEY::RZKEY_OEM_11,11.2},
{RZKEY::RZKEY_RSHIFT,13.2},
{RZKEY::RZKEY_UP,16.5},
//第六行
{RZKEY::RZKEY_LCTRL,0},
{RZKEY::RZKEY_LWIN,1.3},
{RZKEY::RZKEY_LALT,2.4},
{RZKEY::RZKEY_SPACE,6.4},
{RZKEY::RZKEY_RALT,10.2},
{RZKEY::RZKEY_FN,11.3},
{RZKEY::RZKEY_RMENU,12.7},
{RZKEY::RZKEY_RCTRL,13.9},
{RZKEY::RZKEY_LEFT,15.5},
{RZKEY::RZKEY_DOWN,16.5},
{RZKEY::RZKEY_RIGHT,17.5}
};

int KeyQueue[87] = {
	//第六行
	RZKEY::RZKEY_LCTRL,
	RZKEY::RZKEY_LWIN,
	RZKEY::RZKEY_LALT,
	RZKEY::RZKEY_SPACE,
	RZKEY::RZKEY_RALT,
	RZKEY::RZKEY_FN,
	RZKEY::RZKEY_RMENU,
	RZKEY::RZKEY_RCTRL,
	RZKEY::RZKEY_LEFT,
	RZKEY::RZKEY_DOWN,
	RZKEY::RZKEY_RIGHT,
	//第五行
	RZKEY::RZKEY_LSHIFT,
	RZKEY::RZKEY_Z,
	RZKEY::RZKEY_X,
	RZKEY::RZKEY_C,
	RZKEY::RZKEY_V,
	RZKEY::RZKEY_B,
	RZKEY::RZKEY_N,
	RZKEY::RZKEY_M,
	RZKEY::RZKEY_OEM_9,
	RZKEY::RZKEY_OEM_10,
	RZKEY::RZKEY_OEM_11,
	RZKEY::RZKEY_RSHIFT,
	RZKEY::RZKEY_UP,
	//第四行
	RZKEY::RZKEY_CAPSLOCK,
	RZKEY::RZKEY_A,
	RZKEY::RZKEY_S,
	RZKEY::RZKEY_D,
	RZKEY::RZKEY_F,
	RZKEY::RZKEY_G,
	RZKEY::RZKEY_H,
	RZKEY::RZKEY_J,
	RZKEY::RZKEY_K,
	RZKEY::RZKEY_L,
	RZKEY::RZKEY_OEM_7,
	RZKEY::RZKEY_OEM_8,
	RZKEY::RZKEY_ENTER,
	//第三行
	RZKEY::RZKEY_TAB,
	RZKEY::RZKEY_Q,
	RZKEY::RZKEY_W,
	RZKEY::RZKEY_E,
	RZKEY::RZKEY_R,
	RZKEY::RZKEY_T,
	RZKEY::RZKEY_Y,
	RZKEY::RZKEY_U,
	RZKEY::RZKEY_I,
	RZKEY::RZKEY_O,
	RZKEY::RZKEY_P,
	RZKEY::RZKEY_OEM_4,
	RZKEY::RZKEY_OEM_5,
	RZKEY::RZKEY_OEM_6,
	RZKEY::RZKEY_DELETE,
	RZKEY::RZKEY_END,
	RZKEY::RZKEY_PAGEDOWN,
	//第二行
	RZKEY::RZKEY_OEM_1,
	RZKEY::RZKEY_1,
	RZKEY::RZKEY_2,
	RZKEY::RZKEY_3,
	RZKEY::RZKEY_4,
	RZKEY::RZKEY_5,
	RZKEY::RZKEY_6,
	RZKEY::RZKEY_7,
	RZKEY::RZKEY_8,
	RZKEY::RZKEY_9,
	RZKEY::RZKEY_0,
	RZKEY::RZKEY_OEM_2,
	RZKEY::RZKEY_OEM_3,
	RZKEY::RZKEY_BACKSPACE,
	RZKEY::RZKEY_INSERT,
	RZKEY::RZKEY_HOME,
	RZKEY::RZKEY_PAGEUP,
	//第一行
	RZKEY::RZKEY_ESC,
	RZKEY::RZKEY_F1,
	RZKEY::RZKEY_F2,
	RZKEY::RZKEY_F3,
	RZKEY::RZKEY_F4,
	RZKEY::RZKEY_F5,
	RZKEY::RZKEY_F6,
	RZKEY::RZKEY_F7,
	RZKEY::RZKEY_F8,
	RZKEY::RZKEY_F9,
	RZKEY::RZKEY_F10,
	RZKEY::RZKEY_F11,
	RZKEY::RZKEY_F12,
	RZKEY::RZKEY_PRINTSCREEN,
	RZKEY::RZKEY_SCROLL,
	RZKEY::RZKEY_PAUSE
};

const int ControlKeyboard::GetColorArraySize2D(EChromaSDKDevice2DEnum device)
{
	const int maxRow = ChromaAnimationAPI::GetMaxRow((int)device);
	const int maxColumn = ChromaAnimationAPI::GetMaxColumn((int)device);
	return maxRow * maxColumn;
}

void ControlKeyboard::SetFreqVisualizer_FullHorizonal(float* value, int length) {
	
	for (int i = 0; i < nkeys; i++) {
		float pos = key_n_position_x[i].x;
		pos *= (length - 0.5);
		pos /= keyboard_length;
		int posL = (int)pos;
		int posR = posL + 1;
		float toL = pos - posL;
		float toR = posR - pos;
		if (posR >= length) { posR = posL;}
		float trueValue = toL * value[posL] + toR * value[posR];
		const int thres = 30;
		if (trueValue > thres)trueValue += trueValue - thres;
		trueValue /= (100 - thres);
		
		trueValue -= 0.2;
		if (trueValue < 0.01)trueValue = 0.0001;
		Color finalColor = {
			trueValue * color1.R + (1 - trueValue) * color2.R,
			trueValue * color1.G + (1 - trueValue) * color2.G,
			trueValue * color1.B + (1 - trueValue) * color2.B
		};
		int finalColorInt = ChromaAnimationAPI::GetRGB(finalColor.R, finalColor.G, finalColor.B);
		colorsKeyboard[Keyboard::MAX_COLUMN * HIBYTE(key_n_position_x[i].key) + LOBYTE(key_n_position_x[i].key)] = finalColorInt;
	}
	ChromaAnimationAPI::SetCustomColorFlag2D((int)EChromaSDKDevice2DEnum::DE_Keyboard, colorsKeyboard);
	ChromaAnimationAPI::SetEffectKeyboardCustom2D((int)EChromaSDKDevice2DEnum::DE_Keyboard, colorsKeyboard);
}

void ControlKeyboard::Init() {
	if (ChromaAnimationAPI::InitAPI() != 0)
	{
		cerr << "Failed to load Chroma library!" << endl;
		exit(1);
	}

	ChromaSDK::APPINFOTYPE appInfo = {};

	_tcscpy_s(appInfo.Title, 256, _T("CsRic Keyboard Visualizer"));
	_tcscpy_s(appInfo.Description, 1024, _T("Freq-Visualizer using Razer Chroma SDK"));
	_tcscpy_s(appInfo.Author.Name, 256, _T("CsRic"));
	_tcscpy_s(appInfo.Author.Contact, 256, _T("https://github.com/CSRic"));
	appInfo.SupportedDevice = (0x01);

	appInfo.Category = 1;

	RZRESULT result = ChromaAnimationAPI::InitSDK(&appInfo);
	if (result != RZRESULT_SUCCESS)
	{
		cerr << "Failed to initialize Chroma!" << endl;
		exit(1);
	}
	Sleep(100); //wait for init

	nkeys = sizeof(key_n_position_x) / sizeof(KeyNPositionX);
}
void ControlKeyboard::SetFreqVisualizer_SingleKey(float* value, int length) {
	float expand = (float)length / (float)(nkeys);
	for (int i = 0; i < nkeys; i++) {
		int pos = (int)(i * expand);
		if (pos >= length)pos = length - 1;
		float trueValue = value[pos];
		const int thres = 30;
		if (trueValue > thres)trueValue += trueValue - thres;
		trueValue /= (100 - thres);
		trueValue -= 0.1;
		if (trueValue < 0.01)trueValue = 0.0001;
		if (trueValue > 0.9999)trueValue = 0.9999;
		Color finalColor = {
			trueValue * color1.R + (1 - trueValue) * color2.R,
			trueValue * color1.G + (1 - trueValue) * color2.G,
			trueValue * color1.B + (1 - trueValue) * color2.B
		};
		int finalColorInt = ChromaAnimationAPI::GetRGB(finalColor.R, finalColor.G, finalColor.B);
		colorsKeyboard[Keyboard::MAX_COLUMN * HIBYTE(KeyQueue[i]) + LOBYTE(KeyQueue[i])] = finalColorInt;
	}
	ChromaAnimationAPI::SetCustomColorFlag2D((int)EChromaSDKDevice2DEnum::DE_Keyboard, colorsKeyboard);
	ChromaAnimationAPI::SetEffectKeyboardCustom2D((int)EChromaSDKDevice2DEnum::DE_Keyboard, colorsKeyboard);
}