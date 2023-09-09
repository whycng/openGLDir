#pragma once

#include<gl/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();
/*注意 \ 后面直接打回车，不要打空格*/
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
 