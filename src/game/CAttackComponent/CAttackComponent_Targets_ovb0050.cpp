// CAttackComponent unit (tools/tu_map.py unit 2706): three pointer arrays and a walk counter (+0x78)
// that defer changes while the arrays are being walked. fn_7101138188 removes an entry: while the
// counter is positive it records it in the array at +0x28 and takes it out of the one at +0x0;
// otherwise it takes it out of the array at +0x50. fn_710113831c ends a walk (the holder keeps a
// pointer to the arrays): once the counter is back to zero, the entries recorded at +0x28 leave the
// array at +0x50 and those at +0x0 join it, and both are emptied. Elements are read through the
// array's out-of-line accessor fn_71011384e0. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkAttackTargets_ovb0050 {
    base::UnkVector<void*> field_0x0;
    base::UnkVector<void*> field_0x28;
    base::UnkVector<void*> field_0x50;
    // A union member: the original reloads the holder's pointer and the counter after the
    // decrement (no type-based alias info).
    union {
        s32 lock;  // 0x78
    };
};
static_assert(__builtin_offsetof(UnkAttackTargets_ovb0050, lock) == 0x78, "layout");

// base::UnkVector<void*>::GrowAt, out of line.
void** fn_71011384e0(base::UnkVector<void*>* self, u32 index);

struct UnkAttackTargetsLock_ovb0050 {
    UnkAttackTargets_ovb0050* targets;
};

void fn_7101138188(UnkAttackTargets_ovb0050* self, void* target) {
    if (self->lock <= 0) {
        self->field_0x50.Remove(target);
        return;
    }
    self->field_0x28.PushBack(target);
    self->field_0x0.Remove(target);
}

void fn_710113831c(UnkAttackTargetsLock_ovb0050* self) {
    --self->targets->lock;
    UnkAttackTargets_ovb0050* targets = self->targets;
    if (targets->lock > 0)
        return;
    for (u32 i = 0; i < targets->field_0x28.count; ++i)
        targets->field_0x50.Remove(*fn_71011384e0(&targets->field_0x28, i));
    // Cleared through an untyped store: the original reloads the holder's pointer after it.
    __builtin_memset(&targets->field_0x28.count, 0, sizeof(u32));
    targets = self->targets;
    for (u32 i = 0; i < targets->field_0x0.count; ++i)
        targets->field_0x50.PushBack(*fn_71011384e0(&targets->field_0x0, i));
    targets->field_0x0.count = 0;
}
