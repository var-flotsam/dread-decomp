// CTeleporterUsableComponent's unit (tools/tu_map.py unit 1600): stops the effect of every mode in the
// mode-to-effect table (the effects fn_7100a8e4c0 starts). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "types.h"

using base::global::UnkStringData;

// A string id that is never null: the copy and release test only against the pool's empty id.
struct UnkFxId_b0233 {
    UnkStringData* data;

    UnkFxId_b0233(const UnkFxId_b0233& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkFxId_b0233() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
        }
    }
};

struct UnkTeleporterFxEntry_b0233 {
    u32 value;
    u32 pad_0x4;
    UnkFxId_b0233 id;
};
extern UnkTeleporterFxEntry_b0233 g_unk_7172751a60[8] __attribute__((visibility("hidden")));

struct UnkTeleporterFx_b0233 {
    u8 pad_0x0[0x228];
    void* field_0x228;
};

struct UnkTeleporterSelf_b0233 {
    u8 pad_0x0[0x30];
    UnkTeleporterFx_b0233* actor;  // 0x30
    u8 pad_0x38[0x550];
    u32 mode;  // 0x588
};

void fn_7100a8e614(UnkTeleporterSelf_b0233* self) {
#pragma clang loop unroll(full)
    for (UnkTeleporterFxEntry_b0233& entry : g_unk_7172751a60) {
        UnkFxId_b0233 name = entry.id;
        void* component = self->actor->field_0x228;
        u64 hash = name.data->Key();
        fn_71011738f8(static_cast<UnkEnergyWaveFxComp_b49*>(component), &hash, 0);
    }
}
