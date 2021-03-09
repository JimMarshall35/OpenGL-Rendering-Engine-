#include "ShaderLoader.h"
#include "jsonhelper.h"
ShaderLoader* ShaderLoader::_instance;
bool ShaderLoader::loadGlobalShaders(void)
{
    using namespace rapidjson;
    Document doc;
    loadJSONFile(doc, "data/models.json");
    if (!doc.HasMember("shaders") || !doc["shaders"].IsArray()) {
        std::cerr << "no member: shaders in models.json or shaders isn't an array" << std::endl;
        return false;
    }
    const Value& shaders = doc["shaders"];
    GlobalShaders = new Shader[shaders.Size()];
    for (SizeType i = 0; i < shaders.Size(); i++)
    {
        if (!shaders[i].HasMember("vertex") || !shaders[i]["vertex"].IsString()) {
            std::cerr << "shader " << i << " doesn't have member, vertex, or it's not a string" << std::endl;
            return false;
        }
        if (!shaders[i].HasMember("fragment") || !shaders[i]["fragment"].IsString()) {
            std::cerr << "shader " << i << " doesn't have member, fragment, or it's not a string" << std::endl;
            return false;
        }
        if (!shaders[i].HasMember("key") || !shaders[i]["key"].IsString()) {
            std::cerr << "shader " << i << " doesn't have member, key, or it's not a string" << std::endl;
            return false;
        }
        const char* vert = shaders[i]["vertex"].GetString();
        const char* frag = shaders[i]["fragment"].GetString();
        
        GlobalShaders[i].load(vert, frag);
        std::string key = static_cast<std::string>(shaders[i]["key"].GetString());
        GlobalShadersMap[key] = &GlobalShaders[i];
    }

    return true;
}

Shader* ShaderLoader::getShader(std::string key)
{
    return GlobalShadersMap[key];
}
