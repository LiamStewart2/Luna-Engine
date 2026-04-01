#pragma once

#include "Luna.h"
#include "ImGuiPanel.h"

#include "../imgui/GraphEditor.h"
#include "GraphUtils.h"

struct ShaderGraphEditorDelegate : public GraphEditor::Delegate
{
    bool changesMade = false;
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
        switch (mLinks[linkIndex].mOutputSlotIndex)
        {
        }


        mLinks.erase(mLinks.begin() + linkIndex);

    }

    void CustomDraw(ImDrawList* drawList, ImRect rect, GraphEditor::NodeIndex index)
    {
        // Draw whatever visuals you want
        //if (index < textures.size() && textures[index])
        //    drawList->AddImage(textures[index]->GetTextureReference(), rect.Min, rect.Max);
        //else
        //    drawList->AddRect(rect.Min, rect.Max, IM_COL32(255, 0, 0, 255));

        // Move cursor into the node rect so ImGui can place widgets inside it
        ImGui::SetCursorScreenPos(rect.Min);

        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(rect.GetWidth(), rect.GetHeight())); // Drop zone
        ImGui::EndGroup();

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
            2,
            Array{"Input 1", "Input 2"},
            nullptr,
            2,
            Array{"Texture", "Output 2"},
            nullptr
        },

        {
            //Material Output
            IM_COL32(180, 160, 160, 255),
            IM_COL32(140, 100, 100, 255),
            IM_COL32(150, 110, 110, 255),
            5,
            Array{"Albedo", "Specular", "Normal", "Metallic", "AO"},
            Array{ IM_COL32(200,100,100,255), IM_COL32(100,200,100,255), IM_COL32(100,100,200,255), IM_COL32(200,100,100,255), IM_COL32(200,100,100,255) },
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
        unsigned int mNodeID;
    };

    void LoadNodes(Luna::ShaderGraph* shaderGraph)
    {
        // Load all nodes first, then can load the links
        const std::vector<Luna::ShaderGraphNode*>* nodes = shaderGraph->GetNodeList();

        mNodes.reserve(nodes->size());
        for (int i = 0; i < nodes->size(); i++)
            mNodes.push_back(Node("Fortnite", 0, nodes->at(i)->m_Position.x, nodes->at(i)->m_Position.y, false, nodes->at(i)->m_NodeID));

        // Load links
        const Luna::ShaderGraphNode* outputNode = shaderGraph->GetOutputNode();
        LoadLinks(outputNode);
    }


    // Iterates through all input connections and attempts to connect them
    void LoadLinks(const Luna::ShaderGraphNode* node)
    {
        GraphEditor::NodeIndex index = GetNodeIndexFromNodeID(node->m_NodeID);
        for (int i = 0; i < node->m_Inputs.size(); i++)
        {
            if(node->m_Inputs[i].m_Connection != nullptr)
            {
                LoadLinks(node->m_Inputs[i].m_Connection);

                GraphEditor::Link link = {0, 0, index, i};
                link.mInputNodeIndex = GetNodeIndexFromNodeID(node->m_Inputs[i].m_Connection->m_NodeID);
                link.mInputSlotIndex = node->m_Inputs[i].m_NodeIndex;

                mLinks.push_back(link);
            }
        }
    }

    GraphEditor::NodeIndex GetNodeIndexFromNodeID(unsigned int ID)
    {
        for (int i = 0; i < mNodes.size(); i++)
        {
            if (mNodes[i].mNodeID = ID)
                return i;
        }
        return 0;
    }

    std::vector<Node> mNodes;
    std::vector<GraphEditor::Link> mLinks;
};

class ShaderGraphEditorPanel : public ImGuiPanel
{
public:
    ShaderGraphEditorPanel(SceneManager* sceneManager = nullptr);
    ~ShaderGraphEditorPanel() {}

    void Update(unsigned int& inspectorID) override;

private:
	Luna::ShaderGraph* m_CurrentShaderGraph;

    GraphEditor::Options options;
    ShaderGraphEditorDelegate delegate;
    GraphEditor::ViewState viewState;
    GraphEditor::FitOnScreen fit;
};

