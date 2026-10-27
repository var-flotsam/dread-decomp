// Constructor of an object derived from fn_7100393e68's: sets its vtable, clears its members, makes
// the two string ids at +0x40 and +0x48 the empty string (creating the pool first when there is
// none) and finishes in the constructor of the sub-object at +0x50, fn_71002acfe4.
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::UnkStringData;

typedef u64 u64_a4 __attribute__((aligned(4)));

struct UnkNode : UnkBehaviorTreeNodeBase {
    u64_a4 field_0x2c;
    u32 field_0x34;
    u8 pad_0x38[8];
    u64 field_0x40;
    UnkStringData* field_0x48;
    u8 field_0x50[1];
};

void fn_71002acfe4(void* sub);
extern void* g_unk_7101a2ad48[];

void fn_71003b3028(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a2ad48 + 2;
    self->field_0x2c = 0;
    self->field_0x34 = 0;
    self->field_0x40 = 0;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x40 = (u64)&g_unk_7101d47108->empty;
    self->field_0x48 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x48 = &g_unk_7101d47108->empty;
    fn_71002acfe4(self->field_0x50);
}
