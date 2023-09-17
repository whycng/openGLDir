#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	/* generate vertex array object names��ָ������1���洢���ֵ�m_RendererID */
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID)); 
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();/*�󶨵���ǰvao*/
	vb.Bind();/*��vbo���󶨶��㻺����*/
	const auto& elements = layout.GetElements(); /*���ò���*/
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));/* 0λ�ã� 1��ɫ*/
		/*���磺0-λ�ã�2-��ά��1-��ɫ��4-rgbx�ĸ�����ȷ��һ����ɫ
		stride-���ݲ�����pointer-ָ��ָ��ǰindex��Ӧ����ʼƫ����*/
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized , layout.GetStride(), (const void*)offset));/* (const void)*/
		// offset += element.count;
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind() const
{
	/*����array
Specifies the name of the vertex array to bind.*/
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));

}

