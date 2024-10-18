module;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-enum-compare-conditional"
#include "raygui.h"
#pragma clang diagnostic pop
export module raygui;

export
{
  using ::GuiButton;
  using ::GuiGetState;
  using ::GuiGetStyle;
  using ::GuiGroupBox;
  using ::GuiLabel;
  using ::GuiLabelButton;
  using ::GuiListView;
  using ::GuiLoadStyle;
  using ::GuiLoadStyleDefault;
  using ::GuiMessageBox;
  using ::GuiSetFont;
  using ::GuiSetStyle;
}
