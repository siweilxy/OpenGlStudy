#include "vgl.h"
#include <sys/time.h>
#include <memory.h>
#include <iostream>
class VermilionApplication
{
protected:
    inline VermilionApplication(void) {}
    virtual ~VermilionApplication(void) {}

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
        const GLubyte *name = glGetString (GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
        const GLubyte *biaoshifu = glGetString (GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
        const GLubyte *OpenGLVersion = glGetString (GL_VERSION); //返回当前OpenGL实现的版本号
        const GLubyte *gluVersion = gluGetString (GLU_VERSION); //返回当前GLU工具库版本
        printf ("OpenGL实现厂商的名字：%s\n", name);
        printf ("渲染器标识符：%s\n", biaoshifu);
        printf ("OpenGL实现的版本号：%s\n", OpenGLVersion);
        printf ("OGLU工具库版本：%s\n", gluVersion);
    }
};
