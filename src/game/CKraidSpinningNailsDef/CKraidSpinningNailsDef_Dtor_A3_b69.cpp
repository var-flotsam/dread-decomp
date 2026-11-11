// CKraidSpinningNailsDef's unit (tools/tu_map.py unit 909): a destructor (vtable 0x7101a538e0) freeing two
// arrays of owned-object lists (game_s2/UnkOwnedList.h) at +0x118 and +0xf0, then the shared base's
// destructor fn_71005dbe04. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/CCharClassKraidAttack.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

extern void* g_unk_7101a538e0[];

struct UnkSpinningNailsDtor_A3_b69;

template <>
struct base::UnkDtorTraits<UnkSpinningNailsDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a538e0); }
    static void base(char* self) { fn_71005dbe04(self); }
};

template <u32 Offset>
struct UnkSpinningNailsLists_A3_b69 {
    static void Release(char* self) { reinterpret_cast<base::UnkVector<UnkOwnedList>*>(self + Offset)->Free(); }
};

__attribute__((flatten)) void fn_71005eab7c(char* self) {
    base::refl_Dtor<UnkSpinningNailsDtor_A3_b69, UnkSpinningNailsLists_A3_b69<0x118>,
                    UnkSpinningNailsLists_A3_b69<0xf0>>(self);
}
