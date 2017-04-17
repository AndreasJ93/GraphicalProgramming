#pragma once
#define MAXIMAL_NUMBER_OF_PIPELINES 256
class ShaderManager;
class LayoutManager;
struct IUnknown;

class PipelineManager 
{
private:
	
	struct PipeLine
	{
		/*Here, the keys for the pipeline should be stored*/
		struct VertexShader
		{
			IUnknown* vertexShader;
			IUnknown* inputLayout;
		}vertexShader;
		IUnknown* pixelShader;
		IUnknown* geometryShader;
		IUnknown* hullShader;
		IUnknown* domainShader;
		IUnknown* computerShader;
	};
	ShaderManager* _shaderManager;
	LayoutManager* _layoutManager;
	size_t _nrOfPipelines = 0;
	PipeLine _pipeLines[MAXIMAL_NUMBER_OF_PIPELINES];

public:

	struct PipelineDescription
	{
		/*Information about the pipeline that are to be created*/
		char* vertexShaderFile = nullptr;
		char* pixelShaderFile = nullptr;
		char* geometryShaderFile = nullptr;
		char* hullShaderFile = nullptr;
		char* domainShaderFile = nullptr;
		char* computerShaderFile = nullptr;
	};
	PipelineManager();
	~PipelineManager();

	size_t AddPipeline(PipelineDescription desc);
	void BindPipeline(size_t ID);
	void RemovePipeline(size_t ID);
};