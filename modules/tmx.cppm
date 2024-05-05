export module tmx;
export
{

  extern "C" {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#include <tmx.h>
#pragma clang diagnostic pop
  }
  constexpr auto tmx_flip_bits_removal = TMX_FLIP_BITS_REMOVAL;
}
