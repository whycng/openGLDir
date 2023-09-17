#include <iostream>
#include <string> 

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include <imgui/imgui_impl_opengl3.h>
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //if (glewInit() != GLEW_OK)/*glew�ĵ�������ᱨ����Ϊ��Ҫ�����ģ����������ں���*/
    //    std::cout << "ERROR!-1" << std::endl;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(5); // ��Ļˢ���ʣ�

    if (glewInit() != GLEW_OK)/*����Ͳ��ᱨ����*/
        std::cout << "ERROR!-2" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = { /*����ĵ㣬�����Ҫindex buffer*/
        -0.5f, -0.5f, 0.0f, 0.0f,// 0
        0.5f, -0.5f,1.0f, 0.0f,// 1
        0.5f, 0.5f, 1.0f, 1.0f,// 2
        -0.5f, 0.5f, 0.0f, 1.0f// 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    /*ʲô�������ţ�*/
    GLCall(GL_BLEND);
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
   

    /*vertex array object ���㻺��������*/
    //unsigned int vao;
    //GLCall(glGenVertexArrays(1, &vao));/* 1��ʾ����һ��vao,�ڶ��������ǽ��յ�ַ*/
    //GLCall(glBindVertexArray(vao));

    VertexArray va; 
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
   
    VertexBufferLayout layout; 
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout); 
    
    /* ibo - index buffer object ���������� ; ʹ�����������Ĳ���*/
    IndexBuffer ib(indices, 6);
    /* ����ͶӰ��x, y, z ; ��Ȼ������ӳ����-1��1֮�䣬����ʵ����Ҳ�������Ϊ�򶨷�Χ-3,3 */
    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    /* ��ͼ����ƽ�ƾ��������λ�� ; �������ƽ�ƣ�������������������ƽ��*/
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0));
    /*model matrix*/
   

    Shader shader("res/shaders/Basic.shader");
    shader.Bind(); 
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
    

    Texture texture("res/textures/ts_girl.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    va.UnBind();
    vb.Unbind();
    ib.Unbind(); 
    shader.UnBind();

    Renderer renderer;


     
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    const char* glsl_version = "#version 130";

    ImGui_ImplGlfw_InitForOpenGL(window, true); 
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();

    glm::vec3 translation(0, 0, 0);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    float r = 0.2f;
    float increment = 0.05f;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // glClearColor(1.0f, 1.0f, 1.0, 1.0f);��ɫ����Ϊ��ɫ
        /* Render here */
        renderer.Clear();
         
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();  
        ImGui::NewFrame();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);

        /*shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);*/
         
        renderer.Draw(va,ib,shader); 

        /*if (r >= 1.0f || r <= -1.0f)
        {
            increment = -increment;
        }
        r += increment;
        std::cout << "r:" << r << std::endl;*/
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
   /*     if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);*/
        // 2.
        {
        /*    static float f = 0.0f;
            static int counter = 0;*/

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            //ImGui::Checkbox("Another Window", &show_another_window);

            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //    counter++;
            //ImGui::SameLine();
            //ImGui::Text("counter = %d", counter);

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        // 3. Show another simple window.
        //if (show_another_window)
        //{
        //    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //    ImGui::Text("Hello from another window!");
        //    if (ImGui::Button("Close Me"))
        //        show_another_window = false;
        //    ImGui::End();
        //}

        ImGui::Render(); 
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } 
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}