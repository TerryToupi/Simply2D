#pragma once

#include "Audio/audio.h"

#include "Types/HashMap.h"

#include <SDL3_Mixer/SDL_mixer.h>

namespace Simply2D
{
	class AudioSystemImpl final : public AudioSystem
	{
	public:
		AudioSystemImpl(const AudioSystemSpecifications& specs);
		~AudioSystemImpl();

		void virtual PlaySFX(THandle<Audio> audio) override;
		void virtual PlayMusic(THandle<Audio> audio, int loop) override;
		virtual void StopMusic(THandle<Audio> audio, int fadeoutMS) override;

		MIX_Mixer* GetMixer() const { return m_pMixer; }

	private:
		MIX_Mixer* m_pMixer = nullptr;
		THashMap<uint32_t, MIX_Track*> m_tracks;
	};
}