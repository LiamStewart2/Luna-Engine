#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum FramebufferTextureAttatchment
{
	None = 0,

	RGBA8 = 1,

	DEPTH = 2
};

struct FramebufferSpecification
{
	unsigned int Width, Height;
	std::vector<FramebufferTextureAttatchment> Attatchments;
};

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(const FramebufferSpecification& specification);
	~FrameBuffer();

	void Update();

	void Bind();
	void Unbind();

	void Resize(unsigned int width, unsigned int height);

	void ClearAttatchment(unsigned int attatchmentIndex, int value);

	unsigned int GetAttatchmentID(unsigned int index = 0);

private:
	unsigned int m_ID = 0;
	FramebufferSpecification m_Specification;

	void AddColorAttatchment(unsigned int id, GLenum internalFormat, GLenum format, int index);
	void AddDepthAttatchment(unsigned int id, GLenum format, GLenum attatchmentType);

	std::vector<FramebufferTextureAttatchment> m_ColorAttatchments;
	FramebufferTextureAttatchment m_DepthAttatchment = FramebufferTextureAttatchment::None;

	std::vector<unsigned int> m_ColorIDs;
	unsigned int m_DepthID = 0;
};

