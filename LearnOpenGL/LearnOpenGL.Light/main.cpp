#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Image.h"

const GLuint WIDTH = 800, HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3( 0.0f, 0.0f, 3.0f );
glm::vec3 cameraFront = glm::vec3( 0, 0, -1.0f );
glm::vec3 cameraUp = glm::vec3( 0.0f, 1.0f, 0.0f );

glm::vec3 lightPos = glm::vec3( 1.2f, 1.0f, 2.0f );

bool keys[1024];

GLfloat deltaTime = 0.0f; //当前帧与上一帧的时间间隔
GLfloat lastFrame = 0.0f;  //上一帧的时间

bool firstMouse = true;
GLfloat lastX = 400.0f, lastY = 300.0f;

//Camera camera( glm::vec3( 1.2f, 1.0f, 7.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), -90.0f, -5.0f );
Camera camera( glm::vec3( 0.0f, 0.0f, 10.0f ));

void key_callback( GLFWwindow * window, int key, int scancode, int action, int mode )
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
	{
		glfwSetWindowShouldClose( window, GL_TRUE );
	}

	if ( action == GLFW_PRESS )
	{
		keys[key] = true;
	}
	if ( action == GLFW_RELEASE )
	{
		keys[key] = false;
	}
}

void mouse_callback( GLFWwindow * window, double xPos, double yPos )
{
	if ( firstMouse )
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xoffset = xPos - lastX;
	GLfloat yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMove( xoffset, yoffset );
}

void scroll_callback( GLFWwindow * window, double xoffset, double yoffset )
{
	camera.ProcessMouseScroll( yoffset );
}

void do_movement()
{
	if ( keys[GLFW_KEY_W] )
	{
		camera.ProcessKeyboard( FORWARD, deltaTime );
	}
	if ( keys[GLFW_KEY_S] )
	{
		camera.ProcessKeyboard( BACKWORD, deltaTime );
	}
	if ( keys[GLFW_KEY_A] )
	{
		camera.ProcessKeyboard( LEFT, deltaTime );
	}
	if ( keys[GLFW_KEY_D] )
	{
		camera.ProcessKeyboard( RIGHT, deltaTime );
	}
}

