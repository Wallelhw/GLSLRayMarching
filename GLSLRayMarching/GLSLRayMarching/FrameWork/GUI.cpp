//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "GUI.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

void GUI::Test(std::map<const char*, bool>& bValues,
	std::map<const char*, IValue>& iValues,
	std::map<const char*, FValue>& fValues,
	std::map<const char*, Vec4Value>& vec4Values)
{
	if (ImGui::Begin("SuperGameObject"))
	{
		for (auto& v : bValues)
		{
			ImGui::Checkbox(v.first, &v.second);
		}

		for (auto& v : iValues)
		{
			ImGui::SliderInt(v.first, &v.second.value, v.second.min, v.second.max);
		}

		for (auto& v : fValues)
		{
			ImGui::SliderFloat(v.first, &v.second.value, v.second.min, v.second.max);
		}

		for (auto& v : vec4Values)
		{
			float vvv[4] = { v.second.value[0], v.second.value[1], v.second.value[2], v.second.value[3] };

			ImGui::SliderFloat4(v.first, vvv, v.second.min, v.second.max);
		}
	}

	ImGui::End();
}

void GUI::Test2(int& lod, float& ratio, bool& wireframe, float& v)
{
	if (ImGui::Begin("SuperGameObject"))
	{
		if (ImGui::CollapsingHeader("Lod"))
		{
			ImGui::SliderInt("Lod", &lod, 0, 7, "%d");
		}
		if (ImGui::CollapsingHeader("Ratio"))
		{
			ImGui::SliderFloat("Ratio", &ratio, 0.0, 100.0, "%f");
		}
		if (ImGui::CollapsingHeader("WireFrame?"))
		{
			ImGui::Checkbox("WireFrame", &wireframe);
		}
		if (ImGui::CollapsingHeader("Shape and CSG"))
		{
			/*
			ImGui::Indent(10.0f);
			ImGui::Checkbox("Render Shape1", &enabled1);
			ImGui::ColorEdit4("Shape1 Color", &c[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
			ImGui::Checkbox("Shape1 Wire", &enabled2);
			ImGui::Checkbox("Shape1 Solid", &enabled3);
			ImGui::Unindent(10.0f);
			*/
		}

		if (ImGui::CollapsingHeader("vertexData[0].px[0]"))
		{
			ImGui::SliderFloat4("vertexData[0].px[0]", &v, -1, 1, "%f");
		}
	}

	ImGui::End();
}