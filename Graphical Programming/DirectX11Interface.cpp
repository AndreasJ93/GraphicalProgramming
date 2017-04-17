#include "DirectX11Interface.h"
/*TO BE MOVED AND TWEAKED*/
#include "FileReader.h"

#define ReleaseCOM(x) if (x != NULL) { x->Release(); x = NULL; } 
#define ErrorBox(x) MessageBox(NULL, x, LPCTSTR("Error DirectX11Interface"), MB_OK);

DirectX11Interface_Singleton::DirectX11Interface_Singleton()
{

}

DirectX11Interface_Singleton::DirectX11Interface_Singleton(const DirectX11Interface_Singleton & original)
{

}

DirectX11Interface_Singleton::~DirectX11Interface_Singleton()
{
	Shutdown();
}

bool DirectX11Interface_Singleton::Initialize(HWND hWnd)
{
	/*
		THIS CODE WAS ONLY TO MAKE THINGS STARTING. MOVE AND CHANGE EVERYTHING
		NONE DIRECTX RELATED (INFORMATION THAT CAN BE SENT IN INSTEAD SHOULD BE SO!)	
	*/
	HWND handle = hWnd;
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_VIEWPORT viewport;


	/*SET THIS AS INPUT*/
	UINT width = 1080;
	UINT height = 720;

	//VSYNC HARDCODED FOR NOW!
	_vSyncEnabled = true;


	//Swap Chain\\

	//Swap Chain Description
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.OutputWindow = handle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	/*NEEDS TO BE CHANGED*/
	if (false)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&_swapChain,
		&_device,
		NULL,
		&_deviceContext);
	if (FAILED(hr))
	{
		return false;
	}

	

	//Fix the backbuffer\\

	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&_backBuffer);
	if (FAILED(hr))
	{
		return false;
	}




	//Depth Buffer\\
			//Description
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;						//Should be the same as in the swapChain!
	depthBufferDesc.SampleDesc.Quality = 0;						//Should be the same as in the swapChain!
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;


	//Create the depth buffer
	hr = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	//Create the depth stencil description
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create the depth stencil state
	hr = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if (FAILED(hr))
	{
		return false;
	}

	//Set the depth stencil state
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	//Create the depth stencil view description
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));


	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create the depth stencil view
	hr = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_currentDepthStencilView);
	if (FAILED(hr))
	{
		return false;
	}


	//Viewport\\
			//Set the viewport
	viewport.Height = (float)height;
	viewport.Width = (float)width;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	_deviceContext->RSSetViewports(1, &viewport);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

bool DirectX11Interface_Singleton::Resize()
{
	/*TO BE IMPLEMENTED ONCE EVERYTHING ELSE WORKS!*/
	return false;
}

std::vector<unsigned char> DirectX11Interface_Singleton::ReadShaderByteCode(const char * fileName)
{
	auto byteData = GPFileReader::ReadFromFile(fileName);

	if (byteData.size() == 0)
	{
		return std::vector<unsigned char>();
	}

	return byteData;
}

ID3D11VertexShader * DirectX11Interface_Singleton::CreateVertexShaderFromFile(const char *fileName)
{
	
	auto byteData = GPFileReader::ReadFromFile(fileName);

	if (byteData.size() == 0)
	{
		return nullptr;
	}
	
	return CreateVertexShaderFromByteData(&byteData[0], byteData.size());
}

ID3D11GeometryShader * DirectX11Interface_Singleton::CreateGeometryShaderFromFile(const char *fileName)
{
	auto byteData = GPFileReader::ReadFromFile(fileName);

	if (byteData.size() == 0)
	{
		return nullptr;
	}
	
	return CreateGeometryShaderFromByteData(&byteData[0], byteData.size());
}

ID3D11PixelShader * DirectX11Interface_Singleton::CreatePixelShaderFromFile(const char *fileName)
{
	auto byteData = GPFileReader::ReadFromFile(fileName);

	if (byteData.size() == 0)
	{
		return nullptr;
	}

	return CreatePixelShaderFromByteData(&byteData[0], byteData.size());
}

ID3D11ComputeShader * DirectX11Interface_Singleton::CreateComputeShaderFromFile(const char *fileName)
{

	auto byteData = GPFileReader::ReadFromFile(fileName);

	if (byteData.size() == 0)
	{
		return nullptr;
	}
	
	return CreateComputeShaderFromByteData(&byteData[0], byteData.size());
}

ID3D11VertexShader * DirectX11Interface_Singleton::CreateVertexShaderFromByteData(void * data, size_t dataSize)
{
	ID3D11VertexShader* shader;
	HRESULT hr;

	hr = _device->CreateVertexShader(data, dataSize, nullptr, &shader);
	if (FAILED(hr))
	{
		return nullptr;
	}

	return shader;
}

