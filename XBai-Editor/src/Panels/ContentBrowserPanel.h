#ifndef XB_EDITOR_PANELS_CONTENTBROWSERPANEL_H
#define XB_EDITOR_PANELS_CONTENTBROWSERPANEL_H

#include "XBai/Render/Texture.h"
#include <filesystem>

namespace XBai
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}

#endif

