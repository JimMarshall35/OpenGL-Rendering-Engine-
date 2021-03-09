#pragma once
#include "ModelDefs.h"
#include "WindowDefs.h"
#include "Cameras.h"
#include <string>

class Entity
{
public:
	Entity();
	~Entity();
    Transform transform;
    void setModel(std::string key);
    void setModel(Model* model_) { model = model_; }
    void setShader(Shader* s) { shader = s; }
    void draw(void);
    std::string getModelName() { return modelname; }
private:
    std::string modelname;
    //void setUniforms(void);
    Model* model;
    Shader* shader;
};

