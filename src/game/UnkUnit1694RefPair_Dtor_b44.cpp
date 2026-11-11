// Unit 1694 of tools/tu_map.py (no class name known): the complete destructor of the object whose
// deleting destructor is fn_7100b4ee5c (src/clones/): its vtable, the references at 0x18 and 0x10
// released, then the base's vtable and its word at 0x8 cleared. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/GUI/CMainMenu.h"
#include "types.h"

extern char g_unk_7101a9cf08[];

struct UnkUnit1694RefPair_b44a3;
template <>
struct base::UnkDtorTraits<UnkUnit1694RefPair_b44a3> {
    static char* vtable() { return g_unk_7101a9cf08; }
    static void base(char* self) {
        *reinterpret_cast<char**>(self) = g_unk_7101a9c360 + 0x10;
        *reinterpret_cast<u64*>(self + 0x8) = 0;
    }
};

__attribute__((flatten)) void fn_7100b4edcc(char* self) {
    base::refl_Dtor<UnkUnit1694RefPair_b44a3, base::UnkRefPtrAt<0x18>, base::UnkRefPtrAt<0x10>>(self);
}
