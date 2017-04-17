#include "GraphicInterface.h"
#include "ShaderManager.h"
#include <functional>

#define ErrorBox(x) MessageBox(NULL, x, LPCTSTR("Error GraphicsInterface_Singleton"), MB_OK);

GraphicInterface_Singleton::GraphicInterface_Singleton()
{
	_shaderManager = new ShaderManager();
}

GraphicInterface_Singleton::~GraphicInterface_Singleton()
{
	Shutdown();
}

size_t GraphicInterface_Singleton::CreateShaderFromCompiledFile(char *fileName, ShaderType type)
{
	std::hash<char> hash;
	size_t key = hash(*fileName);
	
	return key;
}

void GraphicInterface_Singleton::Shutdown()
{
	if (_shaderManager)
	{
		delete _shaderManager;
		_shaderManager = nullptr;
	}
}
