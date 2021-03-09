#pragma once
#include <vector>
#include "Entity.h"
class Scene
{
public:
	Scene();
	~Scene();
    void AddEntity(Entity e);
    void makeIMGUI_UI();
    void Draw();
private:
    std::vector<Entity> _Entities;
};

