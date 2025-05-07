#ifndef XB_SCENE_SCRIPTABLE_ENTITY_H
#define XB_SCENE_SCRIPTABLE_ENTITY_H

#include "Entity.h"

namespace XBai
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate(){}
		virtual void OnDestroy() {}
		virtual void OnUpdate(TimeStep ts) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}

#endif

