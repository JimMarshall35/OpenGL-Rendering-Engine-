#pragma once
#include "Scene.h"
#include "jsonhelper.h"
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
    void loadPointLights(Scene& scene, const rapidjson::Document& doc);
    void loadEntities(Scene& scene, const rapidjson::Document& doc);
    void loadDirectionalLight(Scene& scene, const rapidjson::Document& doc);
};

