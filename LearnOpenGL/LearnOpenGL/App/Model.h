#pragma once

class Model
{
public:
	/*  成员函数   */
	Model(GLchar* path)
	{
		this->loadModel(path);
	}
	void Draw(Shader shader);
private:
	/*  模型数据  */
	vector<Mesh> meshes;
	string directory;

	/*  私有成员函数   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};