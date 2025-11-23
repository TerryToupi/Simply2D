#pragma once

#include <optional>

#include "Base/assets.h"

#include "tileSet.h"
#include "tileLayers.h"
#include "sprite.h"

#include "Types/Arrays.h"
#include "Types/Deque.h"
#include "Types/SmartPointers.h"

namespace Simply2D
{
	class Scene
	{
	public:
		Scene(Asset level);
		virtual ~Scene() = default;

		// Scripting functions
		virtual void begin(float ts)	{}
		virtual void event()			{}
		virtual void update(float ts)	{} 
		virtual void render()			{}
		virtual void end(float ts)		{}

		// registering
		void registerSprite(Sprite* sprite);

		template<std::derived_from<Sprite> TSprite, typename... Args>
		void addSprite(Args&&... args) 
		{
			m_sprites.push_back(MakeRef<TSprite>(std::forward<Args>(args)...));
		}

	protected:
		std::optional<TileSet>				m_tileset;
		TArray<std::optional<TileLayer>, 6> m_layers;
		unsigned							m_layersCount = 0;

	private:
		TDeque<Sprite*>& GetSpriteRegister() { return m_spriteRegister; }

	private:
		TDeque<Sprite*>	m_spriteRegister;
		TVector<Ref<Sprite>> m_sprites;

		friend class Application;
	};
}