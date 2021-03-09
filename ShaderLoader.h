#pragma once
#include "Shader.h"
#include <map>
#include <string>

class ShaderLoader {
public:
    static ShaderLoader* Instance() {
        if (_instance == NULL) {
            _instance = new ShaderLoader();
        }
        return _instance;
    }
    bool loadGlobalShaders(void);
    Shader* getShader(std::string key);
private:
    ShaderLoader() {};
    static ShaderLoader* _instance;
    Shader* GlobalShaders;
    std::map<std::string, Shader*> GlobalShadersMap;
};




