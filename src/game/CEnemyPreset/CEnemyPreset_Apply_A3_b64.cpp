// CEnemyPreset's unit (tools/tu_map.py): applies the preset to `actor` when it has a live entity state
// (+0x1b8, kind +0x70 not 2) and an AI component (+0x240): when the preset's flag +0x30 is set, the
// component's pooled node list (+0x1178, next at +0x18) is given back to its allocator (+0x1190) and its
// counters cleared; then, when `apply`, the component's vf_0x210 takes the preset. Names are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkPresetNode_A3_b64 {
    u8 pad_0x0[0x18];
    UnkPresetNode_A3_b64* next;
};

struct UnkEnemyPreset_A3_b64;

struct UnkPresetAI_A3_b64 {
    void** vtable;
    u8 pad_0x8[0x1170 - 0x8];
    u32 field_0x1170;
    u8 pad_0x1174[4];
    UnkPresetNode_A3_b64* field_0x1178;
    u64 field_0x1180;
    u8 pad_0x1188[0x8];
    base::UnkAllocator* field_0x1190;

    void vf_0x210(UnkEnemyPreset_A3_b64* preset) {
        reinterpret_cast<void (*)(UnkPresetAI_A3_b64*, UnkEnemyPreset_A3_b64*)>(vtable[0x210 / 8])(this, preset);
    }
};

struct UnkPresetState_A3_b64 {
    u8 pad_0x0[0x70];
    u32 kind;
};

struct UnkPresetActor_A3_b64 {
    u8 pad_0x0[0x1b8];
    UnkPresetState_A3_b64* field_0x1b8;
    u8 pad_0x1c0[0x240 - 0x1c0];
    UnkPresetAI_A3_b64* ai;
};

struct UnkEnemyPreset_A3_b64 {
    u8 pad_0x0[0x30];
    bool field_0x30;
};

void fn_71005769d8(UnkEnemyPreset_A3_b64* self, UnkPresetActor_A3_b64* actor, bool apply) {
    if (!actor || !actor->field_0x1b8 || actor->field_0x1b8->kind == 2)
        return;
    UnkPresetAI_A3_b64* ai = actor->ai;
    if (!ai)
        return;
    if (self->field_0x30) {
        for (UnkPresetNode_A3_b64* node = ai->field_0x1178; node;) {
            UnkPresetNode_A3_b64* next = node->next;
            ai->field_0x1190->vf_0x48(node);
            node = next;
        }
        ai->field_0x1170 = 0;
        ai->field_0x1178 = nullptr;
        ai->field_0x1180 = 0;
    }
    if (apply)
        ai->vf_0x210(self);
}
