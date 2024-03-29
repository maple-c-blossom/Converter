#include "TextureConverter.h"
#include <stringapiset.h>

using namespace std;
using namespace DirectX;
void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath, int numOptions, char* options[])
{

	LoadWICTextureFromFile(filePath);
	SaveDDSTextureToFile(numOptions, options);
}

void TextureConverter::OutputUsage()
{
	printf("画像ファイルをWIC形式からDDS形式に変換します\n");
	printf("\n");
	printf("Converter [ドライブ:][パス][ファイル名]\n");
	printf("\n");
	printf(" [ドライブ:][パス][ファイル名]:変換したいWIC形式の画像ファイルを指定します");
	printf("\n");
	printf("[ファイル名]の後に -ml mipLevel:mipLevelに数値を入力することでmipLevelを指定します");
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

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[])
{
	size_t mipLevel = 0;

	for (int i = 0; i < numOptions; i++)
	{
		if (std::string(options[i]) == "-ml") {
			mipLevel = std::stoi(options[i + 1]);
			break;
		}
	}

	HRESULT result;
	ScratchImage mipChain;
	result = GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(),
		scratchImage_.GetMetadata(), TEX_FILTER_DEFAULT, mipLevel, mipChain
	);
	if (SUCCEEDED(result))
	{
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	ScratchImage converted;
	result = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(result))
	{
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}




	metadata_.format = MakeSRGB(metadata_.format);
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";
	result = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), 
		metadata_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}
