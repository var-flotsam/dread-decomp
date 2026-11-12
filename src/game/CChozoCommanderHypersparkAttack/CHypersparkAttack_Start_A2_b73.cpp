// CChozoCommanderHypersparkAttack's unit (tools/tu_map.py): the attack's start. After the base start
// (fn_710042bb3c) it clears its state, copies the owner's +0x54, hands the owner's effect sink (+0x218) a
// kind-3 parameter block and, when the owner's collision container (+0x2a0) holds the object named
// g_unk_71726c79c0, gives it to the sink (vf_0x2b0). It records at +0x68 whether the actor it acts against
// (fn_7100d7c990) is flagged, then hooks the callback fn_710042e2dc (link at +0x78, handle at +0x90) up to the
// object the owner tracks (+0x220), as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp.
// Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CChozoCommanderGroundAttack.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSelf;
struct UnkEffectSink;
struct UnkFlagged;
void fn_710042e2dc();

extern base::global::CStrId g_unk_71726c79c0 __attribute__((visibility("hidden")));

struct UnkHypersparkParams_A2_b73 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkHypersparkLink_A2_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkHypersparkStart_A2_b73 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x48];
    u32 field_0x58;
    u8 field_0x5c[13];  // 0x68 is its last byte
    u8 pad_0x69[3];
    f32 field_0x6c;
    u8 pad_0x70[0x8];
    UnkHypersparkLink_A2_b73 link;  // 0x78
    UnkWeakHandle* handle;          // 0x90
};

void fn_710042ce00(UnkHypersparkStart_A2_b73* self) {
    fn_710042bb3c(reinterpret_cast<UnkSelf*>(self));
    UnkActor* owner = self->owner;
    *reinterpret_cast<u64*>(self->field_0x5c + 5) = 0;
    *reinterpret_cast<u64*>(self->field_0x5c) = 0;
    self->field_0x6c = owner->field_0x54;
    self->field_0x58 = 0;
    UnkEffectSink* sink = static_cast<UnkEffectSink*>(owner->field_0x218);
    auto* collision = *reinterpret_cast<UnkCollisionContainer**>(reinterpret_cast<char*>(owner) + 0x2a0);
    if (sink && collision) {
        UnkHypersparkParams_A2_b73 params;
        params.field_0xc = 0;
        params.field_0x14 = 0;
        __builtin_memset(params.field_0x18, 0, 13);
        __builtin_memset(params.field_0x28, 0, 13);
        params.vtable = g_unk_7101a2e208 + 2;
        params.field_0x3d = 0;
        params.field_0x38 = 3;
        fn_71010c7e6c(sink, &params, 1);
        if (UnkCollisionObject* object = fn_71008640a4(collision, &g_unk_71726c79c0))
            (*reinterpret_cast<void (***)(UnkEffectSink*, UnkCollisionObject*)>(sink))[0x2b0 / 8](sink, object);
    }
    if (UnkActor* target = fn_7100d7c990(self->owner)) {
        UnkFlagged* flagged = static_cast<UnkFlagged*>(target->field_0x208);
        if (flagged && reinterpret_cast<u8*>(flagged)[0xf9] &&
            (fn_71009e75e8(flagged) || fn_7100df5630(g_unk_7101d47310)))
            self->field_0x5c[12] = 1;
    }
    UnkHypersparkLink_A2_b73* link = &self->link;
    link->context = self;
    link->callback = fn_710042e2dc;
    UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (!track)
        return;
    asm("" : "+r"(self));
    fn_7100316018(track, &self->handle);
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}
