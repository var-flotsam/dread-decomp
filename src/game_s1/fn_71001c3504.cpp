// Sets the string id at 0x150 of an object's lazily created part (at 0x78, made on first use by
// fn_71001c35b4 from the global at g_unk_716e34c2e0 with kind 0x3b). Called by fn_7100f01838 and
// others.

#include "base/UnkDestructors.h"
#include "types.h"

extern void* g_unk_716e34c2e0;

struct UnkPart_71001c3504 {
    u8 pad_0x0[0x150];
    base::UnkStrIdMember id;
};

UnkPart_71001c3504* fn_71001c35b4(void* source, void* arg, u32 kind);

struct UnkObject_71001c3504 {
    u8 pad_0x0[0x78];
    UnkPart_71001c3504* part;
};

void fn_71001c3504(UnkObject_71001c3504* self, const base::UnkStrIdMember* id) {
    UnkPart_71001c3504* part = self->part;
    if (!part) {
        part = fn_71001c35b4(g_unk_716e34c2e0, nullptr, 0x3b);
        self->part = part;
    }
    part->id = *id;
}
