// Unit 2636: the destructor of the 0x150-byte entry fn_7100fd10cc constructs (game/UnkEntry_0x150.h): its
// members release themselves, last to first: the two blocks of settings (each a string id and an array of
// path records), the two arrays, then the base's reflected value. Names and layouts are placeholders.
#include "game/UnkEntry_0x150.h"
#include "game/UnkTU2636.h"
#include "types.h"

// Clang 9, the matching compiler, ignores the asm label on the destructor and emits its mangled
// symbol, so this alias gives the definition the address's name; a newer Clang (the port's) names
// the definition by its label.
#if defined(__clang__) && __clang_major__ <= 9
void fn_7100fd11e8(UnkEntry_0x150*) __attribute__((alias("_ZN14UnkEntry_0x150D2Ev")));
#endif

UnkEntry_0x150::~UnkEntry_0x150() {
    vtable = g_unk_7101cac0c0 + 2;
}
