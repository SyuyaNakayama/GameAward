#include "Material.h"
#include <fstream>
#include <sstream>
#include "D3D12Common.h"
using namespace std;

void LoadVector3Stream(istringstream& stream, Vector3& vec);

void Material::LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME)
{
	ifstream file;
	file.open(DIRECTORY_PATH + FILENAME);
	assert(!file.fail());

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') { key.erase(key.begin()); }
		if (key == "newmtl") { line_stream >> materialName; }
		if (key == "Ka") { LoadVector3Stream(line_stream, ambient); }
		if (key == "Kd") { LoadVector3Stream(line_stream, diffuse); }
		if (key == "Ks") { LoadVector3Stream(line_stream, specular); }
		if (key == "map_Kd")
		{
			line_stream >> textureFilename;
			string path = DIRECTORY_PATH;
			path.erase(path.begin(), path.begin() + 10);
			sprite = Sprite::Create(path + textureFilename);
		}
	}
	file.close();
}

void Material::Initialize()
{
	// 定数バッファ生成
	CreateBuffer(&constBuffer, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
	Update();
}

void Material::Update()
{
	constMap->ambient = ambient;
	constMap->diffuse = diffuse;
	constMap->specular = specular;
	constMap->alpha = 1.0f;
}

void Material::SetMaterial(Material* material)
{
	materialName = material->materialName;
	ambient = material->ambient;
	diffuse = material->diffuse;
	specular = material->specular;
	textureFilename = material->textureFilename;
}