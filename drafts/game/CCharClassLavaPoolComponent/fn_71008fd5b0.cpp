// CCharClassLavaPoolComponent's unit (tools/tu_map.py unit 1393): the constructor. On the char-class
// component base (fn_71011acd10): a flag set, the vtable and four empty file paths.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/CCharClassComponent.h"
#include "types.h"

using base::global::UnkStringData;

extern void* g_unk_7101a80a98[];

struct UnkCharClassBase_A5_b71 {
    void** vtable;
    u8 pad_0x8[0x58 - 0x8];
    bool field_0x58;
    u8 pad_0x59[7];
    UnkStringData* paths[4];  // 0x60
};
static_assert(__builtin_offsetof(UnkCharClassBase_A5_b71, paths) == 0x60, "layout");

static inline void MakeEmptyPath(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    *id = &g_unk_7101d47200->empty;
}

void fn_71008fd5b0(UnkCharClassBase_A5_b71* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->field_0x58 = true;
    self->vtable = g_unk_7101a80a98 + 2;
    for (UnkStringData*& path : self->paths)
        MakeEmptyPath(&path);
}
