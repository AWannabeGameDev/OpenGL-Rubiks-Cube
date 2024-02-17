#include "transform.h"

#include <glm/gtx/string_cast.hpp>

Transform::Transform() :
	position{}, rotation{1.0f, 0.0f, 0.0f, 0.0f}, scale{1.0f, 1.0f, 1.0f}
{}

glm::mat4 Transform::matrix() const
{
	return glm::translate(glm::mat4{1.0f}, position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4{1.0f}, scale);
}

Transform Transform::operator*(const Transform& other) const
{
	Transform ret;
	ret.position = position + other.position;
	ret.rotation = rotation * other.rotation;
	ret.scale = scale * other.scale;
	return ret;
}