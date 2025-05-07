#ifndef XB_GRAPHICS_CONTEXT_H
#define XB_GRAPHICS_CONTEXT_H

namespace XBai
{
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}

#endif
