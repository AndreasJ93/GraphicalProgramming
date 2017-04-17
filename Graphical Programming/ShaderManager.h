#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <unordered_map>
#include "Shader.h"

/*PROBLEM: Removing a specific shader WILL NOT MAKE IT "AVAILABLE" TO USE*/

struct IUnknown;

class ShaderManager
{
private:
	std::vector<unsigned char> _errorVector;
	std::unordered_map<size_t,IUnknown*> _shaderMap;

	ShaderManager(const ShaderManager &original) = delete;
	ShaderManager &operator=(const ShaderManager &original) = delete;

public:
	ShaderManager();
	~ShaderManager();

	IUnknown* AddShader(char* fileName, ShaderType type, std::vector<unsigned char> &shaderByteCode);
	void BindShader(size_t identifier, ShaderType type);

	void DestroyShader(size_t identifier);

};


#endif
