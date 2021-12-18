#include <stdio.h>
#include <dlfcn.h>
#include "vgl.h"
#include "LoadShaders.h"

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

    glCreateBuffers(NumBuffers,Buffers);
    glNamedBufferStorage(Buffers[ArrayBuffer],sizeof(vertices),vertices,0);
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
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(640,480,"Triangles",NULL,NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();
    init();

        const GLubyte *name = glGetString (GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
        const GLubyte *biaoshifu = glGetString (GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
        const GLubyte *OpenGLVersion = glGetString (GL_VERSION); //返回当前OpenGL实现的版本号
        const GLubyte *gluVersion = gluGetString (GLU_VERSION); //返回当前GLU工具库版本
        printf ("OpenGL实现厂商的名字：%s\n", name);
        printf ("渲染器标识符：%s\n", biaoshifu);
        printf ("OpenGL实现的版本号：%s\n", OpenGLVersion);
        printf ("OGLU工具库版本：%s\n", gluVersion);

    while(!glfwWindowShouldClose(window))
    {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

}
