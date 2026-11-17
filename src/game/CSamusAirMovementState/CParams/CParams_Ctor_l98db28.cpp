// CSamusAirMovementState::CParams's unit (tools/tu_map.py unit 1455): a constructor of a small message:
// its vtable, a default movement transition (no id, the two global descriptors, the all-flags mask),
// a kind of 3 and cleared words. Names and layouts are placeholders.
#include <new>

#include "../../../game_s2/UnkSpitSend.h"
#include "game/CAbilityGhostAura.h"
#include "types.h"

// As src/game/CSamusFloorSlideMovementState/CParams/CParams_Slide_ovb0004.cpp declares it.
struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_l98db28 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkTransition_l98db28 {
    s64 id;
    UnkDesc16_l98db28 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkTransition_l98db28(UnkDesc16_l98db28 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
};
static_assert(sizeof(UnkTransition_l98db28) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_l98db28 __attribute__((aligned(4)));

struct UnkAirMessage_l98db28 {
    void** vtable;
    UnkTransition_l98db28 transition;  // 0x8
    u32 kind;                              // 0x38
    UnkU64Align4_l98db28 field_0x3c;
    u32 field_0x44;
};
static_assert(sizeof(UnkAirMessage_l98db28) == 0x48, "layout");

void fn_710098db28(UnkAirMessage_l98db28* self) {
    self->vtable = &g_unk_7101a22cb8[2];
    new (&self->transition) UnkTransition_l98db28(*reinterpret_cast<const UnkDesc16_l98db28*>(&g_unk_710164b958),
                                                  *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                                  UnkAll_SpitSend().value);
    self->kind = 3;
    self->field_0x3c = 0;
    self->field_0x44 = 0;
}
