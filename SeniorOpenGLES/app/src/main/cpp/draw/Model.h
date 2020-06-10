//
// Created by wcg3031 on 2020/6/4.
//

#pragma once

#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model
{
public:
	Model (const string modelPath);
	void Draw (ShaderHelper *pShaderHelper);
private:
	vector <Mesh> m_Meshes;
	vector<Texture> textures_loaded;

	int loadModel (const string modePath);
	void processNode (const aiNode *pNode, const aiScene *pScene);
	Mesh processMesh (const aiMesh *pMesh, const aiScene *pScene);
	vector <Texture> loadMaterialTextures (const aiMaterial *mat, const aiTextureType type, const string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

	string m_Diratory;
};

Model::Model(const string modelPath)
{
	LOGD ("Model::Model modelPath = %s", modelPath.c_str());
	loadModel(modelPath);
}

void Model::Draw (ShaderHelper *pShaderHelper)
{
	LOGD ("Model::Draw");
	for (auto val : m_Meshes)
		val.DrawMesh(pShaderHelper);
}

int Model::loadModel (const string modePath)
{
	LOGD ("Model::loadModel");
	int ret = ERROR_OK;
	Assimp::Importer importer;
	const aiScene *pScene = importer.ReadFile(modePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		LOGE("loadModel error %s", importer.GetErrorString());
		return ERROR_ASSIMP;
	}
	return ERROR_OK;
}

void Model::processNode (const aiNode *pNode, const aiScene *pScene)
{
	LOGD ("Model::processNode");
	// process all meshes of node
	for (int i = 0; i < pNode->mNumMeshes; ++i)
	{
		aiMesh *pMesh = pScene->mMeshes [pNode->mMeshes[i]];
		m_Meshes.push_back(processMesh(pMesh, pScene));
	}

	// process it's children node
	for (int i = 0; i < pNode->mNumChildren; ++i)
	{
		processNode(pNode->mChildren[i], pScene);
	}
}

Mesh Model::processMesh (const aiMesh *pMesh, const aiScene *pScene)
{
	LOGD ("Model::processMesh");
	vector <Vertex> vertices;
	vector <unsigned int> indices;
	vector <Texture> textures;

	for (int i = 0; i < pMesh->mNumVertices; ++i)
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = pMesh->mVertices[i].x;
		vector.y = pMesh->mVertices[i].y;
		vector.z = pMesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = pMesh->mNormals[i].x;
		vector.y = pMesh->mNormals[i].y;
		vector.z = pMesh->mNormals[i].z;
		vertex.Normals = vector;

		if (pMesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = pMesh->mTextureCoords[0][i].x;
			vec.y = pMesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.f, 0.f);

		// tangent
		vector.x = pMesh->mTangents[i].x;
		vector.y = pMesh->mTangents[i].y;
		vector.z = pMesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = pMesh->mBitangents[i].x;
		vector.y = pMesh->mBitangents[i].y;
		vector.z = pMesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertices.push_back(vertex);
	}

	// process faces
	for (int i = 0; i < pMesh->mNumFaces; ++i)
	{
		aiFace face = pMesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// process materials
	aiMaterial *pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];
	// 1. diffuse maps
	vector<Texture> diffuseMaps = loadMaterialTextures(pMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = loadMaterialTextures(pMaterial, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(pMaterial, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(pMaterial, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

vector <Texture> Model::loadMaterialTextures (const aiMaterial *mat, const aiTextureType type, const string typeName)
{
	LOGD ("Model::loadMaterialTextures");
	vector <Texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for(unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
			if(!skip)
			{   // if texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->m_Diratory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
}

unsigned int Model::TextureFromFile(const char *path, const string &directory, bool gamma)
{
	LOGD("Model::TextureFromFile %s, %s", path, directory.c_str());
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	//unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	MyImageInfo myImageInfo {0};
	int ret = OpenImageHelper::LoadPngFromFile(filename.c_str(), &myImageInfo);
	if (ERROR_OK == ret)
	{
		GLenum format;
		if (MY_FORMAT_RGB == myImageInfo.format)
			format = GL_RGB;
		else if (MY_FORMAT_RGBA == myImageInfo.format)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, myImageInfo.buffer[0]);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		OpenImageHelper::FreeMyImageInfo(&myImageInfo);
	}
	else
	{
		LOGE("Texture failed to load at path: %s", path);
		OpenImageHelper::FreeMyImageInfo(&myImageInfo);
	}

	return textureID;
}