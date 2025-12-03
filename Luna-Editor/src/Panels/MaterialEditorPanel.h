#pragma once

#include "ImGuiPanel.h"
#include "../imgui/GraphEditor.h"

#include "Luna.h"

#include <string>
#include <memory>
#include <vector>

template <typename T, std::size_t N>
struct Array
{
    T data[N];
    const size_t size() const { return N; }

    const T operator [] (size_t index) const { return data[index]; }
    operator T* () {
        T* p = new T[N];
        memcpy(p, data, sizeof(data));
        return p;
    }
};

template <typename T, typename ... U> Array(T, U...) -> Array<T, 1 + sizeof...(U)>;

struct GraphEditorDelegate : public GraphEditor::Delegate
{
    std::vector<std::shared_ptr<Luna::ITexture>> textures;
    bool changesMade = false;
    std::shared_ptr<Luna::Material> material;
    Luna::AssetManager* assetManager = nullptr;

    bool AllowedLink(GraphEditor::NodeIndex from, GraphEditor::NodeIndex to) override
    {
        return true;
    }

    void SelectNode(GraphEditor::NodeIndex nodeIndex, bool selected) override
    {
        mNodes[nodeIndex].mSelected = selected;
    }

    void MoveSelectedNodes(const ImVec2 delta) override
    {
        for (auto& node : mNodes)
        {
            if (!node.mSelected)
            {
                continue;
            }
            node.x += delta.x;
            node.y += delta.y;
        }
    }

    virtual void RightClick(GraphEditor::NodeIndex nodeIndex, GraphEditor::SlotIndex slotIndexInput, GraphEditor::SlotIndex slotIndexOutput) override
    {
    }

    void AddLink(GraphEditor::NodeIndex inputNodeIndex, GraphEditor::SlotIndex inputSlotIndex, GraphEditor::NodeIndex outputNodeIndex, GraphEditor::SlotIndex outputSlotIndex) override
    {
        mLinks.push_back({ inputNodeIndex, inputSlotIndex, outputNodeIndex, outputSlotIndex });
    }

    void DelLink(GraphEditor::LinkIndex linkIndex) override
    {
        mLinks.erase(mLinks.begin() + linkIndex);
    }

    void CustomDraw(ImDrawList* drawList, ImRect rect, GraphEditor::NodeIndex index)
    {
        // Draw whatever visuals you want
        if (index < textures.size() && textures[index])
            drawList->AddImage(textures[index]->GetTextureReference(), rect.Min, rect.Max);
        else
            drawList->AddRect(rect.Min, rect.Max, IM_COL32(255, 0, 0, 255));

        // Move cursor into the node rect so ImGui can place widgets inside it
        ImGui::SetCursorScreenPos(rect.Min);

        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(rect.GetWidth(), rect.GetHeight())); // Drop zone
        ImGui::EndGroup();

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM_TEXTURE"))
            {
                const char* texturePath = (const char*)payload->Data;

                auto tex = assetManager->GetTexture(texturePath);

                if (index < textures.size())
                    textures[index] = tex;

                switch (index)
                {
                case(0):
                    assetManager->GetMaterial(material->m_Path)->m_Albedo = tex;
                    break;
                case(1):
                    assetManager->GetMaterial(material->m_Path)->m_SpecularMap = tex;
                    break;
                case(2):
                    assetManager->GetMaterial(material->m_Path)->m_NormalMap = tex;
                    break;
                default:
                    break;
                }

                changesMade = true;
            }
            ImGui::EndDragDropTarget();
        }
    }

    const size_t GetTemplateCount() override
    {
        return sizeof(mTemplates) / sizeof(GraphEditor::Template);
    }

    const GraphEditor::Template GetTemplate(GraphEditor::TemplateIndex index) override
    {
        return mTemplates[index];
    }

    const size_t GetNodeCount() override
    {
        return mNodes.size();
    }

    const GraphEditor::Node GetNode(GraphEditor::NodeIndex index) override
    {
        const auto& myNode = mNodes[index];
        return GraphEditor::Node
        {
            myNode.name,
            myNode.templateIndex,
            ImRect(ImVec2(myNode.x, myNode.y), ImVec2(myNode.x + 200, myNode.y + 200)),
            myNode.mSelected
        };
    }

    const size_t GetLinkCount() override
    {
        return mLinks.size();
    }

    const GraphEditor::Link GetLink(GraphEditor::LinkIndex index) override
    {
        return mLinks[index];
    }

    // Graph datas
    static const inline GraphEditor::Template mTemplates[] = {
        {
            //Texture Input
            IM_COL32(160, 160, 180, 255),
            IM_COL32(100, 100, 140, 255),
            IM_COL32(110, 110, 150, 255),
            0,
            nullptr,
            nullptr,
            1,
            Array{"Texture"},
            nullptr
        },

        {
            //Material Output
            IM_COL32(180, 160, 160, 255),
            IM_COL32(140, 100, 100, 255),
            IM_COL32(150, 110, 110, 255),
            3,
            Array{"Albedo", "Specular", "Normal"},
            Array{ IM_COL32(200,100,100,255), IM_COL32(100,200,100,255), IM_COL32(100,100,200,255) },
            0,
            nullptr,
            nullptr
        }
    };

    struct Node
    {
        const char* name;
        GraphEditor::TemplateIndex templateIndex;
        float x, y;
        bool mSelected;
    };

    std::vector<Node> mNodes = {
        {
            "Texture",
            0,
            0, 0,
            false
        },

        {
            "Texture",
            0,
            0, 250,
            false
        },

        {
            "Texture",
            0,
            0, 500,
            false
        },

        {
            "Output",
            1,
            450, 250,
            false
        }
    };

    std::vector<GraphEditor::Link> mLinks = { {0, 0, 3, 0}, {1, 0, 3, 1}, {2, 0, 3, 2} };
};

class MaterialEditorPanel : public ImGuiPanel
{
public:
	MaterialEditorPanel(SceneManager* sceneManager = nullptr) : ImGuiPanel(sceneManager)
	{
		if(sceneManager == nullptr) return;
		fit = GraphEditor::Fit_None; delegate.assetManager = sceneManager->GetAssetManager();
	}

	void Update(unsigned int& inspectorID) override;
    void SaveCurrentMaterial();
private:
	std::shared_ptr<Luna::Material> m_Material = nullptr;

	GraphEditor::Options options;
	GraphEditorDelegate delegate;
	GraphEditor::ViewState viewState;
	GraphEditor::FitOnScreen fit;
};

