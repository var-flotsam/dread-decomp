// CHydrogigaAIComponent's unit (tools/tu_map.py): looks up the zipline rail entity "zipline_rail_hydrogiga_000"
// in the game manager (fn_7100debbc8, kind g_unk_71727bebb8), sets its dead flag (fn_710091d5c0) and the
// flag of both parts' objects (+0x149a8, +0x149b0; fn_7100898434); turning the flag off also forces both
// objects' second flag on (fn_7100898498) and sets +0x149c0. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;
struct UnkZiplineRail_A5_b65;
struct UnkHidrogigaActivator_ovb0036;
struct UnkShootActivatorHidrogiga_b47;
extern u32 g_unk_71727bebb8;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);
UnkZiplineRail_A5_b65* fn_71005c32d4(UnkWeakTarget* entity);
void fn_710091d5c0(UnkZiplineRail_A5_b65* self, bool dead);
void fn_7100898434(UnkHidrogigaActivator_ovb0036* self, bool enabled);
void fn_7100898498(UnkShootActivatorHidrogiga_b47* self, bool value);

struct UnkStrIdHashed_b0166 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkHydrogigaPart_b0166 {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkHydrogigaAI_b0166 {
    u8 pad_0x0[0x149a8];
    UnkHydrogigaPart_b0166* parts[2];  // 0x149a8
    u8 pad_0x149b8[8];
    bool field_0x149c0;
};

static inline void* ObjectOf(UnkHydrogigaPart_b0166* part) { return part ? part->object : nullptr; }

void fn_71005c3160(UnkHydrogigaAI_b0166* self, bool flag) {
    static CStrId s_rail("zipline_rail_hydrogiga_000", true);
    u64 key;
    alignas(8) u32 kind;
    UnkGameManager* manager = g_unk_7101d47310;
    key = reinterpret_cast<UnkStrIdHashed_b0166*>(s_rail.data)->hash;
    kind = g_unk_71727bebb8;
    UnkWeakTarget* entity = fn_7100debbc8(manager, &key, &kind);
    fn_710091d5c0(fn_71005c32d4(entity), flag);
    fn_7100898434(static_cast<UnkHidrogigaActivator_ovb0036*>(ObjectOf(self->parts[0])), flag);
    fn_7100898434(static_cast<UnkHidrogigaActivator_ovb0036*>(ObjectOf(self->parts[1])), flag);
    if (!flag) {
        fn_7100898498(static_cast<UnkShootActivatorHidrogiga_b47*>(ObjectOf(self->parts[0])), true);
        fn_7100898498(static_cast<UnkShootActivatorHidrogiga_b47*>(ObjectOf(self->parts[1])), true);
        self->field_0x149c0 = true;
    }
}
