// Project Headers
#include <__expected/unexpected.h>
#include <algorithm>
#include <exception>
#include <fstream>
#include <orb/fileparsing/file_loading.hpp>
#include <orb/log/assert.hpp>


// Libraries
// ICU
#include <unicode/errorcode.h>
#include <unicode/ucnv.h>
#include <unicode/ucnv_err.h>
#include <unicode/ucsdet.h>
#include <unicode/urename.h>

namespace orb::fileparsing
{

void InitDefaultUnicode() { ucnv_setDefaultName(ucnv_getDefaultName()); }

void InitUnicodeParsing(const char* rawFile, int32_t len)
{
    icu::ErrorCode result;
    UCharsetDetector* detector = ucsdet_open(result);
    ASSERT(result.isSuccess(), "There was an error opening the characterset detector from ICU lib");

    ucsdet_setText(detector, rawFile, len, result);
    ASSERT(result.isSuccess(),
           "There was an error loading the text while detecting the fileencoding");

    const UCharsetMatch* charset = ucsdet_detect(detector, result);
    ASSERT(result.isSuccess(), "There was an error detecting the fileencoding of the current file");
    ASSERT(charset != NULL, "Couldn't find suitable fileencoding of file");

    const char* converterName = ucsdet_getName(charset, result);
    ASSERT(result.isSuccess(), "Couldn't get name of the files characterset");

    ucnv_setDefaultName(converterName);
}

[[clang::always_inline]]
inline std::expected<icu::UnicodeString, std::string> LoadFilePath(std::filesystem::path path)
{
    std::ifstream fileStream{path};
    if (fileStream.fail())
        return std::unexpected{std::string{"Failed to open file"}};

    int32_t fileSize = static_cast<int32_t>(std::filesystem::file_size(path));

    // Create an iterator over every character of the fileStream
    // And copy every character into the vector
    // std::vector is used over std::string because we only care about the raw data,
    // and to avoid std::string overhead
    std::istreambuf_iterator<char> fileIter{fileStream}, endOfStream{};
    std::vector<char> fileBytes(fileSize);
    std::copy(fileIter, endOfStream, fileBytes.begin());

    // fileSize - 1 because of an extra `\n` character at the end of all files
    return icu::UnicodeString{fileBytes.data(), fileSize - 1};
}

template <>
std::expected<icu::UnicodeString, std::string> LoadFile<const char*>(const char* path)
{
    try
    {
        std::filesystem::path filePath{path};
        return LoadFilePath(filePath);
    }
    catch (std::exception& e)
    {
        return std::unexpected(e.what());
    }
}

template <>
std::expected<icu::UnicodeString, std::string> LoadFile<const std::string&>(const std::string& path)
{
    try
    {
        std::filesystem::path filePath{path};
        return LoadFilePath(filePath);
    }
    catch (std::exception& e)
    {
        return std::unexpected(e.what());
    }
}

template <>
std::expected<icu::UnicodeString, std::string> LoadFile<std::string_view>(std::string_view path)
{
    try
    {
        std::filesystem::path filePath{path};
        return LoadFilePath(filePath);
    }
    catch (std::exception& e)
    {
        return std::unexpected(e.what());
    }
}



} // namespace orb::fileparsing