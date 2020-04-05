#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;
	
    // Starting To Create Vertex Buffer from Points for Triangle
    glGenVertexArrays(1, &VAO); // This assigns ID to this VAO 
    glBindVertexArray(VAO);

    // Assigning ID to IBO
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);

            // The above code automatically binds VBO AND IBO
            glGenBuffers(1, &VBO);  // This assign ID to this VBO
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);   // Unbinding Buffer from previous buffer and Binding it to Nothing
        glBindVertexArray(0);   // Unbinding Current Vertex array to null
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh()
{
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3); // This was done when Triangle vertices were directly used as for creating mesh
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);   // This is done when Triangle vertices references are used through IBO as Indexes
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{

}