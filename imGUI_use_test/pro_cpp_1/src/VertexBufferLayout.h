#pragma once
#include <vector>
#include <GL/glew.h>
#include <stdexcept>
#include "Renderer.h"
/*����㲻����Ϊʹ��һС����glew�Ĺ��ܶ����� glew.h
���Խ��붨�壬�鿴���� GL_FLOAT�� ���壬������Ҫ�Ĳ��� define ճ������*/

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	// bool normalized;
	unsigned char normalized;
	
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1; 
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {};

	template<typename T>  
	void Push(unsigned int count)
	{/*
@lichform
1��ǰ
If you're on VS2022 and having an issue with the static_assert getting tripped in the 
unspecialized template, it's because VS2022 will trigger the assert when it's parsed,
rather than when it's instantiated. I changed it to a std::runtime_error which of course 
isn't checked at compile time but at least I get a verbose error when I try to create a 
VertexBufferLayout with an unsupported type.


@dr_nyt4041
1��ǰ
Thanks, that helped!
#include <stdexcept>

template<typename T>
void Push(unsigned int count) {
	std::runtime_error(false);
}*/
		// static_assert(false);/*��֪��Ϊʲô��������Ĵ�����ʾ ?*/ 
		std::runtime_error(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE }); 
		// m_Stride += 4; /* sizeof(float) */
		// m_Stride += sizeof(GLfloat);
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		// m_Stride += sizeof(GLuint);
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		// m_Stride += sizeof(GLbyte);
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	//inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	//inline unsigined int GetStride() const { return m_Stride; }

	inline const std::vector<VertexBufferElement>& GetElements() const {
		return m_Elements;
	}

	inline unsigned int GetStride() const {
		return m_Stride;
	}
};
