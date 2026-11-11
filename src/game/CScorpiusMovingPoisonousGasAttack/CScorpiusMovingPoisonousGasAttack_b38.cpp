// CScorpiusMovingPoisonousGasAttack: the start and the end of the attack. Both hand the owner's
// target (+0x298) a kind-5 parameter block facing along the third column of the owner's matrix; the
// start also listens (the link at +0x78, calling fn_71006895a0) to the owner's track (+0x220)
// through its weak handle at +0x90, which the end drops. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CScorpiusAttack.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The kind-5 block (game_s2/UnkParams.h) with its first fields seen as words.
struct UnkGasParams {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkGasLink {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkGasAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x60];
    u32 field_0x70;
    u8 pad_0x74[0x4];
    UnkGasLink link;  // 0x78
    UnkWeakHandle* handle;  // 0x90
};

void fn_7100689d84(UnkGasAttack* self);
bool fn_7100689c0c(UnkGasAttack* self);
void fn_7100689ea8(UnkGasAttack* self);
void fn_71006895a0();
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);

// The bit pattern of a float in memory (a word load, as the original reads it).
static inline u32 AsU32(const f32& value) {
    u32 bits;
    __builtin_memcpy(&bits, &value, 4);
    return bits;
}

static inline void DropHandle(UnkGasAttack* self, UnkGasLink* link) {
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
}

void fn_71006885e8(UnkGasAttack* self) {
    fn_7100689d84(self);
    self->field_0x70 = fn_7100689c0c(self);
    UnkActor* owner = self->owner;
    if (void* target = owner->field_0x298) {
        UnkGasParams params;
        __builtin_memset(&params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
        u32 x = AsU32(m.m[2]);
        u32 y = AsU32(m.m[6]);
        u32 z = AsU32(m.m[10]);
        params.field_0x8 = x;
        params.field_0xc = y;
        params.field_0x10 = z;
        params.field_0x14 = 1;
        fn_71006f4b0c(target, &params, 0);
    }
    UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
    UnkGasLink* link = &self->link;
    link->context = self;
    link->callback = fn_71006895a0;
    DropHandle(self, link);
    if (track) {
        // The slot is addressed by its offset.
        fn_7100316018(track, reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<char*>(self) + 0x90));
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
}

// Hands the target a kind-5 block facing along the owner's third matrix column.
__attribute__((always_inline)) static inline void SendFacing(UnkActor* owner, void* target) {
    UnkGasParams params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x30 = 0;
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    __builtin_memset(&params.field_0x8, 0, 13);
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    u32 x = AsU32(m.m[2]);
    u32 y = AsU32(m.m[6]);
    u32 z = AsU32(m.m[10]);
    params.field_0x8 = x;
    params.field_0xc = y;
    params.field_0x10 = z;
    params.field_0x14 = 1;
    fn_71006f4b0c(target, &params, 0);
}

void fn_710068872c(UnkGasAttack* self) {
    fn_7100689ea8(self);
    DropHandle(self, &self->link);
    UnkActor* owner = self->owner;
    if (!owner)
        return;
    if (void* target = owner->field_0x298)
        SendFacing(owner, target);
}

extern __attribute__((visibility("hidden"))) char g_unk_7172703308[];

// In state 4, when the track allows the attack to succeed: fn_7100686960 with the name at
// g_unk_7172703308, then the facing block.
void fn_7100688504(UnkGasAttack* self) {
    if (self->field_0x70 != 4)
        return;
    UnkAnimationComponent* track = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (!track || !fn_710117b348(track, fn_7100c4a358()))
        return;
    fn_7100686960(self, g_unk_7172703308, 0, -1.0f, -1.0f);
    UnkActor* owner = self->owner;
    if (!owner)
        return;
    if (void* target = owner->field_0x298)
        SendFacing(owner, target);
}
