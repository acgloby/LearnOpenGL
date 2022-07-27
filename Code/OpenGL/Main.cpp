#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

float const WIDTH = 800;
float const HEIGHT = 600;

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
unsigned int LoadImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot);

#pragma region ModelData
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
#pragma endregion

unsigned int indices[] = {
	0,1,2,
	2,3,0
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

//相机
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;	//偏航角
float pitch = 0.0f; //俯仰角
bool firstMouse = true; //鼠标第一次进入窗口
float lastX = WIDTH / 2;	//设置鼠标偏移位置到屏幕中间
float lastY = HEIGHT / 2;
Camera* MainCamera = new Camera(cameraPos, cameraUp, yaw, pitch);;


float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

#pragma region 输入事件
/// <summary>
/// 键盘输入检测
/// </summary>
void processInput(GLFWwindow* window)
{
	//按下esc退出
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		MainCamera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		MainCamera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		MainCamera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		MainCamera->ProcessKeyboard(RIGHT, deltaTime);
}

/// <summary>
/// 鼠标移动
/// </summary>
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = lastX - xpos;
	float yOffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	MainCamera->ProcessMouseMovement(xOffset, yOffset, true);
}

/// <summary>
/// 鼠标滑轮镜头缩放
/// </summary>
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	MainCamera->ProcessMouseScroll(static_cast<float>(yOffset));
}
#pragma endregion

#pragma region 加载图片
/// <summary>
/// 加载图片
/// </summary>
unsigned int LoadImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot) 
{
	unsigned int texBuffer;
	glGenTextures(1, &texBuffer);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, texBuffer);

	int width, height, nrChannels;
	//加载贴图
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_set_flip_vertically_on_load(true);
	stbi_image_free(data);
	return texBuffer;
}
#pragma endregion

int main()
{
	#pragma region 打开一个window
	glfwInit();
	// 设置glfw版本号3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to Create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//在当前线程中打开一个窗口
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to Create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);
	////开启面剔除
	//glEnable(GL_CULL_FACE);
	////剔除背面
	//glCullFace(GL_BACK); 
	//设置绘制模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//开启深度测试
	glEnable(GL_DEPTH_TEST);
	//隐藏鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//鼠标回调
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	#pragma endregion

	#pragma region 创建Shader
	Shader* shaderProgram = new Shader("vertexSource.vert", "fragmentSource.frag");
	#pragma endregion

	#pragma region 顶点缓存
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	#pragma endregion

	#pragma region 加载Texture
	//为当前绑定的纹理对象设置环绕过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	unsigned int TexBufferA = LoadImageToGPU("Image/head.jpg", GL_RGB, GL_RGB, 0);
	unsigned int TexBufferB = LoadImageToGPU("Image/bg.png", GL_RGBA, GL_RGBA, 1);
	#pragma endregion

	while (!glfwWindowShouldClose(window))
	{	
		// DeltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Input Process
		processInput(window);

		glClearColor(1, 0.5f, 0.5f, 0);
		//Clear Buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		for (unsigned int i = 0; i < 1; i++)
		{
			// Set MVP
			glm::mat4 model = glm::mat4(1);
			glm::mat4 view = glm::mat4(1);
			glm::mat4 projection = glm::mat4(1);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			if (MainCamera != nullptr)
			{
				view = MainCamera->GetViewMatrix();
				projection = glm::perspective(glm::radians(MainCamera->Fov), WIDTH / HEIGHT, 0.1f, 100.0f);
			}

			//Set Shader
			shaderProgram->use();

			//Set Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TexBufferA);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, TexBufferB);

			shaderProgram->setInt("texture1", 0);
			shaderProgram->setInt("texture2", 1);
			shaderProgram->setMatrix4fv("model", glm::value_ptr(model));
			shaderProgram->setMatrix4fv("view", glm::value_ptr(view));
			shaderProgram->setMatrix4fv("projection", glm::value_ptr(projection));
			//Set Model
			glBindVertexArray(VAO);

			//Drawcall
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(window);
		glfwPollEvents(); //接受输入事件
	}

	//Exit Program
	glfwTerminate();
	return 0;
}

