module;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-enum-compare-conditional"
#include "raygui.h"
#pragma clang diagnostic pop
export module raygui;

export
{
  using ::GuiButton;
  using ::GuiDropdownBox;
  using ::GuiDummyRec;
  using ::GuiGetState;
  using ::GuiGetStyle;
  using ::GuiGroupBox;
  using ::GuiLabel;
  using ::GuiLabelButton;
  using ::GuiLine;
  using ::GuiListView;
  using ::GuiListViewEx;
  using ::GuiLoadStyle;
  using ::GuiLoadStyleDefault;
  using ::GuiMessageBox;
  using ::GuiSetFont;
  using ::GuiSetStyle;
  using ::GuiTabBar;
  using ::GuiWindowBox;
}
