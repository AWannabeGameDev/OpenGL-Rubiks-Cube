#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Camera::Camera(float fov, float aspectRatio, float near, float far) :
	position{}, i{1.0f, 0.0f, 0.0f}, j{0.0f, 1.0f, 0.0f}, k{0.0f, 0.0f, 1.0f},
	projection{glm::perspective(fov, aspectRatio, near, far)}
{}

glm::mat4 Camera::matrix() const
{
	return projection * viewMatrix();
}

glm::mat4 Camera::viewMatrix() const
{
	return rotationMatrix() * glm::translate(glm::mat4{1.0f}, -position);
}

const glm::mat4& Camera::projectionMatrix() const
{
	return projection;
}

const glm::vec3 Camera::right() const
{
	return i;
}

const glm::vec3 Camera::up() const
{
	return j;
}

const glm::vec3 Camera::behind() const
{
	return k;
}

void Camera::rotateGlobal(const glm::vec3& globalAxis, float radians)
{
	glm::mat4 rotation = glm::rotate(glm::mat4{1.0f}, radians, globalAxis);

	j = glm::vec3{rotation * glm::vec4{j, 1.0f}};
	i = glm::vec3{rotation * glm::vec4{i, 1.0f}};
	k = glm::vec3{rotation * glm::vec4{k, 1.0f}};
}

void Camera::rotateLocal(const glm::vec3& localAxis, float radians)
{
	glm::vec3 globalAxis = glm::vec3{inverseRotationMatrix() * glm::vec4{localAxis, 1.0f}};

	rotateGlobal(globalAxis, radians);
}

glm::mat4 Camera::rotationMatrix() const
{
	return {glm::vec4{i.x, j.x, k.x, 0.0f},
			glm::vec4{i.y, j.y, k.y, 0.0f},
			glm::vec4{i.z, j.z, k.z, 0.0f},
			glm::vec4{0.f, 0.f, 0.f, 1.0f}};
}

glm::mat4 Camera::inverseRotationMatrix() const
{
	return {glm::vec4{i, 0.0f},
			glm::vec4{j, 0.0f},
			glm::vec4{k, 0.0f},
			glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}};
}