#include "TextureConverter.h"
#include <stringapiset.h>

using namespace std;
void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath)
{

	LoadWICTextureFromFile(filePath);
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	wstring wfilePath = ConvertMultiByteStringToWideString(filePath);

}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	const auto multiByte = mString.length();
	wstring ret(multiByte,L'\0');
	MultiByteToWideChar(CP_ACP, 0, &mString.front(), static_cast<int32_t>(multiByte),
		&ret.front(), static_cast<int32_t>(ret.size()));
	return ret;
}
