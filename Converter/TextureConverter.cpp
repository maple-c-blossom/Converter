#include "TextureConverter.h"
#include <stringapiset.h>

using namespace std;
using namespace DirectX;
void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath)
{

	LoadWICTextureFromFile(filePath);
	SaveDDSTextureToFile();
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	wstring wfilePath = ConvertMultiByteStringToWideString(filePath);
	SeparateFilePath(wfilePath);
	HRESULT result = LoadFromWICFile(wfilePath.c_str(), WIC_FLAGS_NONE, &metadata_, scratchImage_);
	assert(SUCCEEDED(result));

}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	const auto multiByte = mString.length();
	wstring ret(multiByte,L'\0');
	MultiByteToWideChar(CP_ACP, 0, &mString.front(), static_cast<int32_t>(multiByte),
		&ret.front(), static_cast<int32_t>(ret.size()));
	return ret;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{
	size_t pos1;
	std::wstring exceptExt;
	pos1 = filePath.rfind('.');
	if (pos1 != std::wstring::npos)
	{
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		exceptExt = filePath.substr(0, pos1);
	}
	else
	{
		fileExt_ = L"";
		exceptExt = filePath;
	}
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos)
	{
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos)
	{
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	directoryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile()
{
	metadata_.format = MakeSRGB(metadata_.format);
	HRESULT result;
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";
	result = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), 
		metadata_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}
