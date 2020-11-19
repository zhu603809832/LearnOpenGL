#pragma once

class Model
{
public:
	/*  ��Ա����   */
	Model(GLchar* path)
	{
		this->loadModel(path);
	}
	void Draw(Shader shader);
private:
	/*  ģ������  */
	vector<Mesh> meshes;
	string directory;

	/*  ˽�г�Ա����   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};