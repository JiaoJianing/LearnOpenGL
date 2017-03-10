#pragma once
#include <GL/glew.h>
#include <SOIL.h>
#include <string>

class Image
{
public:
	GLuint textureID;
public:
	Image();
	~Image();

public:
	void Load( const std::string & imagePath );
};

