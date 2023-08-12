#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"

Window mainWindow;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

//Vertex Shader
static const char* vertexShader = "Shaders/vertex.shader"; //{project_path}\OpenGL_01\Shaders

//Fragment Shader
static const char* fragmentShader = "Shaders/fragment.shader";

// Función para crear un triángulo en un búfer de vértices
void CreateObjects()
{
    unsigned int indices[] =
    {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    // Coordenadas de los vértices del triángulo en el espacio 3D
    GLfloat vertices[] =
    {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
    meshList.push_back(obj2);
}

void CreateShader() 
{
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vertexShader, fragmentShader);
    shaderList.push_back(*shader1);
}

int main()
{

    mainWindow = Window(800, 600);
    mainWindow.Init();

    CreateObjects();
    CreateShader();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Loop until window closed
	while (!mainWindow.getShouldClose())
	{
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

		//Get and handle user input events
		glfwPollEvents(); //checking any input events / keyboard, mouse...

        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getMouseButtons(), mainWindow.GetXChange(), mainWindow.GetYChange());

		//Clear window
		glClearColor(0.86f, 0.86f, 0.86f, 0.86f); //We can set any color that we want in RGB
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();

        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();

        glm::mat4 model(1.0f); //Indentity matrix by default
        
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); //Mathematical operation translation
        //model = glm::rotate(model, 90 * toRadiands, glm::vec3(0.0f, 1.0f, 0.0f)); //Mathematical operation rotation
        model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f)); //Mathematical operation scale
        //third argument - Transpose is flip the matrix around sort of diagonal axis
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Matricial way to apply transformations
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
        meshList[0]->RenderMesh();


        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); //Mathematical operation translation
        //model = glm::rotate(model, 90 * toRadiands, glm::vec3(0.0f, 1.0f, 0.0f)); //Mathematical operation rotation
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Mathematical operation scale
        //glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Matricial way to apply transformations
        //meshList[1]->RenderMesh();

        //glUseProgram(0);
        mainWindow.swapBuffers(); //Need EXPLANATION

	}

	return 0;
}
