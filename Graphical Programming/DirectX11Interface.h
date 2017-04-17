#ifndef DIRECT_X_11_INTERFACE
#define DIRECT_X_11_INTERFACE

#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.inl>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


class DirectX11Interface_Singleton
{
	DirectX11Interface_Singleton();
	~DirectX11Interface_Singleton();
	DirectX11Interface_Singleton(const DirectX11Interface_Singleton &original);
	DirectX11Interface_Singleton &operator=(const DirectX11Interface_Singleton &original) = delete;

	/*Basic data needed. Some of these might need "functions" for certain algorithms later*/
	bool _vSyncEnabled = false;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	IDXGISwapChain* _swapChain;
	ID3D11Texture2D* _backBuffer;

	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilState* _depthStencilState;


	ID3D11DepthStencilView* _currentDepthStencilView;
	ID3D11RenderTargetView* _currentRenderTargetView;
	


public:
	/*Struct for colours*/
	struct ClearColour
	{
		float r, g, b, a;
	};
	inline static DirectX11Interface_Singleton &Instance() { static DirectX11Interface_Singleton singleton; return singleton; };

	/*Initialize and Resize functions, TO BE CHANGED*/
	bool Initialize(HWND hWnd);
	bool Resize(); /*Should only be allowed to use this explicity through pre-defined options or fullscreen?*/

	/*Functions to create the shaders*/
	std::vector<unsigned char> ReadShaderByteCode(const char *fileName);
	
	ID3D11VertexShader* CreateVertexShaderFromFile(const char *fileName);
	ID3D11GeometryShader* CreateGeometryShaderFromFile(const char *fileName);
	ID3D11PixelShader* CreatePixelShaderFromFile(const char *fileNamel);
	ID3D11ComputeShader* CreateComputeShaderFromFile(const char *fileName);

	ID3D11VertexShader* CreateVertexShaderFromByteData(void* data, size_t dataSize);
	ID3D11GeometryShader* CreateGeometryShaderFromByteData(void* data, size_t dataSize);
	ID3D11PixelShader* CreatePixelShaderFromByteData(void* data, size_t dataSize);
	ID3D11ComputeShader* CreateComputeShaderFromByteData(void* data, size_t dataSize);

	/*Functions to create RenderTarget and DepthStencil view. Need any information?*/
	ID3D11RenderTargetView* CreateRenderTargetView();
	ID3D11DepthStencilView* CreateDepthStencilView();
	ID3D11DepthStencilState* CreateDepthStencilState();

	/*Functions to create a Layout*/
	/*To be properly implemented once a desicsion has been made of how to do this*/
	ID3D11InputLayout* CreateInputLayoutFromByteData(void* data, size_t dataSize);

	/*Functions to create the buffers. */
	/* WARNING: The Interface has no control over these once returned. */
	ID3D11Buffer* CreateConstantBuffer(const unsigned int &sizeOfBuffer, const void* data = nullptr) const;
	ID3D11Buffer* CreateVertexBuffer(const unsigned int &sizeOfBuffer, void* data) const;
	ID3D11Buffer* CreateIndexBuffer(const unsigned int &sizeOfBuffer, void* data) const;

	/*Functions to create textures*/
	ID3D11Texture2D* CreateTexture2D();

	/*Set the different shaders*/
	bool SetVertexShader(ID3D11VertexShader* shader);
	bool SetGeometryShader(ID3D11GeometryShader* shader);
	bool SetPixelShader(ID3D11PixelShader* shader);
	bool SetComputeShader(ID3D11ComputeShader* shader);

	/*Set the input layot*/
	bool SetInputLayout(ID3D11InputLayout* layout);

	/*Functions to set the buffers. => Use shader reflection to "find" the spot to put it in*/
	bool SetVertexBuffers(unsigned int startingPoint, unsigned int nrOfVertexBuffers, ID3D11Buffer* buffers, unsigned int stride, unsigned int offset);
	bool SetVertexShaderBuffers(unsigned int startingPoint, unsigned int nrOfConstantBuffers, ID3D11Buffer* buffers);
	bool SetGeometryShaderBuffers(unsigned int startingPoint, unsigned int nrOfConstantBuffers, ID3D11Buffer* buffers);
	bool SetPixelShaderBuffers(unsigned int startingPoint, unsigned int nrOfConstantBuffers, ID3D11Buffer* buffers);
	bool SetComputeShaderBuffers(unsigned int startingPoint, unsigned int nrOfConstantBuffers, ID3D11Buffer* buffers);

	/*Set the rendertarget*/
	bool SetRenderTargets(unsigned int nrOfRenderTargets, ID3D11RenderTargetView* targets = nullptr, ID3D11DepthStencilView* depthStencil = nullptr); /*Send in 0 to clear*/
	void BeginFrame(ClearColour &colours);
	void Draw(); /*All information should've been set by this call! (Need any information?)*/
	void PresentFrame();

	/*Destroy Resource can be used with ANY object created*/
	void DestroyResource(IUnknown* resource);

	/*Shutdown the Interaface*/
	void Shutdown();

};









#endif
