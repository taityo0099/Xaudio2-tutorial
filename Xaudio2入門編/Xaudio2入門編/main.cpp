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
		//�I�[�f�B�I
		Microsoft::WRL::ComPtr<IXAudio2>audio = nullptr;
		{
			//Xaudio2
			hr = XAudio2Create(&audio);
			_ASSERT(hr == S_OK);
		}

		//�}�X�^�����O�{�C�X
		IXAudio2MasteringVoice* mastaring = nullptr;
		{
			hr = audio->CreateMasteringVoice(&mastaring);
			_ASSERT(hr == S_OK);
		}

		//�\�[�X�{�C�X
		IXAudio2SourceVoice* sourceVoice = nullptr;
		{
			//�g�`�f�[�^�̏��
			WAVEFORMATEXTENSIBLE fmt{};
			fmt.Format.cbSize			= sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
			fmt.Format.nSamplesPerSec	= sample;
			fmt.Format.wBitsPerSample	= bit;
			fmt.Format.nChannels		= channel;
			fmt.Format.nBlockAlign		= fmt.Format.nChannels * fmt.Format.wBitsPerSample / 8;
			fmt.Format.nAvgBytesPerSec	= fmt.Format.nSamplesPerSec * fmt.Format.nBlockAlign;
			fmt.Format.wFormatTag		= WAVE_FORMAT_EXTENSIBLE;
			fmt.dwChannelMask			= spk[fmt.Format.nChannels - 1];
			fmt.Samples.wValidBitsPerSample	= fmt.Format.wBitsPerSample;
			fmt.SubFormat				= KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

			hr = audio->CreateSourceVoice(&sourceVoice, (WAVEFORMATEX*)&fmt,0,1.0f,nullptr);
			_ASSERT(hr == S_OK);
		}
		
		//�����̉��f�[�^
		std::vector<float>R(sample * channel);
		{
			for (size_t i = 0; i < R.size();i += channel)
			{
				R[i] = std::sin(2.0f * std::acos(-1.0f) * 440.0f * i / float(sample));
				R[i + 1] = R[i];
			}
		}

		//�g�`�f�[�^���o�b�t�@�ɍڂ���
		{
			XAUDIO2_BUFFER buffer{};
			buffer.AudioBytes = sizeof(R[0] * R.size());
			buffer.pAudioData = (unsigned char*)R.data();
			hr = sourceVoice->SubmitSourceBuffer(&buffer);
			_ASSERT(hr == S_OK);
		}
		//�Đ�����
		{
			hr = sourceVoice->Start();
			_ASSERT(hr == S_OK);
		}
		Sleep(10000);
		{
			hr = sourceVoice->Stop();
			_ASSERT(hr == S_OK);
		}

		sourceVoice->DestroyVoice();
		mastaring->DestroyVoice();
		//SAFE_RELEASE(audio);
	}
	CoUninitialize();
	return 0;
}