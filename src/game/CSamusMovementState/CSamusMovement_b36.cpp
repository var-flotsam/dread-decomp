// Functions in the CSamusMovement unit: copies of fn_71009ec054 (CSamusMovement_b35.cpp). Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSamusMovementState2 {
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
    virtual void vf_0x58(void* arg);
    u8 pad_0x8[0x8];
    s32 kind;
};
#pragma pack(push, 4)
struct UnkSamusMovement2 {
    u8 pad_0x0[0x2d18];
    u32 state_count;
    UnkSamusMovementState2* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

static inline UnkSamusMovementState2* FindState(UnkSamusMovement2* self, s32 kind) {
    UnkSamusMovementState2** it = self->states;
    u64 size = (u64)self->state_count * sizeof(*it);
    asm("" : "+r"(size));
    for (; size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

// Forwards the argument to the first state of kind 5 (virtual slot 11), if any.
void fn_71009ebfc4(UnkSamusMovement2* self, void* arg) {
    if (UnkSamusMovementState2* state = FindState(self, 5))
        state->vf_0x58(arg);
}

// The same for kind 6.
void fn_71009ec00c(UnkSamusMovement2* self, void* arg) {
    if (UnkSamusMovementState2* state = FindState(self, 6))
        state->vf_0x58(arg);
}

struct UnkSamusMovementOwner {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
    u8 pad_0x18[0x2278 - 0x18];
    UnkStrIdPathResource resource;  // 0x2278
    u8 pad_0x2290[0x5458 - 0x2290];
    UnkReflObject* movement;  // 0x5458, the object as a CCharClassSamusMovement
};

bool fn_71010d79b8(UnkSamusMovementOwner* self, void* arg);  // the base method
extern __attribute__((visibility("hidden"))) UnkStrIdPathPair g_unk_7172741068;

// Calls the base method, caches the object at +0x10 when it is a CCharClassSamusMovement, then
// assigns the ids at +0x2278 from a global pair.
bool fn_71009dff24(UnkSamusMovementOwner* self, void* arg) {
    bool result = fn_71010d79b8(self, arg);
    UnkReflObject* object = self->object;
    self->movement = object && fn_710009d50c(object->vf_0x0(), fn_7100795584()) ? object : nullptr;
    fn_71000267f8(&self->resource, g_unk_7172741068);
    return result;
}

struct UnkSamusMovementVec3 {
    u32 x;
    u32 y;
    u32 z;
};

struct UnkSamusMovementListener {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28(void* arg);
};

struct UnkSamusMovementUpdate {
    void** vtable;
    u8 pad_0x8[0x1d0];
    UnkSamusMovementVec3 field_0x1d8;
    u8 pad_0x1e4[0x9c];
    u8 field_0x280[0x116];
    bool field_0x396;
    u8 pad_0x397[0x1fe9];
    UnkSamusMovementVec3 field_0x2380;
    u8 pad_0x238c[0x97c];
    UnkSamusMovementListener* field_0x2d08;
    u8 pad_0x2d10[0x24c4];
    bool field_0x51d4;
};
static_assert(__builtin_offsetof(UnkSamusMovementUpdate, field_0x51d4) == 0x51d4, "placeholder layout");

void fn_71010dae00(UnkSamusMovementUpdate* self, f32 dt);  // the base update
void fn_71009e4598(UnkSamusMovementUpdate* self);

// The update: after the base step, hands +0x280 to the listener at +0x2d08, clears +0x396, runs
// virtual slot 98, keeps the vector at +0x1d8 at +0x2380, then runs fn_71009e4598 when +0x51d4 is set.
void fn_71009e4500(UnkSamusMovementUpdate* self, f32 dt) {
    fn_71010dae00(self, dt);
    self->field_0x2d08->vf_0x28(self->field_0x280);
    self->field_0x396 = false;
    reinterpret_cast<void (*)(UnkSamusMovementUpdate*, f32)>(self->vtable[0x310 / 8])(self, dt);
    self->field_0x2380 = self->field_0x1d8;
    if (self->field_0x51d4)
        fn_71009e4598(self);
}
