// CCharClassWaterPoolComponent (unit 1396), the counterpart of fn_7100909cdc
// (CCharClassWaterPoolComponent_Reset_b51.cpp): when the two words at +0xcf8/+0xcfc differ, sets one of the
// pool's two timeline ids (by `second`) to the all-flags mask, resolves every entry's lazily resolved
// handle and passes what each names to fn_71007e6778, then raises +0xd48. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkRefHandle.h"
#include "game/CCharClassSteamJetComponent.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkCountedWater_A5_b63 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkRef {
    UnkCountedWater_A5_b63* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkWaterTarget;
void fn_71007e6778(UnkWaterTarget* target, f32 value);

extern u64 g_unk_710164b1e8;
extern u64 g_unk_710164b1f0;

struct UnkWaterEntry_A5_b63 {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;  // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38
    u8 pad_0x40[0x8];
};

struct UnkWaterOwner_A5_b63 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;
};

struct UnkWaterPool_A5_b63 {
    u8 pad_0x0[0x30];
    UnkWaterOwner_A5_b63* owner;
    u8 pad_0x38[0xc90 - 0x38];
    UnkWaterEntry_A5_b63* entries;  // 0xc90
    u32 count;                      // 0xc98
    u8 pad_0xc9c[0xcf8 - 0xc9c];
    s32 field_0xcf8;
    s32 field_0xcfc;
    u8 pad_0xd00[0xd48 - 0xd00];
    bool field_0xd48;
};

// Takes the handle's value from the source (state 1 -> 2).
__attribute__((always_inline)) static inline void Resolve_A5_b63(UnkWaterEntry_A5_b63* entry) {
    UnkRef fresh = fn_7100353be8(entry->source);
    fn_710031d208(&entry->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
}

void fn_7100909f84(UnkWaterPool_A5_b63* self, bool second) {
    if (self->field_0xcfc == self->field_0xcf8)
        return;
    if (self->owner) {
        if (UnkTimelineComponent* timeline = self->owner->timeline) {
            const u64* key = second ? &g_unk_710164b1f0 : &g_unk_710164b1e8;
            goto start;  // the mask gets no lifetime markers and keeps its own slot
            base::reflection::UnkReflTypeIdArg mask;
        start:
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
        }
    }
    for (u64 i = 0; i < self->count; ++i) {
        UnkWaterEntry_A5_b63* entry = &self->entries[i];
        if (entry->state == 1) {
            Resolve_A5_b63(entry);
            entry->state = 2;
        }
        auto* c = static_cast<UnkCountedWater_A5_b63*>(entry->handle.ptr);
        if (UnkWaterTarget* target = fn_710089a1b0(c ? c->field_0x10 : nullptr))
            fn_71007e6778(target, -1.0f);
    }
    self->field_0xd48 = true;
}
