// Unit 1444 of tools/tu_map.py: starts the state. After the base start (fn_7100969130) it clears the
// state's bookkeeping, runs fn_71009719e0 when the movement is an enemy movement with its flag set, and,
// when an object is stored, tells the AI about it and sends the animation component the hash of the
// current smart-link track's name; it then records whether the state is done. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU1440.h"
#include "game/UnkTU1444.h"
#include "game/UnkTU376.h"
#include "types.h"

using base::global::CStrId;

// The enemy movement (CCharClassEnemyMovement) and its flag.
struct UnkMovement_l97176c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1b0];
    bool field_0x1b8;
};

struct UnkMovementHolder_l97176c {
    u8 pad_0x0[0x10];
    UnkMovement_l97176c* movement;  // 0x10
};

struct UnkAI_l97176c {
    void** vtable;

    // The name of the track the AI is following (slot 0x588).
    const CStrId& TrackName() {
        return reinterpret_cast<const CStrId& (*)(UnkAI_l97176c*)>(vtable[0x588 / 8])(this);
    }
};

struct UnkOwner_l97176c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkAI_l97176c* ai;  // 0x240
};

// The stored object's working copy (fn_71002b57bc builds it, fn_71002b6094 destroys it).
struct UnkPayload_l97176c {
    u8 pad_0x0[0x38];
    base::global::UnkStringData* name;  // 0x38
    u8 pad_0x40[0xc0];

    UnkPayload_l97176c() { fn_71002b57bc(Object()); }
    ~UnkPayload_l97176c() { fn_71002b6094(Object()); }
    UnkObject_0x100* Object() { return reinterpret_cast<UnkObject_0x100*>(this); }
};

// The state's bookkeeping, cleared on start.
struct UnkStartState_l97176c {
    u8 field_0x1d8[0x26];
    bool done;  // 0x1fe
    u8 pad_0x1ff;
    bool field_0x200;
    bool field_0x201;
    bool field_0x202;
    bool field_0x203;
    u8 field_0x204[2];
};

struct UnkState_l97176c {
    u8 pad_0x0[0x30];
    UnkOwner_l97176c* owner;              // 0x30
    UnkMovementHolder_l97176c* holder;    // 0x38
    u8 pad_0x40[0x90];
    UnkObject_0x100 stored;               // 0xd0
    bool hasStored;                       // 0x1d0
    u8 pad_0x1d1[7];
    UnkStartState_l97176c state;          // 0x1d8
};
static_assert(__builtin_offsetof(UnkState_l97176c, state) == 0x1d8, "UnkState_l97176c layout");

// Sends `hash` to `target` with mode 0 under the kind the state's requests use; `mask` is the caller's
// request argument, set here to let every kind through.
inline void SendHash_l97176c(void* target, u64 hash, base::reflection::UnkReflTypeIdArg* mask) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

void fn_71009719e0(UnkState_l97176c* self);
void fn_71011106f4(UnkAI_l97176c* ai, UnkObject_0x100* payload);

void fn_710097176c(UnkState_l97176c* self) {
    fn_7100969130(reinterpret_cast<char*>(self));
    self->state = UnkStartState_l97176c();
    if (UnkMovementHolder_l97176c* holder = self->holder) {
        UnkMovement_l97176c* movement = holder->movement;
        if (movement && fn_710009d50c(movement->vf_0x0(), fn_710095629c()) && movement->field_0x1b8)
            fn_71009719e0(self);
    }
    UnkPayload_l97176c payload;
    UnkTrackQuery query;
    if (self->hasStored)
        fn_71002b5c90(payload.Object(), &self->stored);
    if (!self->hasStored || payload.name == &g_unk_7101d47108->empty) {
        self->state.field_0x200 = true;
        self->state.field_0x201 = true;
        self->state.done = true;
        return;
    }
    const CStrId* name = nullptr;
    if (UnkAI_l97176c* ai = self->owner->ai) {
        fn_71011106f4(ai, payload.Object());
        if (UnkAI_l97176c* now = self->owner->ai)
            name = &now->TrackName();
    }
    if (!name || name->data == &g_unk_7101d47108->empty) {
        query.cls = fn_7100d29808();
        query.result = nullptr;
        query.value = -1.0f;
        fn_710117b08c(self->owner->animation, &query);
        if (query.result)
            name = &query.result->sources;
    }
    if (name && name->data != &g_unk_7101d47108->empty) {
        base::reflection::UnkReflTypeIdArg mask;
        SendHash_l97176c(self->owner->animation, name->data->Key(), &mask);
    }
    self->state.done = fn_7100971ee8(reinterpret_cast<UnkUnit1444_Event*>(self));
}
