#pragma once

#include "Base/assets.h"

#include "tileSet.h"
#include "tileLayers.h"
#include "sprite.h"

#include "Types/Arrays.h"
#include "Types/SmartPointers.h"

#include "Events/event.h"
#include "Events/inputEvents.h"
#include "Events/windowEvent.h"

#include <string>

namespace Simply2D
{
	class Scene
	{
	public:
		Scene(const std::string& vfp);
		virtual ~Scene() = default;

		// Scripting functions
		virtual void begin(float ts)			{}
		virtual void event(Simply2D::Event& e)	{}
		virtual void update(float ts)			{} 
		virtual void render()					{}
		virtual void end(float ts)				{}

		// sprites
		template<std::derived_from<Sprite> TSprite, typename... Args>
		TSprite* AddSprite(Args&&... args) 
		{
			auto ptr = MakeRef<TSprite>(std::forward<Args>(args)...);
			m_sprites.push_back(ptr);
			return ptr.get();
		}

		template<std::derived_from<Sprite> TSprite>
		TSprite* GetSprite(const char* id)
		{
			for (auto& sprite : m_sprites)
				if (strcmp(id, sprite->GetID()) == 0)
					return static_cast<TSprite*>(sprite.get());

			return nullptr;
		}

		template<std::derived_from<Sprite> TSprite>
		TVector<TSprite*> GetAll()
		{
			TVector<TSprite*> result;
			result.reserve(m_sprites.size());

			if constexpr (std::is_same_v<TSprite, Sprite>)
			{
				for (auto& sprite : m_sprites)
					result.push_back(static_cast<TSprite*>(sprite.get()));
			}
			else
			{
				for (auto& sprite : m_sprites)
					if (auto* casted = dynamic_cast<TSprite*>(sprite.get()))
						result.push_back(casted);
			}

			return result;
		}

		void RemoveSprite(const char* id)
		{
			for (size_t i = 0; i < m_sprites.size(); ++i)
			{
				if (strcmp(id, m_sprites[i]->GetID()) == 0)
				{
					m_sprites.erase(m_sprites.begin() + i);
					return;
				}
			}
		}

	protected:
		TVector<Ref<Sprite>> m_sprites;
		Ref<TileSet>		 m_tileset;
		TVector<TileLayer>	 m_layers;
		unsigned			 m_layersCount = 0;
	};
}