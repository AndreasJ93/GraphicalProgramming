#include "LayoutManager.h"
#include "DirectX11Interface.h"

LayoutManager::LayoutManager()
{
	_nrOfLayouts = 0;
}

LayoutManager::~LayoutManager()
{
	for (auto layout : _layoutMap)
		DirectX11Interface_Singleton::Instance().DestroyResource(layout.second);

}

IUnknown* LayoutManager::AddLayout(char* fileName, std::vector<unsigned char>& shaderByteCode)
{
	IUnknown* toSave = nullptr;
	std::hash<char*> hash;
	size_t key = hash(fileName);
	/*ONLY ADD THE SHADER IF IT DOES NOT ALREADY EXISTS!*/
	auto layout = _layoutMap.find(key);
	if (layout == _layoutMap.end())
	{
		if (shaderByteCode.size())
		{
			toSave = DirectX11Interface_Singleton::Instance().CreateInputLayoutFromByteData(&shaderByteCode[0], shaderByteCode.size());
			_layoutMap[key] = toSave;
		}
	}
	else
	{
		toSave = layout->second;
	}
	return toSave;
}

void LayoutManager::BindLayout(size_t identifier)
{

}

void LayoutManager::DestroyLayout(size_t identifier)
{

}