int main( int argc, char ** argv )
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	GLFWwindow * window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
	if ( window == nullptr )
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );
	glfwSetKeyCallback( window, key_callback );
	glfwSetCursorPosCallback( window, mouse_callback );
	glfwSetScrollCallback( window, scroll_callback );

	glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK )
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize( window, &width, &height );

	glViewport( 0, 0, width, height );

	GLfloat vertices[] =
	{
		// position, normal, texCoords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glm::vec3 cubePositions[] =
	{
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 2.0f, 5.0f, -15.0f ),
		glm::vec3( -1.5f, -2.2f, -2.5f ),
		glm::vec3( -3.8f, -2.0f, -12.3f ),
		glm::vec3( 2.4f, -0.4f, -3.5f ),
		glm::vec3( -1.7f, 3.0f, -7.5f ),
		glm::vec3( 1.3f, -2.0f, -2.5f ),
		glm::vec3( 1.5f, 2.0f, -2.5f ),
		glm::vec3( 1.5f, 0.2f, -1.5f ),
		glm::vec3( -1.3f, 1.0f, -1.5f )
	};

	glm::vec3 pointLightPositions[] =
	{
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
	};

	glm::vec3 dirLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 pointLightsColor[] =
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
	};

	glm::vec3 spotLightColor = glm::vec3(1.0f, 1.0f, 0.0f);

	GLuint VBO;
	glGenBuffers( 1, &VBO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	GLuint VAO;
	glGenVertexArrays( 1, &VAO );
	GLuint lightVAO;
	glGenVertexArrays( 1, &lightVAO );

	glBindVertexArray( VAO );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )0 );
	glEnableVertexAttribArray( 0 );

	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 1 );

	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 2 );
	glBindVertexArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBindVertexArray( lightVAO );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )0 );
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );

	Shader shader( ".\\shaders\\shader.vp", ".\\shaders\\shader.fp" );
	Shader lampShader( ".\\shaders\\shader.vp", ".\\shaders\\light.fp" );

	Image diffuseMap;
	diffuseMap.Load( ".\\images\\container2.png" );
	Image specularMap;
	specularMap.Load( ".\\images\\container2_specular.png" );
	Image emitMap;
	emitMap.Load( ".\\images\\matrix.jpg" );

	glEnable( GL_DEPTH_TEST );

	while ( !glfwWindowShouldClose( window ) )
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		do_movement();

		glClearColor( 0.9f, 0.9f, 0.9f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//计算model view projection matrix
		glm::mat4 model;
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective( glm::radians( camera.Zoom ), ( float )width / ( float )height, 0.1f, 100.0f );

		#pragma region 绘制箱子
		shader.Use();
		GLint modelLocation = glGetUniformLocation( shader.Program, "model" );
		GLint viewLocation = glGetUniformLocation( shader.Program, "view" );
		GLint projLocation = glGetUniformLocation( shader.Program, "projection" );
		glUniformMatrix4fv( modelLocation, 1, GL_FALSE, glm::value_ptr( model ) );
		glUniformMatrix4fv( viewLocation, 1, GL_FALSE, glm::value_ptr( view ) );
		glUniformMatrix4fv( projLocation, 1, GL_FALSE, glm::value_ptr( projection ) );
		glUniform3f( glGetUniformLocation( shader.Program, "viewPos" ), camera.Position.x, camera.Position.y, camera.Position.z );
		glUniform1f( glGetUniformLocation( shader.Program, "material.shininess" ), 64.0f );
		glUniform1i( glGetUniformLocation( shader.Program, "material.diffuse" ), 0 );
		glUniform1i( glGetUniformLocation( shader.Program, "material.specular" ), 1 );
		glUniform1i( glGetUniformLocation( shader.Program, "material.emission" ), 2 );

		//设置平行光属性
		glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), dirLightColor.x*0.2f, dirLightColor.y*0.2f, dirLightColor.z*0.2f);
		glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), dirLightColor.x*0.5f, dirLightColor.y*0.5f, dirLightColor.z*0.5f);
		glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), dirLightColor.x, dirLightColor.y, dirLightColor.z);


		//设置点光源属性
		for (int i = 0; i < 4; i++)
		{
			char buf[10];
			sprintf_s(buf, "%d", i);
			std::string str = std::string("pointLights[") + std::string(buf) + std::string("].position");
			glUniform3f(glGetUniformLocation(shader.Program, str.c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			str = std::string("pointLights[") + std::string(buf) + std::string("].constant");
			glUniform1f(glGetUniformLocation(shader.Program, str.c_str()), 1.0f);
			str = std::string("pointLights[") + std::string(buf) + std::string("].linear");
			glUniform1f(glGetUniformLocation(shader.Program, str.c_str()), 0.09f);
			str = std::string("pointLights[") + std::string(buf) + std::string("].quadratic");
			glUniform1f(glGetUniformLocation(shader.Program, str.c_str()), 0.032f);
			str = std::string("pointLights[") + std::string(buf) + std::string("].ambient");
			glUniform3f(glGetUniformLocation(shader.Program, str.c_str()), pointLightsColor[i].x*0.2f, pointLightsColor[i].y*0.2f, pointLightsColor[i].z*0.2f);
			str = std::string("pointLights[") + std::string(buf) + std::string("].diffuse");
			glUniform3f(glGetUniformLocation(shader.Program, str.c_str()), pointLightsColor[i].x*0.5f, pointLightsColor[i].y*0.5f, pointLightsColor[i].z*0.5f);
			str = std::string("pointLights[") + std::string(buf) + std::string("].specular");
			glUniform3f(glGetUniformLocation(shader.Program, str.c_str()), pointLightsColor[i].x, pointLightsColor[i].y, pointLightsColor[i].z);
		}

		//设置聚光灯属性
		glUniform3f(glGetUniformLocation(shader.Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(glGetUniformLocation(shader.Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
		glUniform1f(glGetUniformLocation(shader.Program, "spotLight.cutOff"), glm::cos(glm::radians(2.5f)));
		glUniform1f(glGetUniformLocation(shader.Program, "spotLight.outterCutOff"), glm::cos(glm::radians(7.5f)));
		glUniform3f(glGetUniformLocation(shader.Program, "spotLight.ambient"), spotLightColor.x*0.2f, spotLightColor.y*0.2f, spotLightColor.z*0.2f);
		glUniform3f(glGetUniformLocation(shader.Program, "spotLight.diffuse"), spotLightColor.x*0.5f, spotLightColor.y*0.5f, spotLightColor.z*0.5f);
		glUniform3f(glGetUniformLocation(shader.Program, "spotLight.specular"), spotLightColor.x, spotLightColor.y, spotLightColor.z);

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, diffuseMap.textureID );
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, specularMap.textureID );
		glActiveTexture( GL_TEXTURE2 );
		glBindTexture( GL_TEXTURE_2D, emitMap.textureID );

		glBindVertexArray( VAO );
		for ( int i = 0; i < 10; i++ )
		{
			model = glm::mat4();
			model = glm::translate( model, cubePositions[i] );
			GLfloat angle = 20.0f * i;
			model = glm::rotate( model, angle, glm::vec3( 1.0f, 0.3f, 0.5f ) );
			glUniformMatrix4fv( modelLocation, 1, GL_FALSE, glm::value_ptr( model ) );
			glDrawArrays( GL_TRIANGLES, 0, 36 );
		}
		glBindVertexArray( 0 );
		#pragma endregion

		#pragma region 绘制点光源
		lampShader.Use();
		modelLocation = glGetUniformLocation(lampShader.Program, "model");
		viewLocation = glGetUniformLocation(lampShader.Program, "view");
		projLocation = glGetUniformLocation(lampShader.Program, "projection");
		GLuint lightColorPos = glGetUniformLocation(lampShader.Program, "lightColor");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(lightVAO);
		for (int i = 0; i < 4; i++)
		{
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3f(lightColorPos, pointLightsColor[i].x, pointLightsColor[i].y, pointLightsColor[i].z);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		#pragma endregion

		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1, &VAO );
	glDeleteVertexArrays( 1, &lightVAO );
	glDeleteBuffers( 1, &VBO );

	glfwTerminate();
	return 0;
}