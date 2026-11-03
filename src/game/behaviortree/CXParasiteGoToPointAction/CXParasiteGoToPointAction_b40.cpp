// behaviortree::CXParasiteGoToPointAction unit (tools/tu_map.py unit 1095): the constructor
// (fn_71006e2404). The action node's base constructor, the vtable, two empty string ids at +0x30 and
// +0x38, cleared fields from +0x44 and an array at +0x78 (allocator flags 0xf). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

extern void* g_unk_7101a69e10[];

struct UnkXParasiteGoToPointAction : UnkBehaviorTreeNodeBase {
    u8 pad_0x2c[4];
    base::global::UnkStringData* field_0x30;
    base::global::UnkStringData* field_0x38;
    u8 pad_0x40[4];
    u8 field_0x44[0x2d];
    u8 pad_0x71[3];
    u32 field_0x74;
    base::UnkVector<void*> field_0x78;
};

static inline __attribute__((always_inline)) void InitEmpty_xp(base::global::UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_71006e2404(UnkXParasiteGoToPointAction* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a69e10 + 2;
    InitEmpty_xp(&self->field_0x30);
    InitEmpty_xp(&self->field_0x38);
    __builtin_memset(self->field_0x44, 0, sizeof(self->field_0x44));
    self->field_0x74 = 0;
    new (&self->field_0x78) base::UnkVector<void*>(0xf);
}
