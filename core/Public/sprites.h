#pragma once

#include <scene.h>
#include <components.h>

#include <entt/entity/registry.hpp>

namespace Simply2D
{
	class Sprite
	{
	public:
		Sprite() = default;
		Sprite(entt::entity handle, Scene* scene);
		Sprite(const Sprite& other) = default;

		template<typename TComponent, typename... Args>
		TComponent& addComponent(Args&&... args)
		{
			return m_registeredScene->m_spritesRegistry.emplace<TComponent>(m_handle, std::forward<Args>(args)...);
		}

		template<typename TComponent>
		TComponent& getComponent()
		{
			return m_registeredScene->m_spritesRegistry.get<TComponent>(m_handle);
		}

	private:
		entt::entity m_handle{ entt::null };
		Scene* m_registeredScene{ nullptr };
	};
}