#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform();

	glm::mat4 matrix() const;
	Transform operator*(const Transform& other) const;
};

#endif