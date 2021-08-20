#pragma once
#ifndef _CONTROL_KEYBOARD
#define _CONTROL_KEYBOARD
#include "ChromaAnimationAPI.h"
using namespace ChromaSDK;
using Keyboard::RZKEY;
namespace csric {

	struct KeyNPositionX {
		int key;
		float x;
	};
	struct Color {
		int R = 0;
		int G = 0;
		int B = 0;
	};
	
	

	class ControlKeyboard {
	private:
		Color color1;
		Color color2;
		int nkeys;
		int keyboard_length = 18.5;
		int sizeKeyboard;
		int *colorsKeyboard;
		int *tempColorsKeyboard;
		const int GetColorArraySize2D(EChromaSDKDevice2DEnum device);

	public:
		~ControlKeyboard() {
			if (colorsKeyboard)
				delete[] colorsKeyboard;
			if (tempColorsKeyboard)
				delete[] tempColorsKeyboard;
		}
		ControlKeyboard() {
			Init();
			sizeKeyboard = GetColorArraySize2D(EChromaSDKDevice2DEnum::DE_Keyboard);
			colorsKeyboard = new int[sizeKeyboard];
			memset(colorsKeyboard, 0, sizeof(int) * sizeKeyboard);
			tempColorsKeyboard = new int[sizeKeyboard];

			color1 = { 0,140,230 };
			color2 = { 100,0,0 };
			keyboard_length = 19;
		}
		void SetFreqVisualizer_FullHorizonal(float* value, int length);
		void SetFreqVisualizer_SingleKey(float* value, int length);
		void Init();
	};
}

#endif