module;
#include <imgui-SFML.h>
#include <imgui.h>
export module imgui;

export
{
  using ::ImVec2;
  using ::ImVec4;
  namespace ImGui {
    namespace SFML {
      using ImGui::SFML::Init;
      using ImGui::SFML::SetCurrentWindow;
      using ImGui::SFML::ProcessEvent;
      using ImGui::SFML::Update;
      using ImGui::SFML::Render;
      using ImGui::SFML::Shutdown;
      using ImGui::SFML::UpdateFontTexture;
      using ImGui::SFML::GetFontTexture;
      using ImGui::SFML::SetActiveJoystickId;
      using ImGui::SFML::SetJoystickDPadThreshold;
      using ImGui::SFML::SetJoystickLStickThreshold;
      using ImGui::SFML::SetJoystickRStickThreshold;
      using ImGui::SFML::SetJoystickLTriggerThreshold;
      using ImGui::SFML::SetJoystickRTriggerThreshold;
      using ImGui::SFML::SetJoystickMapping;
      using ImGui::SFML::SetDPadXAxis;
      using ImGui::SFML::SetDPadYAxis;
      using ImGui::SFML::SetLStickXAxis;
      using ImGui::SFML::SetLStickYAxis;
      using ImGui::SFML::SetRStickXAxis;
      using ImGui::SFML::SetRStickYAxis;
      using ImGui::SFML::SetLTriggerAxis;
      using ImGui::SFML::SetRTriggerAxis;
    }// namespace SFML
    using ImGui::Image;
    using ImGui::Text;
    using ImGui::Indent;
    using ImGui::InputFloat;
    using ImGui::Unindent;
    using ImGui::Begin;
    using ImGui::End;
    using ImGui::EndFrame;
    using ImGui::Button;
    using ImGui::BeginTabItem;
    using ImGui::EndTabItem;
    using ImGui::BeginTabBar;
    using ImGui::EndTabBar;
    using ImGui::ImageButton;
    using ImGui::DrawLine;
    using ImGui::DrawRect;
    using ImGui::DrawRectFilled;
    using ImGui::Dummy;
    using ImGui::SetNextWindowSize;
    using ImGui::SetNextWindowPos;
    using ImGui::GetIO;
    using ImGui::GetDrawData;
    using ImGui::SetCursorPosY;
    using ImGui::SetCursorPosX;
    using ImGui::GetWindowSize;
    using ImGui::CalcTextSize;
    using ImGui::Separator;
    using ImGui::PushFont;
    using ImGui::PopFont;
  }// namespace ImGui
}
