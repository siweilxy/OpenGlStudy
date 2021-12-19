#include <stdio.h>
#include <dlfcn.h>
#include "vgl.h"
#include "LoadShaders.h"
#include "drawcommands.hpp"
enum VAO_IDs
{
    Triangles, NumVAOs
};
enum Buffer_IDs
{
    ArrayBuffer, NumBuffers
};
enum Attrib_IDs
{
    vPosition = 0
};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

void init ()
{
    
    static const GLfloat vertices[NumVertices][2] = {
            { -0.90, -0.90 },
            { 0.85,-0.90 },
            { -0.90, 0.85 },
            { 0.90, -0.85 },
            { 0.90, 0.90 },
            { -0.85,0.90 }
    };

    //glCreateVertexArrays(NumVAOs,VAOs);

    glCreateBuffers(NumBuffers,Buffers);//创建NumBuffers个缓存到Buffers中
    glNamedBufferStorage(Buffers[ArrayBuffer],sizeof(vertices),vertices,0);//分配缓存对象的空间并把定点数据从对象传输到缓存对象当中
    ShaderInfo shaders[]={
            {GL_VERTEX_SHADER,"triangles.vert"},
            {GL_FRAGMENT_SHADER,"triangles.frag"},
            {GL_NONE,NULL}
    };

    GLuint program = LoadShaders(shaders);
    glUseProgram(program);

    glGenVertexArrays(NumVAOs,VAOs);
    glBindVertexArray(VAOs[Triangles]);
    glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
    glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
}

void display()
{
    static const float black[]={0.0f,0.0f,0.0f,0.0f};
    glClearBufferfv (GL_COLOR,0,black);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES,0,NumVertices);
}

int main (int argc, char *argv[])
{
    VermilionApplication *app  = DrawCommandExample::Create();
    app->Initialize("1");                                 
    app->MainLoop();                                        
    app->Finalize(); 

    return 0;

}
