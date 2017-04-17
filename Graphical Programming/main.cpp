#include "WindowClass.h"
#include "InputControl.h"
#include "DirectX11Interface.h"
#include "GraphicInterface.h"
#include "PipelineManager.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(221);

	WindowClass mainWindow;
	InputControl_Singleton::Instance().Initialize(); 
	if (mainWindow.Initialize())
	{
		DirectX11Interface_Singleton::Instance().Initialize(mainWindow.GetWindowHandle());
		ID3D11RenderTargetView* test = DirectX11Interface_Singleton::Instance().CreateRenderTargetView();
		int nrOfRenderTargets = 1;
		DirectX11Interface_Singleton::Instance().SetRenderTargets(nrOfRenderTargets, test);
		PipelineManager::PipelineDescription desc;
		desc.vertexShaderFile = "Shaders\\TestShader.cso";
		PipelineManager manager;
		auto t = manager.AddPipeline(desc);
		MSG msg;
		bool done;

		//Initialize message structure;
		ZeroMemory(&msg, sizeof(MSG));


		//Loop until there is a quit message from the windows or the user
		done = false;
		while (!done)
		{
			//Handle the window messages
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			//If we tell the window to quit, quit.
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				DirectX11Interface_Singleton::ClearColour colour;
				colour.a = 1.0f;
				colour.r = .35f;
				colour.b = 0.25f;
				colour.g = 0.45f;
				DirectX11Interface_Singleton::Instance().SetRenderTargets(0);
				DirectX11Interface_Singleton::Instance().BeginFrame(colour);
				DirectX11Interface_Singleton::Instance().PresentFrame();
			}
		}
		DirectX11Interface_Singleton::Instance().DestroyResource(test);
		GraphicInterface_Singleton::Instance().Shutdown();
		DirectX11Interface_Singleton::Instance().Shutdown();
	}
	


	return 0;
}