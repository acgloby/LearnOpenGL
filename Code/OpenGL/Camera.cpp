#include "Camera.h"

//∆´∫ΩΩ«
const float YAW			= -90.0f;
//∏©—ˆΩ«
const float PITCH		= 0.0f;
// ”“∞(Field of View)
const float FOV			= 45.0f;
//“∆∂ØÀŸ∂»
const float SPEED		= 5.0f;
// Û±Í¡È√Ù∂»
const float SENSITIVITY = 0.1f;

Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
{
	Position = position;
	Yaw = yaw;
	Pitch = pitch;
	WorldUp = worldup;
	UpdateCameraVectors();
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) 
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position += Right * velocity;
	if (direction == RIGHT)
		Position -= Right * velocity;
}

void Camera::ProcessMouseScroll(float yoffset)
{
	Fov -= yoffset;
	if (Fov < 1.0f)
		Fov = 1.0f;
	if (Fov > 45.0f)
		Fov = 45.0f;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) 
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	UpdateCameraVectors();
}


void Camera::UpdateCameraVectors()
{
	glm::vec3 front = glm::vec3(0);
	front.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
	front.y = glm::sin(glm::radians(Pitch));
	front.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Front, Right));
}