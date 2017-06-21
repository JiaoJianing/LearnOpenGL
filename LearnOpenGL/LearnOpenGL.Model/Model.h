#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include <string>
#include <vector>
#include "Mesh.h"
using namespace std;

class Model
{
public:
	/*  函数   */
	Model( GLchar * path )
	{
		this->loadModel( path );
	}
	void Draw( Shader shader );
private:
	/*  模型数据  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;
	/*  函数   */
	void loadModel( string path );
	void processNode( aiNode * node, const aiScene * scene );
	Mesh processMesh( aiMesh * mesh, const aiScene * scene );
	vector<Texture> loadMaterialTextures( aiMaterial * mat, aiTextureType type,
	                                      string typeName );
};

GLint TextureFromFile( const char * path, string directory );

