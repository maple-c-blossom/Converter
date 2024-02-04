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
	if (argc < NumArgument)
	{
		TextureConverter::OutputUsage();
		return -1;
	}

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	int numOptions = argc - NumArgument;
	char** options = argv + NumArgument;

	TextureConverter convert;
	convert.ConvertTextureWICToDDS(argv[kFilePath],numOptions,options);
	for (int32_t i = 0; i < argc; i++)
	{
		printf(argv[i]);
		printf("\n");
	}
	CoUninitialize();
	return 0;
}