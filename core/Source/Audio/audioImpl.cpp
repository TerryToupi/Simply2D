#include "Base/app.h"
#include "Source/Audio/audioImpl.h"
#include "Source/Base/assetDataBase.h"

#include "Memory/memory.h"
#include "Types/SmartPointers.h"

namespace Simply2D
{

	AudioSystem* AudioSystem::Create(const AudioSystemSpecifications& specs)
	{
		return MM::New<AudioSystemImpl>(specs);
	}

	AudioSystemImpl::AudioSystemImpl(const AudioSystemSpecifications& specs)
		:	AudioSystem(specs)
	{
		if (!MIX_Init())
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to initialize %s", SDL_GetError());

		SDL_AudioSpec mixerSpecs;
		mixerSpecs.freq = m_specs.freq;
		mixerSpecs.channels = m_specs.channels;
		mixerSpecs.format = SDL_AUDIO_F32;

		m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &mixerSpecs);
		MIX_SetMasterGain(m_pMixer, specs.volume);
	}

	AudioSystemImpl::~AudioSystemImpl()
	{
		MIX_DestroyMixer(m_pMixer);
		MIX_Quit();
	}

	void AudioSystemImpl::PlaySFX(THandle<Audio> audio)
	{
		auto assets = static_cast<AssetDatabaseImpl*>(Application::GetAssetDatabase());

		MIX_Audio* maudio = assets->getAudio(audio);
		if (maudio)
		{
			if(!MIX_PlayAudio(m_pMixer, maudio))
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to play sfx %s", SDL_GetError());
		}
	}

	void AudioSystemImpl::PlayMusic(THandle<Audio> audio, int loop)
	{ 
		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loop);

		MIX_Track* track = nullptr;

		auto cached = m_tracks.find(audio.Pack());
		if (cached == m_tracks.end())
		{
			auto assets = static_cast<AssetDatabaseImpl*>(Application::GetAssetDatabase());
			MIX_Audio* maudio = assets->getAudio(audio);
			if (maudio)
			{
				track = MIX_CreateTrack(m_pMixer);
				MIX_SetTrackAudio(track, maudio);
				m_tracks[audio.Pack()] = track;
			}
		}
		else
		{
			track = m_tracks[audio.Pack()];
		}

		if(!MIX_PlayTrack(track, props))
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to play music %s", SDL_GetError());
	}

	void AudioSystemImpl::StopMusic(THandle<Audio> audio, int fadeoutMS)
	{
		auto cached = m_tracks.find(audio.Pack());
		if (cached != m_tracks.end())
		{
			auto assets = static_cast<AssetDatabaseImpl*>(Application::GetAssetDatabase());
			MIX_Audio* maudio = assets->getAudio(audio);
			if (maudio)
			{
				MIX_Track* track = m_tracks[audio.Pack()];
				Sint64 fadeFrames = MIX_AudioMSToFrames(maudio, fadeoutMS);

				if(!MIX_StopTrack(track, fadeFrames))
					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to stop music %s", SDL_GetError());

				MIX_DestroyTrack(track);
			}

			m_tracks.erase(audio.Pack());
		}
	}
}

