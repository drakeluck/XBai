#ifndef XB_SCENE_SCENESERIALIZER_H
#define XB_SCENE_SCENESERIALIZER_H

#include "Scene.h"

namespace XBai
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}

#endif
