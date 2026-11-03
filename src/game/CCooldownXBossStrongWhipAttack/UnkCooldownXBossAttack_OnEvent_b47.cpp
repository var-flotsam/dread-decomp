// Identical event handlers in the CCooldownXBossStrongWhipAttack and CCooldownXBossLaserBiteAttack units
// (tools/tu_map.py units 750 and 746): on an event of type 1 whose source id (+0x38) is the one the
// attack's lookup (+0x68, vtable slot 0x18) gives for its key (+0x8), copies the lookup's +0x100 to +0x70.
// The original lays the store out as the unlikely path (__builtin_expect). Names are placeholders.
#include "types.h"

struct UnkEventSource_b47 {
    u8 pad_0x0[0x38];
    u64 id;  // 0x38
};

struct UnkEventPayload_b47 {
    u8 pad_0x0[0x10];
    UnkEventSource_b47* source;  // 0x10
};

struct UnkEvent_b47 {
    u32 type;
    UnkEventPayload_b47* payload;  // 0x8
};

struct UnkAttackLookup_b47 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u64* vf_0x18(void* key);
    u8 pad_0x8[0xf8];
    u32 field_0x100;
};

struct UnkCooldownXBossAttack_b47 {
    void** vtable;
    void* field_0x8;
    u8 pad_0x10[0x58];
    UnkAttackLookup_b47* lookup;  // 0x68
    u32 field_0x70;
};

template <int>
static inline bool OnEvent_b47(UnkCooldownXBossAttack_b47* self, const UnkEvent_b47* event) {
    if (event->type == 1) {
        UnkEventSource_b47* source = event->payload->source;
        u64* id = self->lookup->vf_0x18(self->field_0x8);
        if (__builtin_expect(source->id == *id, 0)) {
            self->field_0x70 = self->lookup->field_0x100;
            return true;
        }
    }
    return true;
}

bool fn_71004cb0e8(UnkCooldownXBossAttack_b47* self, const UnkEvent_b47* event) { return OnEvent_b47<0>(self, event); }
bool fn_71004c31f8(UnkCooldownXBossAttack_b47* self, const UnkEvent_b47* event) { return OnEvent_b47<1>(self, event); }
