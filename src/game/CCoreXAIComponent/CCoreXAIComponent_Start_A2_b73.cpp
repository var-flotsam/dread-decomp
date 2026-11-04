// CCoreXAIComponent's unit (tools/tu_map.py): a start step. After fn_71003acb5c and fn_71003aff64(1, 0), when
// fn_710110c438 reports kind 1 with a live target, copies the target's four values at +0x40 (refreshed through
// its vf_0x98 when its flag at +0x60 is set) to +0x149a8; stores the offset from the actor it acts against
// (fn_7100d7c990) to the owner (+0x30) at +0x149b8; then, when the owner has the objects at +0x228 (with its
// +0x50) and +0x220, hooks the callback fn_71004db7f8 (link at +0x149d8, handle at +0x149f0) up to the object
// the owner tracks (+0x220), as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp, and sets
// the +0x50 object's +0x114 when its +0x11c is set. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCoreXAI_A2_b73;
void fn_71003acb5c(UnkCoreXAI_A2_b73* self);
void fn_71003aff64(UnkCoreXAI_A2_b73* self, u32 a, u32 b);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_71004db7f8();

struct UnkCoreXSource_A2_b73 {
    virtual void vf_0x0();
    u8 pad_0x8[0x38];
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    u8 pad_0x50[0x10];
    bool field_0x60;
};

struct UnkCoreXQuery_A2_b73 {
    u8 pad_0x0[0x8];
    UnkWeakHandle** field_0x8;
    u32 field_0x10;
};
UnkCoreXQuery_A2_b73* fn_710110c438(UnkCoreXAI_A2_b73* self);

struct UnkCoreXFlags_A2_b73 {
    u8 pad_0x0[0x114];
    bool field_0x114;
    u8 pad_0x115[0x7];
    bool field_0x11c;
};

struct UnkCoreXShape_A2_b73 {
    u8 pad_0x0[0x50];
    UnkCoreXFlags_A2_b73* field_0x50;
};

struct UnkCoreXLink_A2_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkCoreXVec_A2_b73 {
    f32 x, y, z;
};

struct UnkCoreXState_A2_b73 {
    f32 field_0x0[4];
    UnkCoreXVec_A2_b73 field_0x10;
    u8 pad_0x1c[0x14];
    UnkCoreXLink_A2_b73 link;  // 0x30
    UnkWeakHandle* handle;     // 0x48
};

struct UnkCoreXAI_A2_b73 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14970];
    UnkCoreXState_A2_b73 state;  // 0x149a8
};

void fn_71004d99d4(UnkCoreXAI_A2_b73* self) {
    UnkCoreXState_A2_b73* state = &self->state;
    fn_71003acb5c(self);
    fn_71003aff64(self, 1, 0);
    if (UnkCoreXQuery_A2_b73* query = fn_710110c438(self)) {
        if (query->field_0x10 == 1) {
            if (UnkWeakHandle* handle = *query->field_0x8) {
                if (auto* source = reinterpret_cast<UnkCoreXSource_A2_b73*>(handle->target)) {
                    if (source->field_0x60)
                        (*reinterpret_cast<void (***)(UnkCoreXSource_A2_b73*)>(source))[0x98 / 8](source);
                    state->field_0x0[0] = source->field_0x40;
                    state->field_0x0[1] = source->field_0x44;
                    state->field_0x0[2] = source->field_0x48;
                    state->field_0x0[3] = source->field_0x4c;
                }
            }
        }
    }
    UnkActor* owner = self->owner;
    f32 x = owner->field_0x44;
    f32 y = owner->field_0x54;
    f32 z = owner->field_0x64;
    UnkActor* target = fn_7100d7c990(owner);
    f32 dx = x - target->field_0x44;
    f32 dy = y - target->field_0x54;
    f32 dz = z - target->field_0x64;
    state->field_0x10 = UnkCoreXVec_A2_b73{dx, dy, dz};
    asm volatile("" ::: "memory");  // the owner reload stays after the stores (they pair)
    owner = self->owner;
    auto* shape = static_cast<UnkCoreXShape_A2_b73*>(owner->field_0x228);
    if (!shape)
        return;
    auto* track = static_cast<UnkWeakTarget*>(owner->field_0x220);
    if (!track)
        return;
    UnkCoreXFlags_A2_b73* flags = shape->field_0x50;
    if (!flags)
        return;
    UnkCoreXLink_A2_b73* link = &state->link;
    link->context = self;
    link->callback = fn_71004db7f8;
    if (UnkWeakHandle* handle = state->handle) {
        if (UnkWeakTarget* t = handle->target)
            fn_710117b3f0(t, link);
        if (UnkWeakHandle* p = state->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            state->handle = nullptr;
        }
    }
    asm("" : "+r"(state));
    fn_7100316018(track, &state->handle);
    fn_710117b3e8(state->handle ? state->handle->target : nullptr, link);
    if (flags->field_0x11c)
        flags->field_0x114 = true;
}
