#ifndef __DRAWCOMMANDS_HPP
#define __DRAWCOMMANDS_HPP

//#include "vapp.h"
#include "vutils.h"
#include "vmath.h"
#include "LoadShaders.h"
#include <stdio.h>
#include "vgl.h"
#include "VermilionApplication.hpp"
using namespace vmath;
// VermilionApplication * VermilionApplication::s_app;
class DrawCommandExample : public VermilionApplication
{
public:
    // Member variables
    float aspect;
    GLuint render_prog;
    GLuint vao[1];
    GLuint vbo[1];
    GLuint ebo[1];

    GLint render_model_matrix_loc;
    GLint render_projection_matrix_loc;

    static VermilionApplication *Create(void)
    {
        VermilionApplication::s_app = new DrawCommandExample;
        return VermilionApplication::s_app;
    }

    virtual void Initialize(const char *title)
    {
        VermilionApplication::Initialize(title);
        ShaderInfo shader_info[] =
            {// GL_VERTEX_SHADER 顶点着色器 GL_FRAGMENT_SHADER 片段着色器、像素着色器
             {GL_VERTEX_SHADER, "/home/siwei/work/github/OpenGlStudy/media/shaders/primitive_restart/primitive_restart.vs.glsl"},
             {GL_FRAGMENT_SHADER, "/home/siwei/work/github/OpenGlStudy/media/shaders/primitive_restart/primitive_restart.fs.glsl"},
             {GL_NONE, NULL}};

        render_prog = LoadShaders(shader_info);

        glUseProgram(render_prog);

        // "model_matrix" is actually an array of 4 matrices 在glsl中，这个是从glsl中获取变量地址 顶点着色器
        render_model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");           //模型矩阵
        render_projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix"); //投影矩阵

        std::cout << "render_model_matrix_loc:" << render_model_matrix_loc << std::endl;
        std::cout << "render_projection_matrix_loc:" << render_projection_matrix_loc << std::endl;

        // A single triangle 4个顶点
        static const GLfloat vertex_positions[] =
            {
                -1.0f,
                -1.0f,
                0.0f,
                1.0f,
                1.0f,
                -1.0f,
                0.0f,
                1.0f,
                -1.0f,
                1.0f,
                0.0f,
                1.0f,
                -1.0f,
                -1.0f,
                0.0f,
                1.0f,
            };

        // Color for each vertex 每个顶点的颜色 决定了三角形的颜色
        static const GLfloat vertex_colors[] =
            {
                1.0f,
                1.0f,
                1.0f,
                1.0f,
                1.0f,
                1.0f,
                0.0f,
                1.0f,
                1.0f,
                0.0f,
                1.0f,
                1.0f,
                0.0f,
                1.0f,
                1.0f,
                1.0f,
            };

        // Indices for the triangle strips 三个索引值
        static const GLushort vertex_indices[] =
            {
                0, 1, 2};

        // Set up the element array buffer
        glGenBuffers(1, ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

        // Set up the vertex attributes
        glGenVertexArrays(1, vao);
        glBindVertexArray(vao[0]);

        glGenBuffers(1, vbo);
        // GL_ARRAY_BUFFER P76 用来保存glVertexAttribPointer 设置的顶点数组数据
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        //内存分配，必须要有
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
        //内存分别初始化，把上面的2个表示颜色和顶点位置的数组传入gpu
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);
        // p87，设置顶点属性在index(0、1)位置可以访问的数据值.
        // 0 ,1在glsl中 layout (location = 0) in vec4 position;layout (location = 1) in vec4 color;
        // NULL表示无偏移，第二个的sizeof(vertex_positions)表示偏移到这里开始
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertex_positions));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }

    virtual void Display(bool auto_redraw)
    {
        // float t = float(app_time() & 0x1FFF) / float(0x1FFF);
        // static float q = 0.0f;
        // static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
        // static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
        // static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
        //黑色
        static const vmath::vec4 black = vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        mat4 model_matrix;

        // Setup
        glEnable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        glClearBufferfv(GL_COLOR, 0, black); //设置背景色

        // Activate simple shading program
        //之前初始化时已经使用过，不加也可以，不过还是加了，加了应该没影响
        glUseProgram(render_prog);

        // Set up the model and projection matrix 投影矩阵
        vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 500.0f));
        glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

        // Set up for a glDrawElements call
        glBindVertexArray(vao[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

        // Draw Arrays... 1 x,y,z
        model_matrix = vmath::translate(-3.0f, 0.0f, -5.0f);
        glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //     // DrawElements 2
        model_matrix = vmath::translate(-1.0f, 0.0f, -5.0f);
        glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);

        // DrawElementsBaseVertex 3
        model_matrix = vmath::translate(1.0f, 0.0f, -5.0f);
        glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
        glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL, 1);

        // DrawArraysInstanced 4
        model_matrix = vmath::translate(3.0f, 0.0f, -5.0f);
        glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);

        VermilionApplication::Display();
    }

    virtual void Finalize(void)
    {
        glUseProgram(0);
        glDeleteProgram(render_prog);
        glDeleteVertexArrays(1, vao);
        glDeleteBuffers(1, vbo);
    }

    virtual void Resize(int width, int height)
    {
        glViewport(0, 0, width, height);
        aspect = float(height) / float(width);
    }
};
#endif