#include "SceneLoader.h"

#include "ShaderLoader.h"


SceneLoader* SceneLoader::_instance;
Scene SceneLoader::loadScene(const char * filepath)
{
    Scene scene;
    using namespace rapidjson;
    Document doc;
    loadJSONFile(doc, filepath);

    loadEntities(scene, doc);
    loadPointLights(scene, doc);
    loadDirectionalLight(scene, doc);

    
    return scene;
}

void SceneLoader::loadPointLights(Scene& scene, const rapidjson::Document& doc)
{
    using namespace rapidjson;
    const Value& pos_lights = doc["positional_lights"];
    SizeType len;
    if (pos_lights.Size() > NUMBER_POS_LIGHTS) {
        std::cout << std::endl;
        std::cout << "max number of positional lights, " << NUMBER_POS_LIGHTS << " exceeded" << std::endl;
        std::cout << std::endl;
        len = NUMBER_POS_LIGHTS;
    }
    else {
        len = pos_lights.Size();
    }
    for (SizeType i = 0; i < len; i++) {
        const Value& pos_light = pos_lights[i];
        scene.pointLights[i].active = true;

        scene.pointLights[i].position = glm::vec3(
            pos_light["position_x"].GetFloat(),
            pos_light["position_y"].GetFloat(),
            pos_light["position_z"].GetFloat()
        );
        scene.pointLights[i].specular = glm::vec3(
            pos_light["specular_r"].GetFloat(),
            pos_light["specular_g"].GetFloat(),
            pos_light["specular_b"].GetFloat()
        );
        scene.pointLights[i].diffuse = glm::vec3(
            pos_light["diffuse_r"].GetFloat(),
            pos_light["diffuse_g"].GetFloat(),
            pos_light["diffuse_b"].GetFloat()
        );
        scene.pointLights[i].ambient = glm::vec3(
            pos_light["ambient_r"].GetFloat(),
            pos_light["ambient_g"].GetFloat(),
            pos_light["ambient_b"].GetFloat()
        );
        scene.pointLights[i].constant = pos_light["constant"].GetFloat();
        scene.pointLights[i].linear = pos_light["linear"].GetFloat();
        scene.pointLights[i].quadratic = pos_light["quadratic"].GetFloat();

    }
}

void SceneLoader::loadEntities(Scene & scene, const rapidjson::Document & doc)
{
    using namespace rapidjson;
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
}

void SceneLoader::loadDirectionalLight(Scene & scene, const rapidjson::Document & doc)
{
    using namespace rapidjson;
    const Value& directionalLight = doc["directional_light"];

    scene.directionalLight.direction = glm::vec3(
        directionalLight["direction_x"].GetFloat(),
        directionalLight["direction_y"].GetFloat(),
        directionalLight["direction_z"].GetFloat()
    );
    scene.directionalLight.specular = glm::vec3(
        directionalLight["specular_r"].GetFloat(),
        directionalLight["specular_g"].GetFloat(),
        directionalLight["specular_b"].GetFloat()
    );
    scene.directionalLight.diffuse = glm::vec3(
        directionalLight["diffuse_r"].GetFloat(),
        directionalLight["diffuse_g"].GetFloat(),
        directionalLight["diffuse_b"].GetFloat()
    );
    scene.directionalLight.ambient = glm::vec3(
        directionalLight["ambient_r"].GetFloat(),
        directionalLight["ambient_g"].GetFloat(),
        directionalLight["ambient_b"].GetFloat()
    );
}