ID3D11GeometryShader * DirectX11Interface_Singleton::CreateGeometryShaderFromByteData(void* data, size_t dataSize)
{

	ID3D11GeometryShader* shader;
	HRESULT hr;
	hr = _device->CreateGeometryShader(data, dataSize, nullptr, &shader);
	if (FAILED(hr))
	{
		return nullptr;
	}
	return shader;
}

ID3D11PixelShader * DirectX11Interface_Singleton::CreatePixelShaderFromByteData(void* data, size_t dataSize)
{
	ID3D11PixelShader* shader;
	HRESULT hr;

	hr = _device->CreatePixelShader(data, dataSize, nullptr, &shader);
	if (FAILED(hr))
	{
		return nullptr;
	}
	return shader;
}

ID3D11ComputeShader * DirectX11Interface_Singleton::CreateComputeShaderFromByteData(void* data, size_t dataSize)
{
	ID3D11ComputeShader* shader;
	HRESULT hr;

	hr = _device->CreateComputeShader(data, dataSize, nullptr, &shader);
	if (FAILED(hr))
	{
		return nullptr;
	}
	return shader;
}

ID3D11RenderTargetView * DirectX11Interface_Singleton::CreateRenderTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* renderTargetView;

	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		ReleaseCOM(backBuffer);
		return nullptr;
	}

	hr = _device->CreateRenderTargetView(_backBuffer, NULL, &renderTargetView);
	if (FAILED(hr))
	{
		ReleaseCOM(backBuffer);
		ReleaseCOM(renderTargetView);
		return nullptr;
	}
	ReleaseCOM(backBuffer);
	
	return renderTargetView;
}

ID3D11DepthStencilView * DirectX11Interface_Singleton::CreateDepthStencilView()
{
	/*TO BE CREATED WHEN NEEDED*/
	return nullptr;
}

ID3D11DepthStencilState * DirectX11Interface_Singleton::CreateDepthStencilState()
{
	/*TO BE CREATED WHEN NEEDED*/
	return nullptr;
}

ID3D11InputLayout * DirectX11Interface_Singleton::CreateInputLayoutFromByteData(void * data, size_t dataSize)
{
	/*Reflect the shader. We are getting it from Shader reflection bitches!*/
	/*Taken from the internet and modified to accept byte-code.*/
	ID3D11ShaderReflection* shaderReflection = nullptr;
	
	HRESULT hr = D3D11Reflect(data, dataSize, &shaderReflection);
	if (FAILED(hr))
	{
		ErrorBox(L"Unable to create Input Layot, D3D11Reflect returned failed.");
		return nullptr;
	}

	/*Shader Info gathering!*/
	D3D11_SHADER_DESC shaderDesc;
	shaderReflection->GetDesc(&shaderDesc);

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (UINT i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		shaderReflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		/*Determine the DXGI format. However, this should be changable, there are a lot more formats than this!*/
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);


	}
	ID3D11InputLayout* layout;
	hr = _device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), data, dataSize, &layout);

	if (FAILED(hr))
	{
		ErrorBox(L"Unable to create Input Layot, CreateInputLayout returned failed.");
		ReleaseCOM(shaderReflection);
		return nullptr;
	}

	ReleaseCOM(shaderReflection);	

	return layout;
}

ID3D11Buffer * DirectX11Interface_Singleton::CreateConstantBuffer(const unsigned int & sizeOfBuffer, const void * data) const
{
	ID3D11Buffer* constantBuffer;
	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = sizeOfBuffer;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (data)
	{
		D3D11_SUBRESOURCE_DATA subdata;
		memset(&subdata, 0, sizeof(subdata));
		subdata.pSysMem = data;
		hr = _device->CreateBuffer(&desc, &subdata, &constantBuffer);
	}
	else
	{
		hr = _device->CreateBuffer(&desc, NULL, &constantBuffer);
	}
	if (FAILED(hr))
	{
		return nullptr;
	}

	return constantBuffer;
}

ID3D11Buffer * DirectX11Interface_Singleton::CreateVertexBuffer(const unsigned int & sizeOfBuffer, void * data) const
{
	ID3D11Buffer* vertexBuffer;
	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = sizeOfBuffer;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (data)
	{
		D3D11_SUBRESOURCE_DATA subdata;
		memset(&subdata, 0, sizeof(subdata));
		subdata.pSysMem = data;
		hr = _device->CreateBuffer(&desc, &subdata, &vertexBuffer);
	}
	else
	{
		/*Need input data*/
		return nullptr;
	}
	if (FAILED(hr))
	{
		return nullptr;
	}

	return vertexBuffer;
}

ID3D11Buffer * DirectX11Interface_Singleton::CreateIndexBuffer(const unsigned int & sizeOfBuffer, void * data) const
{

	ID3D11Buffer* indexBuffer;
	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = sizeOfBuffer;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (data)
	{
		D3D11_SUBRESOURCE_DATA subdata;
		memset(&subdata, 0, sizeof(subdata));
		subdata.pSysMem = data;
		hr = _device->CreateBuffer(&desc, &subdata, &indexBuffer);
	}
	else
	{
		/*Need input data*/
		return nullptr;
	}
	if (FAILED(hr))
	{
		return nullptr;
	}

	return indexBuffer;
	return nullptr;
}

