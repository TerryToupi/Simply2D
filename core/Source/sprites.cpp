#include <pch.h>
#include <sprites.h>

namespace Simply2D
{
	Sprite::Sprite(entt::entity handle, Scene* scene)
		:	m_handle(handle), m_registeredScene(scene)
	{
	}
}