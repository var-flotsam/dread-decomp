// CFireComponent unit (tools/tu_map.py unit 1300): the update. Runs the time on; a pending fire request
// (+0x40) waits while the owner's animation plays a CCharClassScheduleFireTrack, else it is sent (when
// ready, a missile gun's, or of kind 1) and dropped. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkBaseComponent;
void fn_710024fb28(UnkBaseComponent*);  // the actor component base step
void fn_710035145c(char* self);

struct UnkFireComponent_b57;
struct UnkFireMessage_b57;
bool fn_7100831aa0(UnkFireComponent_b57* self, UnkFireMessage_b57* message, void* unused, s32* kind);

struct UnkFireRequest_l831c64 {
    void** vtable;
    u8 pad_0x8[0x1871 - 0x8];
    bool ready;  // 0x1871

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkFireRequest_l831c64*)>(vtable[0])(this);
    }
};

struct UnkFireTarget_l831c64 {
    u8 pad_0x0[0x990];
    bool field_0x990;
};

struct UnkFireOwner_l831c64 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2b0 - 0x228];
    UnkFireTarget_l831c64* target;  // 0x2b0
};

// The request part of the component.
struct UnkFireBase_l831c64 {
    u8 pad_0x0[0x30];
    UnkFireOwner_l831c64* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkFireRequest_l831c64* request;  // 0x40
    u8 pad_0x48[0x4];
    s32 kind;  // 0x4c

    bool IsMissile() {
        return request && fn_710009d50c(request->vf_0x0(), fn_710084fd8c());
    }
    void NotifyMissile() {
        if (IsMissile() && kind == 2)
            fn_710035145c(reinterpret_cast<char*>(owner->target));
    }
};

struct UnkFire_l831c64 : UnkFireBase_l831c64 {
    u8 pad_0x50[0x8];
    f32 time;  // 0x58
};
static_assert(__builtin_offsetof(UnkFire_l831c64, time) == 0x58, "");

// The target's flag is cleared every update.
static inline void ClearTarget(UnkFire_l831c64* self) {
    if (UnkFireTarget_l831c64* target = self->owner->target)
        target->field_0x990 = false;
}

void fn_7100831c64(UnkFire_l831c64* self, f32 dt) {
    fn_710024fb28(reinterpret_cast<UnkBaseComponent*>(self));
    self->time += dt;
    if (self->request) {
        UnkAnimationComponent* animation = self->owner->animation;
        if (!animation || !fn_710117b348(animation, fn_7100c49e48())) {
            if (self->request->ready || self->IsMissile() || self->kind == 1) {
                self->NotifyMissile();
                fn_7100831aa0(reinterpret_cast<UnkFireComponent_b57*>(self),
                              reinterpret_cast<UnkFireMessage_b57*>(self->request), nullptr, &self->kind);
            }
            self->request = nullptr;
        } else {
            // Scheduled: the request waits.
            self->NotifyMissile();
        }
    }
    ClearTarget(self);
}