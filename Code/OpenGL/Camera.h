#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

#pragma once
class Camera
{
public :
	Camera(glm::vec3 position, glm::vec3 worldup, float yaw, float pitch);
	~Camera();
	glm::mat4 GetViewMatrix();
	void ProcessMouseScroll(float yoffset);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 Front;
	glm::vec3 WorldUp;
	float Pitch;
	float Yaw;
	float Fov;
	float MovementSpeed;
	float MouseSensitivity;

private :
	void UpdateCameraVectors();
};
