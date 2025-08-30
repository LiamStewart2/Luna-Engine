
#include "Framebuffer.h"

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::FrameBuffer(const FramebufferSpecification& specification)
	: m_Specification(specification)
{
	for (FramebufferTextureAttatchment attatchment : m_Specification.Attatchments)
	{
		if(attatchment != DEPTH)
			m_ColorAttatchments.emplace_back(attatchment);
		else
			m_DepthAttatchment = attatchment;
	}
	Update();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(m_ColorIDs.size(), m_ColorIDs.data());
	glDeleteTextures(1, &m_DepthID);
}

#include <iostream>
void FrameBuffer::Update()
{
	if (m_ID)
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(m_ColorIDs.size(), m_ColorIDs.data());
		glDeleteTextures(1, &m_DepthID);
	}

	glCreateFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	m_ColorIDs.clear();
	m_DepthID = 0;

	for (unsigned int i = 0; i < m_ColorAttatchments.size(); i++)
	{
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Width,
			0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + i,
			GL_TEXTURE_2D, texID, 0);

		m_ColorIDs.push_back(texID);
	}

	if (m_DepthAttatchment != None)
	{
		glGenRenderbuffers(1, &m_DepthID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER, m_DepthID);
	}

	if (!m_ColorIDs.empty())
	{
		std::vector<GLenum> bufs(m_ColorIDs.size());
		for (size_t i = 0; i < bufs.size(); ++i)
			bufs[i] = GL_COLOR_ATTACHMENT0 + (GLenum)i;
		glDrawBuffers((GLsizei)bufs.size(), bufs.data());
	}
	else
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "[FrameBuffer] Incomplete framebuffer!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize(unsigned int width, unsigned int height)
{
	m_Specification.Width = width;
	m_Specification.Height = height;

	Update();
}

void FrameBuffer::ClearAttatchment(unsigned int attatchmentIndex, int value)
{
	glClearTexImage(m_ColorIDs[attatchmentIndex], 0, GL_RGBA8, GL_INT, &value);
}

unsigned int FrameBuffer::GetAttatchmentID(unsigned int index)
{
	if(index >= m_ColorIDs.size())
		return 0;
	return m_ColorIDs.at(index);
}

void FrameBuffer::AddColorAttatchment(unsigned int id, GLenum internalFormat, GLenum format, int index)
{
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Specification.Width, m_Specification.Height, 0, format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0);
}

void FrameBuffer::AddDepthAttatchment(unsigned int id, GLenum format, GLenum attatchmentType)
{
	glTexStorage2D(GL_TEXTURE_2D, 1, format, m_Specification.Width, m_Specification.Height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attatchmentType, GL_TEXTURE_2D, id, 0);
}
