#include "ShaderManager.h"
#include "DirectX11Interface.h"

#define ErrorBox(x) MessageBox(NULL, x, LPCTSTR("Error ShaderManager"), MB_OK);

ShaderManager::ShaderManager()
{
	_shaderMap.reserve(256); /*Reserve enough space to completely store the data needed, no need for rezise.*/
}

ShaderManager::~ShaderManager()
{
	for (auto shader : _shaderMap)
	{
		DirectX11Interface_Singleton::Instance().DestroyResource(shader.second);
	}
}

IUnknown* ShaderManager::AddShader(char *fileName, ShaderType type, std::vector<unsigned char> &shaderByteCode)
{
	IUnknown* toSave = nullptr;
	std::hash<char*> hash;
	size_t key = hash(fileName);
	/*ONLY ADD THE SHADER IF IT DOES NOT ALREADY EXISTS!*/
	auto _shader = _shaderMap.find(key);
	if (_shader == _shaderMap.end())
	{
		shaderByteCode = DirectX11Interface_Singleton::Instance().ReadShaderByteCode(fileName);

		if (shaderByteCode.size() == 0)
		{
			/*Throw error!*/
			throw "No byte-code produced for shader.";
		}

		

		switch (type)
		{
		case VertexShader:
			toSave = DirectX11Interface_Singleton::Instance().CreateVertexShaderFromByteData(&shaderByteCode[0], shaderByteCode.size());
			break;

		case PixelShader:
			toSave = DirectX11Interface_Singleton::Instance().CreatePixelShaderFromByteData(&shaderByteCode[0], shaderByteCode.size());
			break;

		case GeometryShader:
			toSave = DirectX11Interface_Singleton::Instance().CreateGeometryShaderFromByteData(&shaderByteCode[0], shaderByteCode.size());
			break;

		case ComputeShader:
			toSave = DirectX11Interface_Singleton::Instance().CreateComputeShaderFromByteData(&shaderByteCode[0], shaderByteCode.size());
			break;

		default:
			ErrorBox(L"Shader Type Not Supported");

		}

		_shaderMap[key] = toSave;
	}
	else
	{
		toSave = _shader->second;
	}

	return toSave;
}

void ShaderManager::BindShader(size_t identifier, ShaderType type)
{
	IUnknown* shaderPointer = nullptr;
	auto shader = _shaderMap.find(identifier);
	if (shader != _shaderMap.end())
	{
		shaderPointer = shader->second;
	}
	switch (type)
	{
		case VertexShader:
			DirectX11Interface_Singleton::Instance().SetVertexShader((ID3D11VertexShader*)shaderPointer);
			break;

		case PixelShader:
			DirectX11Interface_Singleton::Instance().SetPixelShader((ID3D11PixelShader*)shaderPointer);
			break;

		case GeometryShader:
			DirectX11Interface_Singleton::Instance().SetGeometryShader((ID3D11GeometryShader*)shaderPointer);
			break;

		case ComputeShader:
			DirectX11Interface_Singleton::Instance().SetComputeShader((ID3D11ComputeShader*)shaderPointer);
			break;

		default:
			ErrorBox(L"Shader Type Not Supported");
		
	}
}

void ShaderManager::DestroyShader(size_t identifier)
{
	/*Problem with the vector approach. Have to think this over*/
	if (_shaderMap.find(identifier) != _shaderMap.end())
	{
		DirectX11Interface_Singleton::Instance().DestroyResource(_shaderMap[identifier]);
	}
}
