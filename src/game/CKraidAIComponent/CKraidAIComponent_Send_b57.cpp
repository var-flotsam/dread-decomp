// The CKraidAIComponent unit. fn_71005fe634: unless the owner's (+0x30) sub-object at +0x220 is in a
// CCharClassKraidBellyGrabTrack track, sends it the hash of its own string id at +0x2b8 under kind
// 0xf4061ff25a2163d6 with mode 2 (descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h). Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CGrabComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkKraidOwner_b57a4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkKraidAI_b57a4 {
    u8 pad_0x0[0x30];
    UnkKraidOwner_b57a4* owner;
};

static inline u64 HashOf_b57a4(const void* obj, u32 offset) {
    const CStrId* id = reinterpret_cast<const CStrId*>(reinterpret_cast<const char*>(obj) + offset);
    return *reinterpret_cast<u64*>(reinterpret_cast<char*>(id->data) + 0x30);
}

static inline __attribute__((always_inline)) void Send_b57a4(void* target, u64 hash, u64 kind, u32 m) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_71005fe634(UnkKraidAI_b57a4* self) {
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (target && !fn_710117b348(target, fn_7100c38c48()))
        Send_b57a4(target, HashOf_b57a4(target, 0x2b8), 0xf4061ff25a2163d6, 2);
}

// fn_71005fcc40: the component at +0x10 of the owner's +0x260 object, when it is a CCharClassGrabComponent (the
// class getter fn_7100790104), hands over its grab at +0x70: the grab's word at +0x30 becomes 1 and the one
// at +0x24 0, the player's +0x260 component (CGrabComponent unit, fn_710113d940) takes it with the
// owner, and the owner's sub-object at +0x220 is sent hash 0xdec1d0efc9e2415f with mode 2.
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkKraidGrab_b57a4 {
    u8 pad_0x0[0x24];
    u32 field_0x24;
    u8 pad_0x28[0x8];
    u32 field_0x30;
};
struct UnkKraidGrabber_b57a4 {
    u8 pad_0x0[0x260];
    void* field_0x260;
};

struct UnkKraidGrabHolder_b57a4 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x68];
    UnkKraidGrab_b57a4* field_0x70;
};
struct UnkKraidGrabSlot_b57a4 {
    u8 pad_0x0[0x10];
    UnkKraidGrabHolder_b57a4* field_0x10;
};
struct UnkKraidOwner2_b57a4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x38];
    UnkKraidGrabSlot_b57a4* field_0x260;
};
struct UnkKraidAI2_b57a4 {
    u8 pad_0x0[0x30];
    UnkKraidOwner2_b57a4* owner;
};

void fn_71005fcc40(UnkKraidAI2_b57a4* self) {
    UnkKraidGrabHolder_b57a4* holder = self->owner->field_0x260->field_0x10;
    UnkKraidGrabHolder_b57a4* cast = fn_710009d50c(holder->vf_0x0(), fn_7100790104()) ? holder : nullptr;
    UnkKraidGrab_b57a4* grab = cast->field_0x70;
    if (grab) {
        grab->field_0x30 = 1;
        grab->field_0x24 = 0;
        UnkKraidGrabber_b57a4* player = reinterpret_cast<UnkKraidGrabber_b57a4*>(fn_7100dec788(g_unk_7101d47310));
        if (player->field_0x260) {
            fn_710113d940(player->field_0x260, grab, self->owner);
            Send_b57a4(self->owner->field_0x220, 0xdec1d0efc9e2415f, 0x9317bfcf28edee2b, 2);
        }
    }
}

// fn_71005fd9cc: while the timer at +0x149b0 is zero and the flag at +0x149c0 clear, when fn_7100917ba8
// holds for the object at +0x149b8, the owner's sub-object at +0x220 is in a track of the class of
// fn_7100c0e948 and plays neither of the tracks 0x116ba2b3da4ed40e and 0x7419d9ec82a263b6
// (fn_710117ae38), it is sent the first of the two hashes with mode 2.
struct UnkKraidBlock_b57a4 {
    f32 timer;
    u8 pad_0x4[0x4];
    void* field_0x8;
    bool field_0x10;
};
struct UnkKraidAI3_b57a4 {
    u8 pad_0x0[0x30];
    UnkKraidOwner_b57a4* owner;
    u8 pad_0x38[0x149b0 - 0x38];
    UnkKraidBlock_b57a4 field_0x149b0;
};
bool fn_7100917ba8(void* obj);

void fn_71005fd9cc(UnkKraidAI3_b57a4* self) {
    UnkKraidBlock_b57a4* block = &self->field_0x149b0;
    if (block->timer != 0.0f || block->field_0x10)
        return;
    if (!fn_7100917ba8(block->field_0x8))
        return;
    if (!fn_710117b348(self->owner->field_0x220, fn_7100c0e948()))
        return;
    const u64 hash = 0x116ba2b3da4ed40e;
    if (fn_710117ae38(self->owner->field_0x220, hash, false))
        return;
    if (fn_710117ae38(self->owner->field_0x220, 0x7419d9ec82a263b6, false))
        return;
    Send_b57a4(self->owner->field_0x220, hash, 0xf4061ff25a2163d6, 2);
}
