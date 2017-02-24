#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include "Shader.h"
#include <SOIL.h>

const GLuint WIDTH = 800, HEIGHT = 600;

void key_callback( GLFWwindow * window, int key, int scancode, int action, int mode )
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
	{
		glfwSetWindowShouldClose( window, GL_TRUE );
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
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,//左上角
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,//左下角
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,//右下角
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f//右上角
	};

	GLuint indices[] =
	{
		0, 1, 2,//第一个三角形
		0, 2, 3//第二个三角形
	};

	GLuint VBO;
	glGenBuffers( 1, &VBO );
	GLuint VAO;
	glGenVertexArrays( 1, &VAO );
	GLuint EBO;
	glGenBuffers( 1, &EBO );

	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * )0 );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 1 );
	glBindVertexArray( 0 );

	int width, height;
	unsigned char * image = SOIL_load_image( "container.jpg", &width, &height, 0, SOIL_LOAD_RGB );

	Shader shader( ".\\shaders\\shader.vp", ".\\shaders\\shader.fp" );

	while ( !glfwWindowShouldClose( window ) )
	{
		glfwPollEvents();

		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		shader.Use();

		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = ( sin( timeValue ) / 2 ) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation( shader.Program, "ourColor" );
		glUniform4f( vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f );

		glBindVertexArray( VAO );
		//glDrawArrays( GL_TRIANGLES, 0, 3 );
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );

		glfwSwapBuffers( window );
	}

	glfwTerminate();
	return 0;
}