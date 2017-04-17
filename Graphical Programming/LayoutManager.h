#pragma once

#include <unordered_map>

/*PROBLEM: Removing a specific shader WILL NOT MAKE IT "AVAILABLE" TO USE*/

struct IUnknown;
#define MAXIMAL_NUMBER_OF_LAYOUTS 256

class LayoutManager
{
private:
	size_t _nrOfLayouts;
	std::unordered_map<size_t, IUnknown*> _layoutMap;

	LayoutManager(const LayoutManager &original) = delete;
	LayoutManager &operator=(const LayoutManager &original) = delete;

public:
	LayoutManager();
	~LayoutManager();

	IUnknown* AddLayout(char* fileName, std::vector<unsigned char> &shaderByteCode);
	void BindLayout(size_t identifier);
	void DestroyLayout(size_t identifier);

};
