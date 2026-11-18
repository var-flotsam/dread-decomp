// CAccessPointComponent's unit (tools/tu_map.py, unit 1581): when the owner (+0x30) is enabled (+0x1b4),
// keeps a counted reference to the player's weak handle at +0xe8, hands the player's movement target
// (+0x208) a transition of the vtable-0x7101a874c0 kind, then runs fn_7100aa01a4 and its own virtual at
// 0x2b0. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a874c0[];

struct UnkUsable_b0101;
void fn_7100aa01a4(UnkUsable_b0101* self);

struct UnkAccessTarget_la70e48;
void fn_710031433c(UnkAccessTarget_la70e48* target, void* params);

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_la70e48 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_la70e48 {
    s64 id;
    UnkDesc16_la70e48 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_la70e48(UnkDesc16_la70e48 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_la70e48()
        : UnkMoveTransition_la70e48(*reinterpret_cast<const UnkDesc16_la70e48*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_la70e48) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_la70e48 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with the access point's vtable.
struct UnkAccessParams_la70e48 {
    void* vtable;
    UnkMoveTransition_la70e48 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_la70e48 field_0x3c;
    u32 field_0x44;

    UnkAccessParams_la70e48() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a874c0) + 0x10;
    }
};
static_assert(sizeof(UnkAccessParams_la70e48) == 0x48, "layout");

struct UnkAccessPlayer_la70e48 {
    u8 pad_0x0[0x208];
    UnkAccessTarget_la70e48* movement;  // 0x208
};

struct UnkAccessOwner_la70e48 {
    u8 pad_0x0[0x1b4];
    bool enabled;  // 0x1b4
};

struct UnkAccessPoint_la70e48;
struct UnkAccessPointVtable_la70e48 {
    void* pad_0x0[0x2b0 / 8];
    void (*vf_0x2b0)(UnkAccessPoint_la70e48* self);
};

struct UnkAccessPoint_la70e48 {
    UnkAccessPointVtable_la70e48* vtable;
    u8 pad_0x8[0x28];
    UnkAccessOwner_la70e48* owner;  // 0x30
    u8 pad_0x38[0xb0];
    UnkWeakHandle* player_handle;  // 0xe8
};

void fn_7100a70e48(UnkAccessPoint_la70e48* self) {
    if (!self->owner->enabled)
        return;
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    AssignRef(&self->player_handle, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(player)));
    if (UnkAccessTarget_la70e48* movement = reinterpret_cast<UnkAccessPlayer_la70e48*>(player)->movement) {
        UnkAccessParams_la70e48 params;
        fn_710031433c(movement, &params);
    }
    fn_7100aa01a4(reinterpret_cast<UnkUsable_b0101*>(self));
    self->vtable->vf_0x2b0(self);
}
