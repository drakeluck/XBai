#include "xbpch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>
#include <filesystem>

namespace XBai
{
	//TODO:未来需要改
	extern const std::filesystem::path g_Assetpath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:m_CurrentDirectory(g_Assetpath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(g_Assetpath))
		{
			if (ImGui::Button("<-", {30.0f, 30.0f}))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}
		ImGui::SameLine(40.0f);
		ImGui::Text(m_CurrentDirectory.string().c_str());

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
		{
			columnCount = 1;
		}
		ImGui::Columns(columnCount, 0, false);
		for (auto& directorEntity : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directorEntity.path();
			auto relativePath = std::filesystem::relative(path, g_Assetpath);
			//filename是文件/文件夹全称
			//stem是文件/文件夹名主体，即不包含后缀
			std::string filenameString = relativePath.filename().string();

			Ref<Texture2D> icon = directorEntity.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton(path.string().c_str(), (ImTextureID)icon->GetRendererID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directorEntity.is_directory())
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			ImGui::TextWrapped(filenameString.c_str());
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}
