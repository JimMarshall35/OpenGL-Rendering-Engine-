#include "Scene.h"
#include "imgui.h"
#include <string>

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
        _Entities[i].draw();
    }
}
