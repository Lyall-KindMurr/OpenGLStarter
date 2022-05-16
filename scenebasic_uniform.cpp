#include "scenebasic_uniform.h"
#include <sstream>

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "helper/texture.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f, 50.0f, 100, 100), torus(0.7,0.35,30,70)
{
    mesh = ObjMesh::load("./media/pig_triangulated.obj", true);
}

void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);
    
    view = glm::lookAt(vec3(-1.5f, 1.0f, 2.5f),
        vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    //code to set up 3 lights with different colors around the model
    /*
    //prog.setUniform("lights[0].Ld", 1.0f, 1.0f, 1.0f); //color of light RGB
    //prog.setUniform("lights[0].Ls", 1.0f, 1.0f, 1.0f); // color of the shine
    prog.setUniform("lights[0].La", 0.0f, 0.0f, 0.1f); //base diffuse color
    prog.setUniform("lights[0].L", 0.0f, 0.0f, 0.8f); // RGB light intensity

    prog.setUniform("lights[1].La", 0.0f, 0.1f, 0.0f); // color of nothing, basically
    prog.setUniform("lights[1].L", 0.0f, 0.8f, 0.0f);

    prog.setUniform("lights[2].La", 0.1f, 0.0f, 0.0f);
    prog.setUniform("lights[2].L", 0.8f, 0.0f, 0.0f);

    float x, z;
    for (int i = 0; i < 3; i++)
    {
        std::stringstream name;
        name << "lights[" << i << "].LightPosition";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z +
            1.0f, 1.0f));
    }
    */

    

    //blinnPhong 1 light set-up
    prog.setUniform("lights.La", 0.2f, 0.3f, 0.4f); //base diffuse color
    prog.setUniform("lights.L", 1.0f, 1.0f, 1.0f); // RGB light intensity
    prog.setUniform("lights.LightPosition", view * glm::vec4(2, 1.2f, 1.0f, 0.0f));

    //BlinnPhong with spotlight set-up
    prog.setUniform("Spot.L", vec3(0.9f));
    prog.setUniform("Spot.La", vec3(0.5f));
    prog.setUniform("Spot.Exponent", 50.0f);
    prog.setUniform("Spot.Cutoff", glm::radians(15.0f));


    //adding fog
    prog.setUniform("Fog.MaxDist", 30.0f);
    prog.setUniform("Fog.MinDist", 1.0f);
    prog.setUniform("Fog.Color", vec3(0.5f, 0.5f, 0.5f));

    /*
    /// sprite content

    numSprites = 50;
    locations = new float[numSprites * 3];
    srand((unsigned int)time(0));

    for (int i = 0; i < numSprites; i++)
    {
        vec3 p(((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f);
        locations[i * 3] = p.x;
        locations[i * 3 + 1] = p.y;
        locations[i * 3 + 2] = p.z;
    }

    
    GLuint handle;
    glGenBuffers(1, &handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, numSprites * 3 * sizeof(float), locations, GL_STATIC_DRAW);

    delete[] locations;

    glGenVertexArrays(1, &sprites);
    glBindVertexArray(sprites);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    const char* texName = "../Coursework 1/media/hay.jpg";
    Texture::loadTexture(texName);

    prog.setUniform("SpriteTex", 0);
    prog.setUniform("Size2", 0.15f);
    */
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
        //prog.compileShader("shader/particles.vert");
        //prog.compileShader("shader/particles.frag");
        //prog.compileShader("shader/particles.geom");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    model = glm::rotate(model, glm::radians(1.0f), vec3(0.0f, 1.0f, 1.0f));
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // stuff for sprittes
    /*
    vec3 cameraPos(0.0f, 0.0f, 3.0f);
    view = glm::lookAt(cameraPos,
        vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(sprites);
    glDrawArrays(GL_POINTS, 0, numSprites);

    glFinish();
    */

    

    //second part of directional light
    glm::vec4 lightPos = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    prog.setUniform("Spot.Position", vec3(view * lightPos));
    //glm::mat3 normalMatrix = glm::mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    prog.setUniform("Spot.Direction", vec3(0.0f, 50.0f, 0.0f));

    //pig

    glBindTexture(GL_TEXTURE_2D, 0);
    prog.setUniform("material.Kd", 0.9f, 0.6f, 0.7f);
    prog.setUniform("material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("material.Ks", 1.0f, 1.0f, 1.0f);
    prog.setUniform("material.shininess", 200.0f);

    model = mat4(1.0f);

    model = glm::rotate(model, glm::radians(-65.0f), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    mesh->render();

    //torus

    prog.setUniform("material.Kd", 0.6f, 0.9f, 0.3f);
    prog.setUniform("material.Ka", 0.3f, 0.1f, 0.3f);
    prog.setUniform("material.Ks", 1.0f, 1.0f, 1.0f);
    prog.setUniform("material.shininess", 500.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(-1.25f, 0.0f, 0.0f));

    setMatrices();
    torus.render();

    //floor
    /*
    prog.setUniform("material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("material.Ks", 0.2f, 0.2f, 0.2f);
    prog.setUniform("material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("material.shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    
    setMatrices();
    plane.render();
    */
    
        //texturing
    GLuint texID1 = Texture::loadTexture("./media/brick1.jpg");
    GLuint texID2 = Texture::loadTexture("./media/hay.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID1);
    //glBindTexture(GL_TEXTURE_2D, 0); //set the texture to nothing

    prog.setUniform("material.Kd", 1.0f, 1.0f, 1.0f);
    prog.setUniform("material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("material.Ks", 1.0f, 1.0f, 1.0f);
    prog.setUniform("material.shininess", 200.0f);

    model = mat4(1.0f);

    model = glm::rotate(model, glm::radians(-80.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(1.25f, 0.0f, 0.0f));
    setMatrices();
    cube.render();

    // big plane

    glBindTexture(GL_TEXTURE_2D, 0);
    prog.setUniform("material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("material.Ks", 0.2f, 0.2f, 0.2f);
    prog.setUniform("material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("material.shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    model = glm::rotate(model, glm::radians(20.0f), vec3(0.0f, 1.0f, 0.0f));

    setMatrices();
    plane.render();

    
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model;

    prog.setUniform("itemView.ModelViewMatrix", mv);
    prog.setUniform("itemView.NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("itemView.MVP", projection * mv);
    prog.setUniform("itemView.View", view);   

    // for sprites
    /*
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("ProjectionMatrix", projection);
    */
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.1f, 100.0f);
}
