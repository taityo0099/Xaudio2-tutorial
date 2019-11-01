#include<xaudio2.h>
#include<wrl.h>
#include <ks.h>
#include <ksmedia.h>
#include<vector>
#include<stdio.h>
#include<crtdbg.h>
#include<Windows.h>


#pragma comment(lib,"xaudio2.lib")

// �X�s�[�J�ꗗ
const unsigned long spk[] = {
	KSAUDIO_SPEAKER_MONO,
	KSAUDIO_SPEAKER_STEREO,
	KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
	KSAUDIO_SPEAKER_QUAD,
	0,
	KSAUDIO_SPEAKER_5POINT1,
	0,
	KSAUDIO_SPEAKER_7POINT1_SURROUND
};

//�T���v�����O���g���@44.1kHz
unsigned int sample = 44.100;

//�ʎq���r�b�g�� 32�r�b�g
unsigned char bit = 32;

//�`�����l���� 2ch
unsigned char channel = 2;

int main()
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT(hr == S_OK);
	{
		Microsoft::WRL::ComPtr<IXAudio2>xaudio2 = nullptr;

		//Xaudio2
		hr = XAudio2Create(&xaudio2, 0);
		_ASSERT(hr == S_OK);

		//�}�X�^�����O�{�C�X
		IXAudio2MasteringVoice* mastaring = nullptr;
		
		hr = xaudio2->CreateMasteringVoice(&mastaring);
		_ASSERT(hr == S_OK);


		//�g�`�f�[�^�̏��
		WAVEFORMATEXTENSIBLE fmt{};
		fmt.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
		fmt.Format.nSamplesPerSec = sample;
		fmt.Format.wBitsPerSample = bit;
		fmt.Format.nChannels = channel;
		fmt.Format.nBlockAlign = fmt.Format.nChannels * fmt.Format.wBitsPerSample / 8;
		fmt.Format.nAvgBytesPerSec = fmt.Format.nSamplesPerSec * fmt.Format.nBlockAlign;
		fmt.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
		fmt.dwChannelMask = spk[fmt.Format.nChannels - 1];
		fmt.Samples.wValidBitsPerSample = fmt.Format.wBitsPerSample;
		fmt.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
		
		//�\�[�X�{�C�X
		IXAudio2SourceVoice* sourceVoice = nullptr;

		hr = xaudio2->CreateSourceVoice(&sourceVoice,0);
		_ASSERT(hr == S_OK);

		


		sourceVoice->DestroyVoice();
		mastaring->DestroyVoice();
	}
	CoUninitialize();
	return 0;
}