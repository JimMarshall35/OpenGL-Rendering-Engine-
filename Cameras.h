#pragma once
#include "Camera.h"
#include <vector>
class Cameras {
public:
    
    Camera* getActive(void) {
        return &_Cameras[activeCamera];
    }
    static Cameras& instance() {
        if (_instance == NULL) {
            _instance = new Cameras();
        }
        return *_instance;
    }
private:
    Cameras() {
        Camera c;
        _Cameras.push_back(c);
        activeCamera = 0;
    }
    std::vector<Camera> _Cameras;
    unsigned short activeCamera;
    static Cameras* _instance;
};
