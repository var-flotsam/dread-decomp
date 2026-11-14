// CXParasiteAIComponent's unit (tools/tu_map.py unit 1102): the component's destructor (vtable 0x7101a6a1c8).
// It drops the counted references at +0x147b0 to +0x14790, empties the keyed table of bindings (each
// binding's file path id first) and the array of entries, drops the reference at +0x146d0, then runs the
// base's destructor, fn_7100369380. Names are placeholders.
#include "game/CBehaviorTreeAIComponent.h"
#include "game/UnkXParasiteAI.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a6a1c8[];

// One counted reference less; the last one deletes the object (vtable slot 0x10).
static inline void Release(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

void fn_71006e6184(UnkXParasiteAI* self) {
    self->vtable = &g_unk_7101a6a1c8[2];
    Release(self->field_0x147b0);
    Release(self->field_0x147a0);
    Release(self->field_0x14798);
    Release(self->link);

    UnkXParasiteBindings& bindings = self->bindings;
    if (bindings.values)
        for (u64 i = 0; i < bindings.count; ++i)
            bindings.values[i].effect.~UnkStrIdMemberOf();
    bindings.count = 0;
    if (bindings.keys)
        bindings.allocator->vf_0x48(bindings.keys);
    if (bindings.values)
        bindings.allocator->vf_0x48(bindings.values);

    base::UnkVector<UnkWeakHandle*>& entries = self->entries;
    if (entries.data) {
        for (u64 i = 0; i < entries.count; ++i)
            Release(entries.data[i]);
        entries.count = 0;
        entries.allocator->vf_0x48(entries.data);
        entries.capacity = 0;
    }

    Release(self->field_0x146d0);
    fn_7100369380(self);
}
