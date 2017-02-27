#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWORD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat MoveSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0), GLfloat yaw = -90.0f, GLfloat pitch = 0);

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	void ProcessMouseMove(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	void ProcessMouseScroll(GLfloat yoffset);

private:
	void updateCameraVectors();
};