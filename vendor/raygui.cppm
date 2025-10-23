module;
#include <raylib.h>
export module raygui;

export {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-enum-compare-conditional"
#include "raygui.h"
#pragma clang diagnostic pop
  using enum ::GuiControl;
  using enum ::GuiControlProperty;
  using enum ::GuiDefaultProperty;
}
