#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	/* generate vertex array object names，指定个数1，存储名字到m_RendererID */
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID)); 
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();/*绑定到当前vao*/
	vb.Bind();/*绑定vbo。绑定顶点缓冲区*/
	const auto& elements = layout.GetElements(); /*设置布局*/
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));/* 0位置， 1颜色*/
		/*例如：0-位置，2-二维；1-颜色，4-rgbx四个属性确定一个颜色
		stride-数据步长，pointer-指针指向当前index对应的起始偏移量*/
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized , layout.GetStride(), (const void*)offset));/* (const void)*/
		// offset += element.count;
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind() const
{
	/*参数array
Specifies the name of the vertex array to bind.*/
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));

}

