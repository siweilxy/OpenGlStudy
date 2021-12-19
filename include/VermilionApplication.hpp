#ifndef __VAPP_HPP__
#define __VAPP_HPP__
#include "vgl.h"
#include <sys/time.h>
#include <memory.h>
#include <iostream>
#include <string>
class VermilionApplication
{
protected:
    inline VermilionApplication(void) {}
    virtual ~VermilionApplication(void) {}

    std::string name;
    std::string biaoshifu; //返回一个渲染器标识符，通常是个硬件平台
    std::string OpenGLVersion; //返回当前OpenGL实现的版本号
    std::string version; //返回当前GLU工具库版本
    
    char temp[255];

    static VermilionApplication * s_app;
    GLFWwindow* m_pWindow;

    struct timeval  m_appStartTime;

    static void window_size_callback(GLFWwindow* window, int width, int height)
    {
        VermilionApplication* pThis = (VermilionApplication*)glfwGetWindowUserPointer(window);
        pThis->Resize(width, height);
    }
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        VermilionApplication* pThis = (VermilionApplication*)glfwGetWindowUserPointer(window);
        pThis->OnKey(key, scancode, action, mods);
    }
    static void char_callback(GLFWwindow* window, unsigned int codepoint)
    {
        VermilionApplication* pThis = (VermilionApplication*)glfwGetWindowUserPointer(window);
        pThis->OnChar(codepoint);
    }
    unsigned int app_time()
    {
        return 0;
    }

public:
    void MainLoop(void)
    {
        do                                                      
        {                                                       
            Display();                                          
            glfwPollEvents();                                   
        } while (!glfwWindowShouldClose(m_pWindow));
    }

    virtual void Initialize(const char * title = 0)
    {
        gettimeofday(&m_appStartTime, nullptr);
        glfwInit();
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        m_pWindow = glfwCreateWindow(800, 600, title ? title : "OpenGL Application", nullptr, nullptr);
        glfwSetWindowUserPointer(m_pWindow, this);
        glfwSetWindowSizeCallback(m_pWindow, window_size_callback);
        glfwSetKeyCallback(m_pWindow, key_callback);
        glfwSetCharCallback(m_pWindow, char_callback);
        glfwMakeContextCurrent(m_pWindow);
        gl3wInit();
        Resize(800, 600);
        printVersion();
    }

    virtual void Display(bool auto_redraw = true)
    {
        glfwSwapBuffers(m_pWindow);
    }

    virtual void Finalize(void) {}

    virtual void Resize(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    virtual void OnKey(int key, int scancode, int action, int mods) 
    { 
        std::cout<<"key:"<<key<<" scancode:"<<scancode<<" action:"<<action<<" mods:"<<mods<<std::endl;
    }
    virtual void OnChar(unsigned int codepoint) 
    {
        std::cout<<"codepoint:"<<codepoint<<std::endl;
    }

    void printVersion()
    {
        const GLubyte *glName = glGetString (GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
        const GLubyte *glBiaoshifu = glGetString (GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
        const GLubyte *glOpenGLVersion = glGetString (GL_VERSION); //返回当前OpenGL实现的版本号
        const GLubyte *gluVersion = gluGetString (GLU_VERSION); //返回当前GLU工具库版本

        snprintf(temp,sizeof(temp),"%s",glName);
        name = temp;
        snprintf(temp,sizeof(temp),"%s",glBiaoshifu);
        biaoshifu = temp;
        snprintf(temp,sizeof(temp),"%s",glOpenGLVersion);
        OpenGLVersion = temp;
        snprintf(temp,sizeof(temp),"%s",gluVersion);
        version = temp;

        std::cout<<"OpenGL实现厂商的名字："<<name<<" 渲染器标识符："
        <<biaoshifu<<" OpenGL实现的版本号："<<OpenGLVersion
        <<" OGLU工具库版本："<<version<<std::endl;
    }
};
#endif //  __VAPP_HPP__
