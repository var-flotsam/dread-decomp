// SCameraRailNode's unit: builds a temporary argument object (a position and an empty callback set),
// hands it to the insert function, sets the owner's dirty flag, and runs a follow-up on the inserted
// target. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/SCameraRailNode.h"
#include "game/UnkCallbackSetVector.h"
#include "game/UnkTU341.h"
#include "types.h"

void* fn_7100c03a2c(void* target, u32 value);


struct UnkPosArg_lc038b8 {
    void* vtable;
    f32 x, y, z;
    void* set_vtable;
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;
    UnkRefCounted* owner;

    UnkPosArg_lc038b8(const f32* pos)
        : vtable(g_unk_7101aa2888 + 0x10), x(pos[0]), y(pos[1]), z(pos[2]) {
        field_0x30 = 0;
        set_vtable = g_unk_7101a20950 + 2;
        owner = nullptr;
    }
    ~UnkPosArg_lc038b8() {
        set_vtable = g_unk_7101a20950 + 2;
        vtable = g_unk_7101aa2888 + 0x10;
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
        set_vtable = g_unk_7101a20998 + 2;
    }
};

struct UnkRailOwner_lc038b8 {
    u8 pad_0x0[0x8];
    u8 target[0x28];
    bool field_0x30;
};


void* fn_7100c038b8(UnkRailOwner_lc038b8* self, u32 value, const f32* pos) {
    UnkPosArg_lc038b8 arg(pos);
    fn_7100c0c10c(reinterpret_cast<UnkInsertTarget*>(self->target), reinterpret_cast<const UnkArgObj*>(&arg), value);
    self->field_0x30 = true;
    return fn_7100c03a2c(self->target, value);
}
