// behaviortree::CEmmySearchingAction's unit (tools/tu_map.py unit 805): starts the search unless the
// E.M.M.I. is busy; when it can be left, waits for the movement instead (its listener), and when the
// movement is in mode 4 on one of two targets, does nothing. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/global/CStrId.h"
#include "game/CEnemyMovement.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU376.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "types.h"

struct UnkEmmySearchingAction_b0068;
struct UnkEmmySearchingAction_A3_b67;
bool CEmmySearchingAction_ShouldDeferSearchStart(UnkEmmySearchingAction_A3_b67* self);
void CEmmySearchingAction_TryStartSearch(UnkEmmySearchingAction_b0068* self, bool flag);
void CEmmySearchingAction_OnMovementModeChanged(UnkEmmySearchingAction_b0068* self, s32 from, s32 to);

// What the movement calls when its state changes: the object and the function called with it.
struct UnkEmmyMovementListener_l519048 {
    UnkEmmySearchingAction_b0068* self;
    void (*fn)(UnkEmmySearchingAction_b0068* self, s32 from, s32 to);
};

// The movement's current target: its name is the string id at +0x38.
struct UnkEmmyTarget_l519048 {
    u8 pad_0x0[0x38];
    base::global::UnkStringData* name;  // 0x38
    u8 pad_0x40[0xc0];

    UnkEmmyTarget_l519048() { fn_71002b57bc(Object()); }
    ~UnkEmmyTarget_l519048() { fn_71002b6094(Object()); }
    void Assign(const UnkObject_0x100* other) { fn_71002b5c90(Object(), other); }
    UnkObject_0x100* Object() { return reinterpret_cast<UnkObject_0x100*>(this); }
};

struct UnkEmmyMovement_l519048 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1868];
    UnkObject_0x100 target;  // 0x1870
    bool hasTarget;          // 0x1970
    u8 pad_0x1971[0x6f];
    UnkEmmyMovementListener_l519048 listener;  // 0x19e0
};
static_assert(__builtin_offsetof(UnkEmmyMovement_l519048, listener) == 0x19e0, "layout");

struct UnkEmmyAI_l519048 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x29410];
    u8 timer[0xb0c];  // 0x29418
    bool busy;        // 0x29f24
    bool field_0x29f25;
    bool locked;  // 0x29f26
};
static_assert(__builtin_offsetof(UnkEmmyAI_l519048, busy) == 0x29f24, "layout");

struct UnkEmmyActor_l519048 {
    u8 pad_0x0[0x1f0];
    UnkEmmyMovement_l519048* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkEmmyAI_l519048* ai;  // 0x240
};

struct UnkEmmySearchingAction_l519048 {
    u8 pad_0x0[0x18];
    UnkEmmyActor_l519048* actor;  // 0x18
    u8 pad_0x20[0x44];
    bool field_0x64;
    u8 pad_0x65[0x12a8c];
    bool waiting;    // 0x12af1
    bool searching;  // 0x12af2
    u8 pad_0x12af3[0x4d];
    UnkEmmyMovementListener_l519048 listener;  // 0x12b40
};
static_assert(__builtin_offsetof(UnkEmmySearchingAction_l519048, listener) == 0x12b40, "layout");

void fn_7100519048(UnkEmmySearchingAction_l519048* self) {
    UnkEmmyAI_l519048* ai = self->actor->ai;
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        ai = nullptr;
    if (self->searching)
        return;
    if (fn_710002fb70(ai->timer) || ai->locked || self->field_0x64)
        return;
    if (!ai->busy)
        fn_7100541de4(reinterpret_cast<UnkEmmyActor_b55*>(ai));

    bool canLeave = CEmmySearchingAction_ShouldDeferSearchStart(reinterpret_cast<UnkEmmySearchingAction_A3_b67*>(self));
    UnkEmmyMovement_l519048* movement = self->actor->movement;
    if (movement && !fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()))
        movement = nullptr;
    if (canLeave) {
        if (movement) {
            self->listener.self = reinterpret_cast<UnkEmmySearchingAction_b0068*>(self);
            self->listener.fn = CEmmySearchingAction_OnMovementModeChanged;
            movement->listener = self->listener;
        }
        self->waiting = true;
        return;
    }
    if (movement) {
        u32 mode = fn_71010cc3a4(movement);
        UnkEmmyTarget_l519048 target;
        if (mode == 4 && movement->hasTarget) {
            target.Assign(&movement->target);
            if (movement->hasTarget) {
                switch (target.name->Key()) {
                case 0x387e59ed19c454a7:
                case 0x542190be51e40729:
                    return;
                }
            }
        }
    }
    CEmmySearchingAction_TryStartSearch(reinterpret_cast<UnkEmmySearchingAction_b0068*>(self), false);
}
