#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600; //Screen size

GLuint VAO, VBO, shader, uniformModel; //Unsigned GL int variable, can't contain negative values.

bool direction = true;
float triOffset = 0.0f, triMaxOffset = 0.5f, triIncrement = 0.005f;

const float toRadiands = 3.14159265f / 180.0f;

//Vertex Shader
static const char* vertexShader = R"(
    #version 330 core

    layout(location = 0) in vec3 position;

    uniform mat4 model;

    void main() 
	{
        gl_Position = model * vec4(position, 1.0);
    }
)";

//Fragment Shader
static const char* fragmentShader = R"(
    #version 330 core

    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
)";

// Función para crear un triángulo en un búfer de vértices
void CreateTriangle()
{
    // Coordenadas de los vértices del triángulo en el espacio 3D
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    // Crear y configurar un Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);           // Generar un VAO
    glBindVertexArray(VAO);               // Vincular el VAO activo

    // Crear y configurar un Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);                // Generar un VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);   // Vincular el VBO activo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW); // Cargar los datos en el VBO

    // Configurar el atributo de vértice (posición) en el shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Definir el atributo de posición
    glEnableVertexAttribArray(0);                          // Habilitar el atributo en el VAO

    glBindBuffer(GL_ARRAY_BUFFER, 0);    // Desvincular el VBO
    glBindVertexArray(0);                // Desvincular el VAO
}

// Función para agregar un shader a un programa
void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint _shader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(_shader, 1, code, codeLength); // Cargar el código del shader
    glCompileShader(_shader);                     // Compilar el shader

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(_shader, GL_COMPILE_STATUS, &result); // Obtener el estado de compilación del shader
    if (!result)
    {
        glGetShaderInfoLog(_shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(program, _shader); // Adjuntar el shader al programa
}

// Función para compilar los shaders y crear el programa
void CompileShaders()
{
    shader = glCreateProgram(); // Crear un programa de shaders

    if (!shader)
    {
        printf("Error creating shader program!");
        return;
    }

    // Agregar los shaders al programa
    AddShader(shader, vertexShader, GL_VERTEX_SHADER);     // Agregar el shader de vértices
    AddShader(shader, fragmentShader, GL_FRAGMENT_SHADER); // Agregar el shader de fragmentos

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader); // Vincular los shaders en el programa
    glGetProgramiv(shader, GL_LINK_STATUS, &result); // Obtener el estado de vinculación del programa

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader); // Validar el programa
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result); // Obtener el estado de validación del programa

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
}


int main()
{

	//Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialisation failed.");
		glfwTerminate();
		return 1;
	}

	//Setup glfw window properties
	//Setting the OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Make sure we are usen long term support code / Core profile = No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Allow forward compatibility

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "XR SOFTWARE DEVELOPER!", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	//Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	

	//Set context for GLEW to use / I will tie all that is gonna be drawn is gonna be in the mainWindow
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extension feature  
	glewExperimental = GL_TRUE;

	//Initializing GLEW
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Get and handle user input events
		glfwPollEvents(); //checking any input events / keyboard, mouse...

        if (direction)
        {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset)
        {
            //direction = !direction;
        }

		//Clear window
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //We can set any color that we want in RGB
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

        glm::mat4 model(1.0f); //Indentity matrix by default
        model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f)); //Mathematical operation translation
        model = glm::rotate(model, 0 * toRadiands, glm::vec3(0.0f, 0.0f, 1.0f)); //Mathematical operation rotation
        model = glm::scale(model, glm::vec3(2.0f,2.0f,1.0f)); //Mathematical operation rotation


        //glUniform1f(uniformModel, triOffset); //Plain way to move an object

        //third argumetn - Transpose is flip the matrix around sort of diagonal axis
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //Matricial way to apply transformations

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow); //Need EXPLANATION

	}


	return 0;
}
