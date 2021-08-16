#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "AudioCapture.h"
#include "TempMacro.h"

using namespace csric;

void AudioCapture::Exit() {
	pAudioClient->Stop();
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator)
	SAFE_RELEASE(pDevice)
	SAFE_RELEASE(pAudioClient)
	SAFE_RELEASE(pCaptureClient)
}

HRESULT AudioCapture::SetUpAudioCapture(AudioProcess* audioProcess) {
	HRESULT hr;
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator);
	if (FAILED(hr)) { Exit(); return hr; }


	//hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);


	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

	if (FAILED(hr)) { Exit(); return hr; }

	hr = pDevice->Activate(
		IID_IAudioClient, CLSCTX_ALL,
		NULL, (void**)&pAudioClient);
	if (FAILED(hr)) { Exit(); return hr; }

	hr = pAudioClient->GetMixFormat(&pwfx);
	if (FAILED(hr)) { Exit(); return hr; }

	hr = pAudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		//0,
		AUDCLNT_STREAMFLAGS_LOOPBACK,
		hnsRequestedDuration,
		0,
		pwfx,
		NULL);
	if (FAILED(hr)) { Exit(); return hr; }

	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	if (FAILED(hr)) { Exit(); return hr; }

	hr = pAudioClient->GetService(
		IID_IAudioCaptureClient,
		(void**)&pCaptureClient);
	if (FAILED(hr)) { Exit(); return hr; }

	// Notify the audio sink which format to use.
	hr = audioProcess->SetFormat(pwfx);
	if (FAILED(hr)) { Exit(); return hr; }

	// Calculate the actual duration of the allocated buffer.

	hr = pAudioClient->Start();  // Start recording.
	if (FAILED(hr)) { Exit(); return hr; }
	return hr;

}

HRESULT AudioCapture::ListenOnce(AudioProcess* audioProcess) {
	HRESULT hr;
	hr = pCaptureClient->GetNextPacketSize(&packetLength);
	if (FAILED(hr)) { Exit(); return hr; }

	while (packetLength != 0)
	{
		// Get the available data in the shared buffer.
		hr = pCaptureClient->GetBuffer(
			&pData,
			&numFramesAvailable,
			&flags, NULL, NULL);
		if (FAILED(hr)) { Exit(); return hr; }

		if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
		{
			pData = NULL;  // Tell CopyData to write silence.
		}

		// Copy the available capture data to the audio sink.
		hr = audioProcess->EnqueueFrames(pData, numFramesAvailable);
		if (FAILED(hr)) { Exit(); return hr; }
		//audioProcess->PrintStream();
		hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
		if (FAILED(hr)) { Exit(); return hr; }

		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		if (FAILED(hr)) { Exit(); return hr; }

	}
	return 0;
}