// CHydrogigaZiplineRailComponent's unit (tools/tu_map.py unit 1411): refreshes the rail for its "dead" flag
// (+0x3d): the owner's timeline key (g_unk_710164b218 dead, g_unk_710164b220 alive), every linked part
// (as CHydrogigaZiplineRailComponent_b49.cpp walks them) and the material 0xb712c6b2c8a50af6 of the
// owner's model, with a 0.2 fade. Names and layouts are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern u64 g_unk_710164b218;
extern u64 g_unk_710164b220;
extern const f32 g_unk_710164b228[] __attribute__((visibility("hidden")));
extern const f32 g_unk_710164b238[] __attribute__((visibility("hidden")));

void fn_71001c3040(void* object, u64 hash, u32 a, const f32* values, u32 count, f32 f);

// Typed as CHydrogigaZiplineRailComponent_b49.cpp declares them (one signature per symbol).
struct UnkZiplineRail_b49;
struct UnkZiplineLink_b49;
struct UnkZiplineLinkTarget_b49;
UnkZiplineLinkTarget_b49** fn_71003b0c4c(UnkZiplineLink_b49* self);
struct UnkZiplineRailPart_b49;
UnkZiplineRailPart_b49* fn_71005c3084(void* object);
void fn_710091c994(UnkZiplineRailPart_b49* self, bool dead);

struct UnkZiplineTarget_ovb0115 {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkZiplineLink_ovb0115 {
    u8 pad_0x0[0x40];
};

struct UnkZiplineModel_ovb0115 {
    u8 pad_0x0[0x138];
    void* slots;  // 0x138
};

struct UnkZiplineModelComponent_ovb0115 {
    u8 pad_0x0[0x50];
    UnkZiplineModel_ovb0115* model;  // 0x50
};

struct UnkZiplineOwner_ovb0115 {
    u8 pad_0x0[0x228];
    UnkZiplineModelComponent_ovb0115* model;  // 0x228
};

struct UnkZiplineRail_ovb0115 {
    u8 pad_0x0[0x30];
    UnkZiplineOwner_ovb0115* owner;  // 0x30
    u8 pad_0x38[5];
    bool dead;  // 0x3d
    u8 pad_0x3e[2];
    UnkZiplineLink_ovb0115* links;  // 0x40
    u32 count;                      // 0x48
};

// Refreshes the rail for its "dead" flag: the owner's timeline, every linked part, and the material.
void fn_710091d4bc(UnkZiplineRail_b49* rail) {
    UnkZiplineRail_ovb0115* self = reinterpret_cast<UnkZiplineRail_ovb0115*>(rail);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), self->dead ? &g_unk_710164b218 : &g_unk_710164b220,
                  false);
    bool dead = self->dead;
    if (self->count) {
        UnkZiplineLink_ovb0115* it = self->links;
        do {
            UnkZiplineTarget_ovb0115* target = reinterpret_cast<UnkZiplineTarget_ovb0115*>(
                *fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(it)));
            fn_710091c994(fn_71005c3084(target ? target->object : nullptr), dead);
            ++it;
        } while (it != self->links + self->count);
        dead = self->dead;
    }
    const f32* values = dead ? g_unk_710164b228 : g_unk_710164b238;
    fn_71001c3040(self->owner->model->model->slots, 0xb712c6b2c8a50af6, 0, values, 1, 0.2f);
}
