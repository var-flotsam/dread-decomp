// The CGoliathXBurstProjectionBombsAttack unit's update: after the base update, counts the timer at
// +0x60 down; when it runs out and the flag at +0x68 is set, restarts it from the value at +0xf0 of
// the object at +0x28 and sends a fixed request with mode 0 to the owner's sub-object (+0x220 of the
// owner at +0x8), descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"


struct UnkGoliathXBombsOwner_b57a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkGoliathXBombsData_b57a3 {
    u8 pad_0x0[0xf0];
    f32 field_0xf0;
};

struct UnkGoliathXBombsAttack_b57a3 {
    u8 pad_0x0[0x8];
    UnkGoliathXBombsOwner_b57a3* owner;
    u8 pad_0x10[0x18];
    UnkGoliathXBombsData_b57a3* field_0x28;
    u8 pad_0x30[0x30];
    f32 field_0x60;
    u8 pad_0x64[4];
    u32 field_0x68;
};

void fn_71005847a4(UnkGoliathXBombsAttack_b57a3* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    f32 t = self->field_0x60;
    if (!(t > 0.0f))
        return;
    t -= dt;
    self->field_0x60 = t;
    if (!(t <= 0.0f))
        return;
    if (!self->field_0x68)
        return;
    self->field_0x60 = self->field_0x28->field_0xf0;
    void* target = self->owner->field_0x220;
    if (target)
        UnkSpitSend(target, 0xf2068194674bde3b, 0xf4061ff25a2163d6);
}

// The unit's event handler: for a kind-1 event whose id hash (+0x8 -> +0x10 -> +0x38 -> +0x30) is
// 0x800830e9bd22c437, or which isn't the hash above with +0x68 clear and fn_710117af10 finds one of
// the two names below in the owner's sub-object, restarts the timer at +0x60 from the value at
// +0xec of the object at +0x28; the hash above with +0x68 clear sends 0x46463586a994ec9b with mode 1.
// Always returns true.
struct UnkStruct_710117af10;
struct UnkCls_710117af10;
extern UnkCls_710117af10* g_unk_71726e55b8 __attribute__((visibility("hidden")));
extern UnkCls_710117af10* g_unk_71726e55c0 __attribute__((visibility("hidden")));

struct UnkGoliathXBombsIdNode_b57a3 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkGoliathXBombsIdHolder_b57a3 {
    u8 pad_0x0[0x38];
    UnkGoliathXBombsIdNode_b57a3* node;
};

struct UnkGoliathXBombsIdRef_b57a3 {
    u8 pad_0x0[0x10];
    UnkGoliathXBombsIdHolder_b57a3* holder;
};

struct UnkGoliathXBombsEvent_b57a3 {
    s32 kind;
    u8 pad_0x4[4];
    UnkGoliathXBombsIdRef_b57a3* ref;
};

struct UnkGoliathXBombsData2_b57a3 {
    u8 pad_0x0[0xec];
    f32 field_0xec;
};

struct UnkGoliathXBombsAttack2_b57a3 {
    u8 pad_0x0[0x8];
    UnkGoliathXBombsOwner_b57a3* owner;
    u8 pad_0x10[0x18];
    UnkGoliathXBombsData2_b57a3* field_0x28;
    u8 pad_0x30[0x30];
    f32 field_0x60;
    u8 pad_0x64[4];
    u32 field_0x68;
};

bool fn_7100585c98(UnkGoliathXBombsAttack2_b57a3* self, UnkGoliathXBombsEvent_b57a3* event) {
    void* target = self->owner->field_0x220;
    if (!target || event->kind != 1)
        return true;
    u64 hash = event->ref->holder->node->hash;
    if (hash != 0x800830e9bd22c437) {
        if (hash == 0xf2068194674bde3b && !self->field_0x68) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, 0x46463586a994ec9b, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
            return true;
        }
        UnkStruct_710117af10* t = static_cast<UnkStruct_710117af10*>(target);
        if (!fn_710117af10(t, &g_unk_71726e55b8, false) && !fn_710117af10(t, &g_unk_71726e55c0, false))
            return true;
    }
    self->field_0x60 = self->field_0x28->field_0xec;
    return true;
}
