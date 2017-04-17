#include "PipelineManager.h"

#include "ShaderManager.h"
#include "LayoutManager.h"

PipelineManager::PipelineManager()
{
	_shaderManager = new ShaderManager();
	_layoutManager = new LayoutManager();
}

PipelineManager::~PipelineManager()
{
	delete _shaderManager;
	delete _layoutManager;
}

size_t PipelineManager::AddPipeline(PipelineDescription desc)
{
	if (_nrOfPipelines < MAXIMAL_NUMBER_OF_PIPELINES)
	{
		PipeLine toBeAdded;
		std::vector<unsigned char> shaderByteCode;
		if (desc.vertexShaderFile)
		{
			toBeAdded.vertexShader.vertexShader = _shaderManager->AddShader(desc.vertexShaderFile, VertexShader, shaderByteCode);
			toBeAdded.vertexShader.inputLayout = _layoutManager->AddLayout(desc.vertexShaderFile, shaderByteCode);
		}
		if (desc.pixelShaderFile)
		{
			toBeAdded.pixelShader = _shaderManager->AddShader(desc.pixelShaderFile, PixelShader, shaderByteCode);
		}
		if (desc.geometryShaderFile)
		{
			toBeAdded.pixelShader = _shaderManager->AddShader(desc.geometryShaderFile, GeometryShader, shaderByteCode);
		}
		if (desc.hullShaderFile)
		{
			toBeAdded.hullShader = _shaderManager->AddShader(desc.hullShaderFile, HullShader, shaderByteCode);
		}
		if (desc.domainShaderFile)
		{
			toBeAdded.hullShader = _shaderManager->AddShader(desc.domainShaderFile, DomainShader, shaderByteCode);
		}
		if (desc.computerShaderFile)
		{
			toBeAdded.hullShader = _shaderManager->AddShader(desc.computerShaderFile, ComputeShader, shaderByteCode);
		}

		_pipeLines[_nrOfPipelines] = toBeAdded;

		return _nrOfPipelines++;
	}
	return -1;
}

void PipelineManager::BindPipeline(size_t ID)
{
	
}

void PipelineManager::RemovePipeline(size_t ID)
{
}
