#pragma once
#include <vector>
#include "Entity.h"
#define NUMBER_POS_LIGHTS 4
struct PointLight {
    bool active;
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    PointLight() {
        ambient = glm::vec3(0.0);
        diffuse = glm::vec3(0.0);
        specular = glm::vec3(0.0);
        active = false;
    }
};
struct DirectionalLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
class Scene{
public:
    friend class SceneLoader;
	Scene();
	~Scene();
    void AddEntity(Entity e);
    void makeIMGUI_UI();
    void Draw();
    
private:
    void setLightUniforms(Shader* shader);
    DirectionalLight directionalLight;
    PointLight pointLights[NUMBER_POS_LIGHTS];
    std::vector<Entity> _Entities;
};

