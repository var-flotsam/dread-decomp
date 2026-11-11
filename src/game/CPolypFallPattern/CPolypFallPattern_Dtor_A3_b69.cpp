// CPolypFallPattern's unit (tools/tu_map.py unit 871): a destructor (vtable 0x7101a50368) freeing three
// arrays of owned objects (game_s2/UnkOwnedList.h's UnkOwnedPtr) at +0x150, +0x128 and +0x100, then the
// base's destructor fn_71005acee8. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/CPolypFallPattern.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

void fn_71005acee8(char* self);  // the base's destructor

struct UnkPolypFallDtor_A3_b69;

template <>
struct base::UnkDtorTraits<UnkPolypFallDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a50368); }
    static void base(char* self) { fn_71005acee8(self); }
};

template <u32 Offset>
struct UnkPolypOwned_A3_b69 {
    static void Release(char* self) { reinterpret_cast<base::UnkVector<UnkOwnedPtr>*>(self + Offset)->Free(); }
};

__attribute__((flatten)) void fn_71005af7e4(char* self) {
    base::refl_Dtor<UnkPolypFallDtor_A3_b69, UnkPolypOwned_A3_b69<0x150>, UnkPolypOwned_A3_b69<0x128>,
                    UnkPolypOwned_A3_b69<0x100>>(self);
}
