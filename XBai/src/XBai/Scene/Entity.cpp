#include "xbpch.h"
#include "Entity.h"

namespace XBai
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{
	}
}
