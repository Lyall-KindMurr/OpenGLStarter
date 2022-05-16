#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include "plane.h"
#include "objmesh.h"
#include "helper/cube.h"
#include "helper/torus.h"
#include <glm/glm.hpp>

#include <glad/glad.h>
#include "helper/glslprogram.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;
    Plane plane;
    Cube cube;
    Torus torus;

    GLuint sprites;
    int numSprites;
    float* locations;
    
    std::unique_ptr<ObjMesh> mesh;

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void setMatrices();
};

#endif // SCENEBASIC_UNIFORM_H
