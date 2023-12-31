#include "Mesh.h"

Mesh::Mesh() 
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}


void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

    // Crear y configurar un Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);           // Generar un VAO
    glBindVertexArray(VAO);               // Vincular el VAO activo

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

    // Crear y configurar un Vertex Buffer Object (VBO)
    glGenBuffers(1, &VBO);                // Generar un VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);   // Vincular el VBO activo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // Cargar los datos en el VBO

    // Configurar el atributo de v�rtice (posici�n) en el shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Definir el atributo de posici�n
    glEnableVertexAttribArray(0);                          // Habilitar el atributo en el VAO

    glBindBuffer(GL_ARRAY_BUFFER, 0);    // Desvincular el VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);                // Desvincular el VAO
}

void Mesh::RenderMesh()
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
    if (IBO != 0)
    {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
}

Mesh::~Mesh()
{
    ClearMesh();
}