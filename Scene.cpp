#include "Scene.h"
#include "imgui.h"
#include <string>
#include "ErrorHandliong.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::AddEntity(Entity e)
{
    _Entities.push_back(e);
}

void Scene::makeIMGUI_UI()
{
    // Create a window called "Hello, world!" and append into it.
    ImGui::Begin("Hello, world!");                 
    for (Entity e : _Entities) {
        std::string name = e.getModelName();
        ImGui::DragFloat3((name + " position").c_str(), &e.transform.translation.x);
        ImGui::DragFloat3((name + " rotation").c_str(), &e.transform.rotation.x);
        ImGui::DragFloat3((name + " scale").c_str(), &e.transform.scale.x);
    }
    ImGui::Text("camera pos = x: %f, y: %f, z: %f",
        Cameras::instance().getActive()->Position.x,
        Cameras::instance().getActive()->Position.y,
        Cameras::instance().getActive()->Position.z);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


    ImGui::End();
}

void Scene::Draw()
{
    for (size_t i = 0; i < _Entities.size(); i++) {
        setLightUniforms(_Entities[i].shader);
        _Entities[i].draw();
    }
}

void Scene::setLightUniforms(Shader * shader)
{
    GLClearErrors();
    for (int i = 0; i < NUMBER_POS_LIGHTS; i++) {
        std::stringstream ss;
        ss << i;
        std::string name = std::string("pointLights[") + ss.str() + std::string("]");
        shader->setVec3(name + ".position", pointLights[i].position);
        GLPrintErrors("setting pointLight position uniform");

        shader->setFloat(name + ".constant", pointLights[i].constant);
        GLPrintErrors("setting pointLight constant uniform");
        shader->setFloat(name + ".linear", pointLights[i].linear);
        GLPrintErrors("setting pointLight linear uniform");
        shader->setFloat(name + ".quadratic", pointLights[i].quadratic);
        GLPrintErrors("setting pointLight quadratic uniform");

        shader->setVec3(name + ".ambient", pointLights[i].ambient);
        GLPrintErrors("setting pointLight ambient uniform");
        shader->setVec3(name + ".diffuse", pointLights[i].diffuse);
        GLPrintErrors("setting pointLight diffuse uniform");
        shader->setVec3(name + ".specular", pointLights[i].specular);
        GLPrintErrors("setting pointLight specular uniform");
    }
    shader->setVec3("dirLight.ambient", directionalLight.ambient);
    GLPrintErrors("setting directionalLight ambient uniform");
    shader->setVec3("dirLight.diffuse", directionalLight.diffuse);
    GLPrintErrors("setting directionalLight diffuse uniform");
    shader->setVec3("dirLight.specular", directionalLight.specular);
    GLPrintErrors("setting directionalLight specular uniform");
    shader->setVec3("dirLight.direction", directionalLight.direction);
    GLPrintErrors("setting directionalLight direction uniform");

}
