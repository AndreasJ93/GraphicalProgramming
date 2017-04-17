#ifndef GRAPHIC_INTERFACE_H
#define GRAPHIC_INTERFACE_H

#include <unordered_map>
#include "Shader.h"

class SystemClass;
class ShaderManager;

class GraphicInterface_Singleton
{
private:
	GraphicInterface_Singleton();
	~GraphicInterface_Singleton();
	GraphicInterface_Singleton(const GraphicInterface_Singleton &original) = delete;
	GraphicInterface_Singleton operator=(const GraphicInterface_Singleton &original) = delete;

	
	ShaderManager* _shaderManager = nullptr;
	
	/*TO DO: Change this to a hash instead. In fact, it should probably be in another class altogether! */
	std::unordered_map<size_t /*Key*/, size_t /*Shader Key in ShaderManager*/> _keyMap; 

public:
	static inline GraphicInterface_Singleton &Instance() { static GraphicInterface_Singleton instance; return instance; };

	/*To do: Change this and add new things*/
	size_t CreateShaderFromCompiledFile(char *fileName, ShaderType type);
	void Shutdown();

};





#endif
