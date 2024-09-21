module;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-enum-compare-conditional"
#include "raygui.h"
#pragma clang diagnostic pop
export module raygui;


export {
using ::GuiLoadStyle;
using ::GuiLoadStyleDefault;
using ::GuiButton;
using ::GuiMessageBox;
using ::GuiLabel;
using ::GuiLabelButton;
using ::GuiSetStyle;
using ::GuiSetFont;
using ::GuiGroupBox;
using ::GuiListView;
using ::GuiGetStyle;
using ::GuiGetState;
}
