#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <vector>
#include "ModelDefs.h"
#include <map>
#include <string>
#include "stb_image.h"
#include "jsonhelper.h"
class ModelLoader {

public:
    static ModelLoader* Instance() {
        if (_instance == NULL) {
            _instance = new ModelLoader();
        }
        return _instance;
    }
    bool loadGlobalModels(void);

    Model* getModel(std::string key);
    unsigned int getTexture(const char* path);
    
private:
    ModelLoader() {};
    static ModelLoader* _instance;

    bool LoadModel(const char* path, Model& modelref);
    Model* GlobalModels;
    std::map<std::string, Model*> GlobalModelsMap;
    std::map <std::string, unsigned int> GlobalTexturesMap;
    void printNodes(aiNode* node, int rdepth);
    inline void indent(int num);
    void processNode(aiNode* node, int rdepth, Model& modelref, aiMatrix4x4 matrix);
    void processNode(aiNode* node, ModelDrawNode* targetParent, glm::mat4 accTransform, Model& modelref);
    bool loadEmbeddedModel(const rapidjson::Value& model, Model& modelref);
    inline glm::mat4 convertMatrix(const aiMatrix4x4 &aiMat);
    void SetMaterials(Model& modelref, const aiScene* scene);
    void SetMeshes(Model& modelref, const aiScene* scene);
};






