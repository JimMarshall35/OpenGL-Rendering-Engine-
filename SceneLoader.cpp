#include "SceneLoader.h"
#include "jsonhelper.h"
#include "ShaderLoader.h"


SceneLoader* SceneLoader::_instance;
Scene SceneLoader::loadScene(const char * filepath)
{
    Scene scene;
    using namespace rapidjson;
    Document doc;
    loadJSONFile(doc, filepath);
    const Value& entities = doc["entities"];
    for (SizeType i = 0; i < entities.Size(); i++) {
        const Value& json_entity = entities[i];
        const char* model = json_entity["model"].GetString();
        const char* shader = json_entity["shader"].GetString();

        Entity entity;
        entity.setModel(model);
        entity.setShader(ShaderLoader::Instance()->getShader(shader));

        if (json_entity.HasMember("transform")) {
            if (json_entity["transform"].HasMember("scale")) {
                const Value& scale = json_entity["transform"]["scale"];
                entity.transform.scale.x = scale["x"].GetFloat();
                entity.transform.scale.y = scale["y"].GetFloat();
                entity.transform.scale.z = scale["z"].GetFloat();
            }
            if (json_entity["transform"].HasMember("position")) {
                const Value& pos = json_entity["transform"]["position"];
                entity.transform.translation.x = pos["x"].GetFloat();
                entity.transform.translation.y = pos["y"].GetFloat();
                entity.transform.translation.z = pos["z"].GetFloat();
            }
            if (json_entity["transform"].HasMember("rotation")) {
                const Value& rotation = json_entity["transform"]["rotation"];
                entity.transform.rotation.x = rotation["x"].GetFloat();
                entity.transform.rotation.y = rotation["y"].GetFloat();
                entity.transform.rotation.z = rotation["z"].GetFloat();
            }
        }
        scene.AddEntity(entity);
    }
    return scene;
}
