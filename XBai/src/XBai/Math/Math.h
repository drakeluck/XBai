#ifndef XB_MATH_MATH_H
#define XB_MATH_MATH_H

#include <glm/glm.hpp>

namespace XBai::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
}

#endif