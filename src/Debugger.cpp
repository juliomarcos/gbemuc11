#include "Debugger.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include "imgui.h"

namespace debugger {
	
	bool showMemoryViewer;
	const auto ADDR_DIGITS_COUNT = 4;
	const auto ROWS = 32;
	const auto MEM_SIZE = 64 * 1024;
	
	char readPc[4];
	bool seekingPc;
	int pcTarget;
	
	int ramViewerWidth(int cellWidth) {
		return cellWidth * (4 + ROWS);
	}
	
	bool ShouldExecuteNextInstruction(gbemu::CPU &cpu) {
		bool ret;
		
		auto glyphWidth = ImGui::CalcTextSize("F").x;
		auto cellWidth = glyphWidth * 3;
		ImGui::SetNextWindowPos(ImVec2(ramViewerWidth(cellWidth),cellWidth * 12));
		ImGui::SetNextWindowSize(ImVec2(0, cellWidth * 6));
		if (ImGui::Begin("Stepper")) {
			ret = ImGui::Button("Step");
			ImGui::Separator();
			ImGui::InputText("PC==", readPc, 4);
			std::string pcTargetStr(readPc);
			auto ifRes = ImGui::Button("Stop When");
			if (!seekingPc && ifRes) {
				seekingPc = true;
				std::stringstream ss;
				ss << std::hex << pcTargetStr;
				ss >> pcTarget;
				ret = cpu._pc != pcTarget;
			}
		}
		ImGui::End();
		return ret;
	}
	
	void RegistersViewer(gbemu::CPU &cpu) {
		auto glyphWidth = ImGui::CalcTextSize("F").x;
		auto cellWidth = glyphWidth * 3;
		ImGui::SetNextWindowPos(ImVec2(ramViewerWidth(cellWidth),0));
		ImGui::SetNextWindowSize(ImVec2(0, cellWidth * 12));
		if (ImGui::Begin("Registers", &showMemoryViewer)) {
			ImGui::Text("PC %04X", cpu._pc);
			ImGui::Text("SP %04X", cpu._sp);
			ImGui::Separator();
			ImGui::Text("A  %02X", cpu.a);
			ImGui::Text("B  %02X", cpu.b);
			ImGui::Text("C  %02X", cpu.c);
			ImGui::Text("D  %02X", cpu.d);
			ImGui::Text("E  %02X", cpu.e);
			ImGui::Text("H  %02X", cpu.h);
			ImGui::Text("L  %02X", cpu.l);
			ImGui::Separator();
			ImGui::Text("F  %s", bitset<8>(cpu.f).to_string().c_str());
		}
		ImGui::End();
	}
	
	void MemoryViewer(gbemu::CPU &cpu) {
		auto glyphWidth = ImGui::CalcTextSize("F").x;
		auto cellWidth = glyphWidth * 3;
		auto lineHeight = ImGui::GetTextLineHeight();
		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::SetNextWindowSize(ImVec2(ramViewerWidth(cellWidth), lineHeight * 64));
		
		if (ImGui::Begin("RAM", &showMemoryViewer)) {
			ImGui::BeginChild("##scrolling", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
			
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
			
			auto totalLineCount = static_cast<int>((MEM_SIZE + ROWS - 1) / ROWS);
			ImGuiListClipper clipper(totalLineCount, lineHeight);
			int visibleStartAddr = clipper.DisplayStart * ROWS;
			int visibleEndAddr = clipper.DisplayEnd * ROWS;

			// Draw address
			for(size_t i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
				int addr = i * ROWS;
				ImGui::Text("%0*X: ", ADDR_DIGITS_COUNT, addr);
				ImGui::SameLine();
				
				// Draw memory value
				auto lineStartX = ImGui::GetCursorPosX();
				for (size_t n = 0; n < ROWS && addr < MEM_SIZE; n++, addr++) {
					ImGui::SameLine(lineStartX + cellWidth * n);
					ImGui::Text("%02X ", cpu.ram[addr]);
				}
				
			}
			
			clipper.End();
			
			ImGui::PopStyleVar(2);
			ImGui::EndChild();
		}
		ImGui::End();
		
	} 
		
}