#pragma once
#include "Scene.h"
class SceneLoader
{
public:
	
    Scene loadScene(const char* filepath);
    static SceneLoader* Instance() {
        if (_instance == NULL) {
            _instance = new SceneLoader();
        }
        return _instance;
    }
private:
    SceneLoader(){}
    static SceneLoader* _instance;
};

