//
// Created by jonat on 08/04/2024.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include "Camera/Camera.h"
#include "Mesh/Mesh.h"
#include "Mesh/Actor.h"


//class VertexBuffer;
//class ElementBuffer;
struct GLFWwindow;

class Application {

public:
	Application(){}
    GLFWwindow *mWindow;
    static Camera mCamera;
    static float DeltaTime;
    
    Cube Player;
    
    Sphere sphere;
    Sphere sphere2;
    Sphere sphere3;
    Sphere sphere4;
    Sphere sphere5;
    Sphere sphere6;
    Sphere sphere7;
    Sphere sphere8;

    Cube Floor;
    Cube Wall;
    Cube Wall2;
    Cube Wall3;
    Cube Wall4;

  

    void init();
    void create();
    void update();
    void run();

    virtual ~Application();

private:
  /*  VertexBuffer* tVBO = nullptr;
    ElementBuffer* tEBO = nullptr;*/
};



#endif //APPLICATION_H
