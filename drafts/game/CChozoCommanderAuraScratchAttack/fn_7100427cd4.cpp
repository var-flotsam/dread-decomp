// CChozoCommanderAuraScratchAttack's unit (tools/tu_map.py unit 645): the scratch's start. After the ground
// attack's step (fn_710042bb3c), with a commander AI and the owner's spawner (+0x2a0), spawns the unit's
// object (slot 0x168, as src/game/UnkUnit1049/UnkUnit1049_b50.cpp) and hands its part of kind 4 the AI's
// area (+0x15d1c) raised by 150, then starts the part (slot 0x80) and releases the object (slot 0x48).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChozoCommanderGroundAttack.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern char g_unk_71726c72c0[] __attribute__((visibility("hidden")));

// An area: a position and a size.
struct UnkScratchArea_l427cd4 {
    u32 field_0x0;
    f32 y;
    u32 field_0x8;
    u32 field_0xc;
};

struct UnkScratchAI_l427cd4 : UnkReflObject {
    u8 pad_0x8[0x15d1c - 0x8];
    UnkScratchArea_l427cd4 area;  // 0x15d1c
};
static_assert(__builtin_offsetof(UnkScratchAI_l427cd4, area) == 0x15d1c, "layout");

struct UnkScratchPart_l427cd4 {
    void** vtable;
    u8 pad_0x8[0xa0];
    UnkScratchArea_l427cd4 area;  // 0xa8

    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkScratchPart_l427cd4*)>(vtable[0x18 / 8])(this); }
    void vf_0x80() { reinterpret_cast<void (*)(UnkScratchPart_l427cd4*)>(vtable[0x80 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkScratchPart_l427cd4, area) == 0xa8, "layout");

struct UnkScratchSpawned_l427cd4 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkScratchPart_l427cd4* part;  // 0x158

    void vf_0x48() { reinterpret_cast<void (*)(UnkScratchSpawned_l427cd4*)>(vtable[0x48 / 8])(this); }
};

struct UnkScratchSpawner_l427cd4 {
    void** vtable;

    UnkScratchSpawned_l427cd4* vf_0x168(const void* a, const void* b, const void* c, const void* d, u32 e, u32 f,
                                        u32 g, u64 h, s32 i, u64 j, s32 k, u64 l, s32 m, bool n, bool o, bool p) {
        using Fn = UnkScratchSpawned_l427cd4* (*)(UnkScratchSpawner_l427cd4*, const void*, const void*, const void*,
                                                  const void*, u32, u32, u32, u64, s32, u64, s32, u64, s32, bool,
                                                  bool, bool);
        return reinterpret_cast<Fn>(vtable[0x168 / 8])(this, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);
    }
};

struct UnkScratchOwner_l427cd4 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x2a0 - 0x248];
    UnkScratchSpawner_l427cd4* spawner;  // 0x2a0
};

struct UnkScratchAttack_l427cd4 {
    void** vtable;
    UnkScratchOwner_l427cd4* owner;  // 0x8
};

// The part when it is of kind 4.
static inline UnkScratchPart_l427cd4* AreaPart(UnkScratchPart_l427cd4* part) {
    return part && part->vf_0x18() == 4 ? part : nullptr;
}

void fn_7100427cd4(UnkScratchAttack_l427cd4* self) {
    fn_710042bb3c(reinterpret_cast<UnkSelf*>(self));
    UnkReflObject* object = self->owner->ai;
    if (!object)
        return;
    UnkScratchSpawner_l427cd4* spawner = self->owner->spawner;
    bool is_ai = fn_710009d50c(object->vf_0x0(), fn_710044e708());
    UnkScratchAI_l427cd4* ai = is_ai ? static_cast<UnkScratchAI_l427cd4*>(object) : nullptr;
    if (!spawner || !is_ai)
        return;
    UnkScratchSpawned_l427cd4* spawned =
        spawner->vf_0x168(g_unk_71726c72c0, g_unk_71726c72c0, &g_unk_7101d470e0, &GameManagerIdTable().field_0x628, 0,
                          0, 1, 0, -1, 0, -1, 0, -1, false, false, true);
    if (!spawned)
        return;
    u32 field_0x0 = ai->area.field_0x0;
    f32 y = ai->area.y + 150.0f;
    u32 field_0x8 = ai->area.field_0x8;
    u32 field_0xc = ai->area.field_0xc;
    UnkScratchPart_l427cd4* part = AreaPart(spawned->part);
    part->area.field_0x0 = field_0x0;
    part->area.y = y;
    part->area.field_0x8 = field_0x8;
    part->area.field_0xc = field_0xc;
    part->vf_0x80();
    spawned->vf_0x48();
}
