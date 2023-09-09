#include <iostream>
#include <string> 

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    /*�������һ�������⡣���﷨�Ƕ�������һ��,enum class Ϊʲô����Ϊ"���������ö������":

- ��ͨ�� enum ������:

  enum EnumName {
   value1, 
   value2
  }

- ö��ֵ����������,����ֱ��ʹ��ֵ��

- �� enum class ������:

  enum class EnumName {
   value1,
   value2
  } 

- ����ʹ����class�ؼ���

- ����C++��׼,class�ؼ��ֻ�Ϊö����������һ���µ�������

- ö��ֵ�����������µ���������

- ����Ҫʹ��ö��ֵ��,��Ҫ���������������::

 ��EnumName::value1

- �����͸��뿪�����������еĿ����ظ�����

- ����ֹö��ֵ�����������Ƴ�ͻ

����,��enum class�﷨��class�ؼ��ֲ���������������:

- ��Ϊö������ֵ��������һ�������������ռ�

- ��Ͳ�����"��������"������

ϣ������������԰������enum class���﷨����!*/
    enum class ShaderType { /* ���������ö�����ͣ�������*/
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) { /* �ҵ���*/
            if (line.find("vertex") != std::string::npos) {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}
 

/*�������,д��һ������*/
static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);/*vertex ���� fragment */
    const char* src = source.c_str(); /*����д &source[0]*/
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // char message[length]; /*����ᷢ����Ϊ���Ȳ������޷�ջ���䣬������Ҫ��ô��*/
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex":"fragment" )<< "shader!�붨λ������" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/*ʹ��static����Ϊ������й¶���������뵥Ԫ��
ʹ��string������õ�ѡ�񣬵�����԰�ȫ, int����-����ɫ��Ψһ��ʶ����һ��ID*/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    /*ʹ��unsigned����Ϊ�����ܵĲ�������������
    ���߿���ʹ�� GLuint���������߲�ϲ����������Ϊ��Ҫʹ�ö��ͼ��api*/
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
        -0.5f, -0.5f,// 0
        0.5f, -0.5f,// 1
        0.5f, 0.5f, // 2
        -0.5f, 0.5f, // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    /*vertex array object ���㻺��������*/
    //unsigned int vao;
    //GLCall(glGenVertexArrays(1, &vao));/* 1��ʾ����һ��vao,�ڶ��������ǽ��յ�ַ*/
    //GLCall(glBindVertexArray(vao));

    VertexArray va; 
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
   
    VertexBufferLayout layout; 
    layout.Push<float>(2);
    va.AddBuffer(vb, layout); 
    
    /* ibo - index buffer object ���������� ; ʹ�����������Ĳ���*/
    IndexBuffer ib(indices, 6);
  
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); 
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader)); 
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);/*����Ϊ�٣������ϵ㡣���û�����������ϵ�*/
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f)); 

    float r = 0.2f;
    float increment = 0.05f;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
 
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        if (r > 1.0f || r < -1.0f )
        {
            increment = -increment;
        } 
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}