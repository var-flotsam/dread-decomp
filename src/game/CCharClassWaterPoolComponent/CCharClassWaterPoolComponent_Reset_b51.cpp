// CCharClassWaterPoolComponent (unit 1396): dropping the pool's two timeline ids, resolving every
// lazily resolved handle of its 0x48-byte entries (the shape of CTutoExitLogicAction_b47.cpp) and
// passing what each names to fn_71007e670c, before the shared reset fn_7100901260. Names and
// layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/CCharClassSteamJetComponent.h"
#include "game/UnkTimelineComponent.h"

struct UnkCountedWater_b51 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkRef {
    UnkCountedWater_b51* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkWaterTarget;
void fn_71007e670c(UnkWaterTarget* target, f32 value);

struct UnkLavaPool_b48;

extern u64 g_unk_710164b1e8;
extern u64 g_unk_710164b1f0;

struct UnkWaterEntry {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;  // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38
    u8 pad_0x40[0x8];
};

struct UnkWaterOwner {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;
};

struct UnkWaterPool {
    u8 pad_0x0[0x30];
    UnkWaterOwner* owner;
    u8 pad_0x38[0xc90 - 0x38];
    UnkWaterEntry* entries;  // 0xc90
    u32 count;               // 0xc98
    u8 pad_0xc9c[0xd48 - 0xc9c];
    bool field_0xd48;
};

void fn_7100909cdc(UnkWaterPool* self) {
    if (self->owner) {
        if (UnkTimelineComponent* timeline = self->owner->timeline) {
            fn_7100a2b7b8(timeline, &g_unk_710164b1e8);
            fn_7100a2b7b8(timeline, &g_unk_710164b1f0);
        }
    }
    for (u64 i = 0; i < self->count; ++i) {
        UnkWaterEntry* entry = &self->entries[i];
        if (entry->state == 1) {
            {
                UnkRef fresh = fn_7100353be8(entry->source);
                fn_710031d208(&entry->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            entry->state = 2;
        }
        auto* c = static_cast<UnkCountedWater_b51*>(entry->handle.ptr);
        if (UnkWaterTarget* target = fn_710089a1b0(c ? c->field_0x10 : nullptr))
            fn_71007e670c(target, -1.0f);
    }
    self->field_0xd48 = false;
    fn_7100901260(reinterpret_cast<UnkLavaPool_b48*>(self));
}
