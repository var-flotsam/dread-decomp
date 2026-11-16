// CHydrogigaAIComponent's unit: fn_71005bf13c looks up the two "zipline_bottom_hydrogiga" entities and sets
// each one's rail to the Hydrogiga's value (+0x55c of the object at +0x149a0), then to the level of its two
// linked parts. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU883.h"
#include "types.h"

using base::global::CStrId;

struct UnkZiplineComponent_b0080;
void fn_710091c8b8(UnkZiplineComponent_b0080* self, f32 value);
void fn_710091c8c0(UnkZiplineComponent_b0080* self, f32 value);
char* fn_71005c1008(u64 id);
f32 fn_7100a68874(char* object);

struct UnkHydrogigaPartRef_l5bf13c {
    u8 pad_0x0[0x18];
    u64 id;  // 0x18
};

struct UnkHydrogigaPartLink_l5bf13c {
    u8 pad_0x0[0x10];
    UnkHydrogigaPartRef_l5bf13c* ref;  // 0x10
};

struct UnkHydrogigaValue_l5bf13c {
    u8 pad_0x0[0x55c];
    f32 value;  // 0x55c
};

struct UnkHydrogigaZiplines_l5bf13c {
    u8 pad_0x0[0x149a0];
    UnkHydrogigaValue_l5bf13c* field_0x149a0;
    UnkHydrogigaPartLink_l5bf13c* parts[2];  // 0x149a8
};

// The level of the two linked parts: that of the first flagged one (+0x44), else the first part's value.
static inline f32 Level(UnkHydrogigaZiplines_l5bf13c* self) {
    char* a = fn_71005c1008(self->parts[0]->ref->id);
    char* b = fn_71005c1008(self->parts[1]->ref->id);
    if (a[0x44] | b[0x44])
        return fn_7100a68874(a[0x44] ? a : b);
    return *reinterpret_cast<f32*>(a + 0x178);
}

// The entity named `name`, looked up in the game manager.
static inline UnkWeakTarget* FindEntity(const CStrId& name) {
    u64 key = name.data->Key();
    UnkGameManager* manager = g_unk_7101d47310;
    alignas(8) u32 kind = g_unk_71727bebb8;
    return fn_7100debbc8(manager, &key, &kind);
}

void fn_71005bf13c(UnkHydrogigaZiplines_l5bf13c* self) {
    static CStrId s_zipline0("zipline_bottom_hydrogiga_000", true);
    static CStrId s_zipline1("zipline_bottom_hydrogiga_001", true);
    base::global::CRntVector<UnkWeakTarget*> ziplines(0xf);
    ziplines.PushBack(FindEntity(s_zipline0));
    ziplines.PushBack(FindEntity(s_zipline1));
    for (u32 i = 0; i < ziplines.count; ++i) {
        if (UnkWeakTarget* zipline = ziplines.data[i]) {
            auto* rail = reinterpret_cast<UnkZiplineComponent_b0080*>(fn_71005c3084(zipline));
            fn_710091c8b8(rail, self->field_0x149a0->value);
            fn_710091c8c0(rail, Level(self));
        }
    }
    ziplines.Free();
}
