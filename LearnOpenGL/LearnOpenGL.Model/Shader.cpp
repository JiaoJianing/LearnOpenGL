#include "Shader.h"

Shader::Shader( const GLchar * vertexPath, const GLchar * fragmentPath )
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//保证ifstream可以抛出异常
	vShaderFile.exceptions( std::ifstream::badbit );
	fShaderFile.exceptions( std::ifstream::badbit );

	try
	{
		vShaderFile.open( vertexPath );
		fShaderFile.open( fragmentPath );
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch ( std::ifstream::failure e )
	{
		std::cout << "Open Shader File Failed!" << std::endl;
	}

	//编译着色器
	const char * vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	//顶点着色器
	vertex = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertex, 1, &vShaderCode, NULL );
	glCompileShader( vertex );

	glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
	if ( !success )
	{
		glGetShaderInfoLog( vertex, 512, NULL, infoLog );
		std::cout << "Compile Vertex Shader Failed! Info: " << infoLog << std::endl;
	}

	//片段着色器
	fragment = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragment, 1, &fShaderCode, NULL );
	glCompileShader( fragment );

	glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
	if ( !success )
	{
		glGetShaderInfoLog( fragment, 512, NULL, infoLog );
		std::cout << "Compile Fragment Shader Failed! Info: " << infoLog << std::endl;
	}

	//链接着色器
	this->Program = glCreateProgram();
	glAttachShader( this->Program, vertex );
	glAttachShader( this->Program, fragment );
	glLinkProgram( this->Program );
	glGetProgramiv( this->Program, GL_LINK_STATUS, &success );
	if ( !success )
	{
		glGetProgramInfoLog( this->Program, 512, NULL, infoLog );
		std::cout << "Link Shader Failed! Info: " << infoLog << std::endl;
	}

	glDeleteShader( vertex );
	glDeleteShader( fragment );
}

Shader::~Shader()
{
}

void Shader::Use()
{
	glUseProgram( this->Program );
}