ID3D11Texture2D * DirectX11Interface_Singleton::CreateTexture2D()
{
	/*To be implemented!*/
	return nullptr;
}

bool DirectX11Interface_Singleton::SetVertexShader(ID3D11VertexShader * shader)
{
	_deviceContext->VSSetShader(shader, nullptr, 0);
	return true;
}

bool DirectX11Interface_Singleton::SetGeometryShader(ID3D11GeometryShader * shader)
{
	_deviceContext->GSSetShader(shader, nullptr, 0);
	return true;
}

bool DirectX11Interface_Singleton::SetPixelShader(ID3D11PixelShader * shader)
{
	_deviceContext->PSSetShader(shader, nullptr, 0);
	return true;
}

bool DirectX11Interface_Singleton::SetComputeShader(ID3D11ComputeShader * shader)
{
	_deviceContext->CSSetShader(shader, nullptr, 0);
	return true;
}

bool DirectX11Interface_Singleton::SetInputLayout(ID3D11InputLayout * layout)
{

	_deviceContext->IASetInputLayout(layout);
	return true;
}

bool DirectX11Interface_Singleton::SetVertexBuffers(unsigned int startingPoint, unsigned int nrOfVertexBuffers, ID3D11Buffer* buffers, unsigned int stride, unsigned int offset)
{
	/*To be implemented with index buffers when needed. Format and the like need to be adressed for that.*/

	_deviceContext->IASetVertexBuffers(startingPoint, nrOfVertexBuffers, &buffers, &stride, &offset);

	return true;
}

bool DirectX11Interface_Singleton::SetVertexShaderBuffers(unsigned int startingPoint, unsigned int nrOfConstantBuffers, ID3D11Buffer * buffers)
{
	_deviceContext->VSSetConstantBuffers(startingPoint, nrOfConstantBuffers, &buffers);
	return true;
}

bool DirectX11Interface_Singleton::SetGeometryShaderBuffers(unsigned int startingPoint, unsigned int nrOfConstantBuffers, ID3D11Buffer * buffers)
{
	_deviceContext->GSSetConstantBuffers(startingPoint, nrOfConstantBuffers, &buffers);
	return true;
}

bool DirectX11Interface_Singleton::SetPixelShaderBuffers(unsigned int startingPoint, unsigned int nrOfConstantBuffers, ID3D11Buffer * buffers)
{
	_deviceContext->PSSetConstantBuffers(startingPoint, nrOfConstantBuffers, &buffers);
	return true;
}

bool DirectX11Interface_Singleton::SetComputeShaderBuffers(unsigned int startingPoint, unsigned int nrOfConstantBuffers, ID3D11Buffer * buffers)
{
	_deviceContext->CSSetConstantBuffers(startingPoint, nrOfConstantBuffers, &buffers);
	return false;
}

bool DirectX11Interface_Singleton::SetRenderTargets(unsigned int nrOfRenderTargets,  ID3D11RenderTargetView* targets, ID3D11DepthStencilView* depthStencil)
{
	if (targets == nullptr)
	{
		_deviceContext->OMSetRenderTargets(NULL, nullptr, nullptr);
	}
	else
	{
		if (_currentRenderTargetView != targets) 
		{
		
			_currentRenderTargetView = targets; /*I swear to never use the targets for anything else than to keep an eye on the adress*/
		}
		if (depthStencil != nullptr && depthStencil != _currentDepthStencilView)
		{
			_currentDepthStencilView = depthStencil;
		}

		_deviceContext->OMSetRenderTargets(nrOfRenderTargets, &_currentRenderTargetView, _currentDepthStencilView);

	}
	
	return false;
}

void DirectX11Interface_Singleton::BeginFrame(ClearColour & colours)
{
	float colour[] = { colours.r, colours.g, colours.b, colours.a };
	_deviceContext->ClearRenderTargetView(_currentRenderTargetView, colour);
	_deviceContext->ClearDepthStencilView(_currentDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectX11Interface_Singleton::Draw()
{

}

void DirectX11Interface_Singleton::PresentFrame()
{
	if (_vSyncEnabled)
	{
		_swapChain->Present(1, 0);
	}
	else
	{
		_swapChain->Present(0, 0);
	}
}

void DirectX11Interface_Singleton::DestroyResource(IUnknown * resource)
{
	ReleaseCOM(resource);
}

void DirectX11Interface_Singleton::Shutdown()
{
	ReleaseCOM(_backBuffer);

	ReleaseCOM(_depthStencilBuffer);
	ReleaseCOM(_depthStencilState);
	ReleaseCOM(_currentDepthStencilView);

	ReleaseCOM(_swapChain);
	ReleaseCOM(_deviceContext);
	ReleaseCOM(_device);
}
