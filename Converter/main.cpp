#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <DirectXTex.h>
#include "TextureConverter.h"
int main(int32_t argc,char* argv[])
{
	enum Argument
	{
		kApplicationPath,
		kFilePath,
		NumArgument
	};
	assert(argc >= NumArgument);
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));
	TextureConverter convert;
	convert.ConvertTextureWICToDDS(argv[kFilePath]);
	for (int32_t i = 0; i < argc; i++)
	{
		printf(argv[i]);
		printf("\n");
	}
	CoUninitialize();
	system("pause");
	return 0;
}