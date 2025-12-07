#pragma once

#include "Types/Handle.h"
#include "Base/resources.h"

namespace Simply2D
{
	struct AudioSystemSpecifications
	{
		int freq = 48000;
		int channels = 2;
		float volume = 0.7f;
	};

	class AudioSystem
	{
	public:
		AudioSystem(const AudioSystemSpecifications& specs = AudioSystemSpecifications())
			:	m_specs(specs) { }

		virtual ~AudioSystem() = default;

		virtual void PlaySFX(const THandle<Audio> audio) = 0;
		virtual void PlayMusic(const THandle<Audio> audio, int loop) = 0;
		virtual void StopMusic(const THandle<Audio> audio, int fadeoutMS) = 0;

	protected:
		AudioSystemSpecifications m_specs{};

	private:
		static AudioSystem* Create(const AudioSystemSpecifications& specs);

		friend class Application;
	};
}