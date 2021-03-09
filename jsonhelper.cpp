#include "jsonhelper.h"
bool loadJSONFile(rapidjson::Document &jsonDoc, const char *pFileName)
{
    // Load the file
    FILE *fp = nullptr;
    fopen_s(&fp, pFileName, "rt");
    if (fp == nullptr)
        return false;

    fseek(fp, 0, SEEK_END);
    const size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *pFileText = static_cast<char*>(malloc(fileSize));
    fread(pFileText, 1, fileSize, fp);
    fclose(fp);
    std::cout << pFileText << std::endl;

    // Parse the JSON
    const bool parseError = jsonDoc.Parse<rapidjson::kParseStopWhenDoneFlag>(pFileText).HasParseError();

    free(pFileText);

    if (parseError)
    {
        rapidjson::ParseErrorCode error = jsonDoc.GetParseError();
        std::cout << "ERROR" << std::endl;
        return false;
    }

    return true;
}
