
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
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(m_ColorIDs.size(), m_ColorIDs.data());
	glDeleteTextures(1, &m_DepthID);
}

void FrameBuffer::Update()
{
	if (m_ID)
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(m_ColorIDs.size(), m_ColorIDs.data());
		glDeleteTextures(1, &m_DepthID);

		m_ColorIDs.clear();
		m_DepthID = None;
	}

	glCreateFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	if (m_ColorAttatchments.size())
	{
		m_ColorIDs.resize(m_ColorAttatchments.size());
		glCreateTextures(GL_TEXTURE_2D, m_ColorIDs.size(), m_ColorIDs.data());

		for (size_t i = 0; i < m_ColorIDs.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorIDs[i]);
			switch (m_ColorAttatchments[i])
			{
			case RGBA8:
				AddColorAttatchment(m_ColorIDs[i], GL_RGBA8, GL_RGBA, i);
				break;
			}
		}
	}

	if (m_DepthAttatchment != None)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthID);
		glBindTexture(GL_TEXTURE_2D, m_DepthID);
		switch (m_DepthAttatchment)
		{
		case DEPTH:
			AddDepthAttatchment(m_DepthID, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
			break;
		}
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
	if(index < m_ColorIDs.size())
		return 0;
	return m_ColorIDs[index];
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
