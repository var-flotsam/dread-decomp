// CCharClassEmmyAIComponent unit (tools/tu_map.py): picks the animation name for the owner's
// situation: none while the movement holds still, the cell's own when the target's cell has one
// the animation knows, else by the target's state (3 or 4) and whether the Emmy AI is free to act.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CMovementComponent.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

// Pairs of animation names, and the one a cell asks for.
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726df5a0[2];
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726df5c0[2];
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726df5d0;

struct UnkEmmyTarget_l51ed5c {
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
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual void vf_0x230();
    virtual void vf_0x238();
    virtual void vf_0x240();
    virtual void vf_0x248();
    virtual void vf_0x250();
    virtual void vf_0x258();
    virtual void vf_0x260();
    virtual void vf_0x268();
    virtual void vf_0x270();
    virtual void vf_0x278();
    virtual void vf_0x280();
    virtual void vf_0x288();
    virtual void vf_0x290();
    virtual void vf_0x298();
    virtual void vf_0x2a0();
    virtual void vf_0x2a8();
    virtual void vf_0x2b0();
    virtual void vf_0x2b8();
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual UnkNavMeshCell* vf_0x2d0();
};

struct UnkEmmyAI_l51ed5c : UnkReflObject {
    u8 pad_0x8[0x2a370];
    u8 timer[8];  // 0x2a378
};

// The tunable's flag that keeps the Emmy from acting.
struct UnkEmmyTunable_l51ed5c {
    u8 pad_0x0[0xf4];
    bool field_0xf4;
};

struct UnkEmmyOwner_l51ed5c {
    u8 pad_0x0[0x218];
    UnkEmmyTarget_l51ed5c* target;  // 0x218
    UnkStruct_7101176ea8* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkEmmyAI_l51ed5c* ai;  // 0x240
};
static_assert(__builtin_offsetof(UnkEmmyOwner_l51ed5c, ai) == 0x240, "layout");

struct UnkEmmyAIComponent_l51ed5c;

const CStrId* fn_710051ed5c(UnkEmmyAIComponent_l51ed5c* self, UnkEmmyOwner_l51ed5c* owner) {
    UnkEmmyTarget_l51ed5c* target;
    if (!owner || !(target = owner->target))
        return &g_unk_71726df5a0[1];
    if (fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(target)))
        return &g_unk_7101d470e0;
    UnkNavMeshCell* cell = target->vf_0x2d0();
    if (cell && fn_71002b005c(cell) &&
        fn_7101176e78(owner->animation, reinterpret_cast<void*>(g_unk_71726df5d0.data->Key()), nullptr))
        return &g_unk_71726df5d0;
    UnkEmmyAI_l51ed5c* ai = owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) &&
        (fn_710054ad2c(reinterpret_cast<UnkEmmyAIView*>(ai)) ||
         fn_710054aecc(reinterpret_cast<UnkEmmyAI2_b62a3*>(ai)))) {
        bool waiting = fn_710002fb70(ai->timer);
        if (!waiting && !reinterpret_cast<UnkEmmyTunable_l51ed5c*>(&g_unk_71726e0aa8)->field_0xf4) {
            if (fn_71010cc38c(target) == 3 || fn_71010cc38c(target) == 4)
                return &g_unk_71726df5c0[1];
            return &g_unk_71726df5c0[0];
        }
    }
    if (fn_71010cc38c(target) == 3 || fn_71010cc38c(target) == 4)
        return &g_unk_71726df5a0[0];
    return &g_unk_71726df5a0[1];
}
