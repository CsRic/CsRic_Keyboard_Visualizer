#pragma once
#include "AudioProcess.h"

#include <Mmdeviceapi.h>
#include <Audioclient.h>
#include <Audiopolicy.h>
#include <Devicetopology.h>
#include <Endpointvolume.h>

#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

namespace csric {
	class AudioCapture {
	
	private:
		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
		const IID IID_IAudioClient = __uuidof(IAudioClient);
		const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

		REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
		REFERENCE_TIME hnsActualDuration;
		IMMDeviceEnumerator* pEnumerator = NULL;
		IMMDevice* pDevice = NULL;
		IAudioClient* pAudioClient = NULL;
		WAVEFORMATEX* pwfx = NULL;
		UINT32 bufferFrameCount;
		UINT32 numFramesAvailable;
		IAudioCaptureClient* pCaptureClient = NULL;

		UINT32 packetLength = 0;
		BYTE* pData;
		DWORD flags;

		void Exit();
	public:
		~AudioCapture() {
			Exit();
		}
		HRESULT SetUpAudioCapture(AudioProcess* audioProcess);
		HRESULT ListenOnce(AudioProcess* audioProcess);
	};
}
