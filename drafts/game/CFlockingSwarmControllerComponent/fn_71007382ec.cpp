// CFlockingSwarmControllerComponent unit: creates the swarm's tile blockers once, the ones named
// "internal_tileblocker_0" to "_15" that the owner's scenario provides, and keeps each one found
// (after calling its vf_0x60 with false). Names and layouts are placeholders.
#include <cstdio>
#include "../../../build/lean/scratch/game/UnkGameManagerIdTable.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkTileBlocker_l7382ec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60(bool enabled);
};

// Finds an object by name: only the slot this unit calls is named.
struct UnkTileBlockerSource_l7382ec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual UnkTileBlocker_l7382ec* vf_0x168(const CStrId& name, const CStrId& alias, const CStrId& type,
                                             const CStrId& group, bool arg4, bool arg5, bool arg6, void* arg7,
                                             s32 arg8, void* arg9, s32 arg10, void* arg11, s32 arg12, bool arg13,
                                             bool arg14, bool arg15);
};

struct UnkSwarmOwner_l7382ec {
    u8 pad_0x0[0x2a0];
    UnkTileBlockerSource_l7382ec* field_0x2a0;
};

// The blockers found: a fixed-capacity list whose elements live in raw storage after the count.
struct UnkTileBlockers_l7382ec {
    u32 count;
    u8 storage[16 * sizeof(UnkTileBlocker_l7382ec*)];

    UnkTileBlocker_l7382ec** Data() { return reinterpret_cast<UnkTileBlocker_l7382ec**>(storage); }
    void Add(UnkTileBlocker_l7382ec* blocker) { Data()[count++] = blocker; }
};

struct UnkFlockingSwarm_l7382ec {
    u8 pad_0x0[0x30];
    UnkSwarmOwner_l7382ec* field_0x30;
    u8 pad_0x38[0x3e0 - 0x38];
    UnkTileBlockers_l7382ec blockers;
};
static_assert(__builtin_offsetof(UnkFlockingSwarm_l7382ec, blockers) == 0x3e0, "layout");

void fn_71007382ec(UnkFlockingSwarm_l7382ec* self) {
    if (self->blockers.count != 0 || !self->field_0x30->field_0x2a0)
        return;
    for (u32 i = 0; i < 16; ++i) {
        char text[0x40];
        snprintf(text, sizeof(text), "internal_tileblocker_%u", i);
        CStrId name(text, true);
        UnkTileBlocker_l7382ec* blocker = self->field_0x30->field_0x2a0->vf_0x168(
            name, name, g_unk_7101d470e0, GameManagerIdTable().field_0x1e70, false, false, true, nullptr, -1,
            nullptr, -1, nullptr, -1, false, false, true);
        if (blocker) {
            blocker->vf_0x60(false);
            self->blockers.Add(blocker);
        }
    }
}
