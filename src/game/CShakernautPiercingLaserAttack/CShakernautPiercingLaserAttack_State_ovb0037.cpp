// CShakernautPiercingLaserAttack's unit (tools/tu_map.py). Two identical constructors on the attack base
// constructor fn_710036069c; the laser's start toward the side the AI component (+0x240, its facing at
// +0x14810) faces (fn_71006a8e94: the owner's +0x278 player stops, then plays the entry at +0x146d0 with
// the "right_" or "left_lasertogroundshock_init" id); the copy assignment (the listener at +0x38 shares
// its owner's weak reference unless assigned to itself, the words at +0x74 copied one by one); and the
// state step (fn_71006a8fd8: state 1 plays one of two ids, chosen by +0xb0, and moves to 2; 2 and 4 hand
// over to fn_71006a7448 with 3 and 5; 7 plays a third id). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIAttackComponent.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "types.h"

void fn_710036069c(void* self);
extern void* g_unk_7101a62b48[];
extern void* g_unk_7101a22d48[];

struct UnkShakernautVec3_ovb0037 {
    f32 x, y, z;
};

struct UnkShakernautPiercingLaserAttack_ovb0037 {
    void** vtable;
    u8 pad_0x8[0x20];
    u64 field_0x28;
    u32 field_0x30;
    u8 pad_0x34[0x4];
    void** listener_vtable;  // 0x38
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u8 pad_0x58[0x1c];
    UnkShakernautVec3_ovb0037 field_0x74;
    UnkShakernautVec3_ovb0037 field_0x80;
    UnkShakernautVec3_ovb0037 field_0x8c;
    u8 pad_0x98[0x4];
    UnkShakernautVec3_ovb0037 field_0x9c;
};
static_assert(__builtin_offsetof(UnkShakernautPiercingLaserAttack_ovb0037, field_0x9c) == 0x9c, "layout");

__attribute__((always_inline)) static inline void Construct_ovb0037(UnkShakernautPiercingLaserAttack_ovb0037* self) {
    fn_710036069c(self);
    self->vtable = g_unk_7101a62b48 + 2;
    self->field_0x28 = 0;
    self->field_0x30 = 0;
    self->listener_vtable = g_unk_7101a22d48 + 2;
    __builtin_memset(&self->field_0x40, 0, 24);
    self->field_0x74 = {0.0f, 0.0f, 0.0f};
    self->field_0x80 = {0.0f, 0.0f, 0.0f};
    self->field_0x8c = {0.0f, 0.0f, 0.0f};
    self->field_0x9c = {0.0f, 0.0f, 0.0f};
}

void fn_71006a6de4(UnkShakernautPiercingLaserAttack_ovb0037* self) {
    Construct_ovb0037(self);
}

void fn_71006a9400(UnkShakernautPiercingLaserAttack_ovb0037* self) {
    Construct_ovb0037(self);
}

using base::global::CStrId;
using base::math::CVector2D;

struct UnkCommanderPlayer_b67a2;

struct UnkShakernautPlayer_ovb0037 {
    void** vtable;

    void vf_0x178() { reinterpret_cast<void (*)(UnkShakernautPlayer_ovb0037*)>(vtable[0x178 / 8])(this); }
};

struct UnkShakernautOwner_ovb0037 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x30];
    UnkShakernautPlayer_ovb0037* player;  // 0x278
};

struct UnkShakernautLaserAttack_ovb0037 {
    u8 pad_0x0[0x8];
    UnkShakernautOwner_ovb0037* owner;  // 0x8
};

void fn_71006a8e94(UnkShakernautLaserAttack_ovb0037* self) {
    base::reflection::UnkReflObject* ai = self->owner->ai;
    char* cast = ai && fn_710009d50c(ai->vf_0x0(), fn_71006ad0a4()) ? reinterpret_cast<char*>(ai) : nullptr;
    self->owner->player->vf_0x178();
    const CVector2D* facing = reinterpret_cast<const CVector2D*>(cast + 0x14810);
    const CStrId* id;
    if (facing->x + facing->y * 0.0f > 0.0f) {
        static CStrId s_right("right_lasertogroundshock_init", true);
        id = &s_right;
    } else {
        static CStrId s_left("left_lasertogroundshock_init", true);
        id = &s_left;
    }
    fn_7101134420(reinterpret_cast<UnkCommanderPlayer_b67a2*>(self->owner->player), cast + 0x146d0, 0, 0,
                  const_cast<CStrId*>(id), -1.0f);
}

struct UnkShakernautListener_ovb0037 {
    virtual ~UnkShakernautListener_ovb0037();
    u64 field_0x8;
    u64 field_0x10;
    UnkWeakHandle* owner;

    __attribute__((always_inline)) UnkShakernautListener_ovb0037& operator=(const UnkShakernautListener_ovb0037& other) {
        __builtin_memcpy(&field_0x8, &other.field_0x8, 0x10);
        if (this != &other)
            CopyWeakRef(&owner, other.owner);
        return *this;
    }
};

// Words copied one by one.
struct UnkShakernautWords_ovb0037 {
    u32 w[13];

    UnkShakernautWords_ovb0037& operator=(const UnkShakernautWords_ovb0037& other) {
        for (int i = 0; i < 13; i++)
            w[i] = other.w[i];
        return *this;
    }
};

struct UnkShakernautPiercingLaserAttackAssign_ovb0037 : CAttack {
    ~UnkShakernautPiercingLaserAttackAssign_ovb0037() override;
    u64 field_0x28;
    u32 field_0x30;
    UnkShakernautListener_ovb0037 field_0x38;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    u32 field_0x70;
    UnkShakernautWords_ovb0037 field_0x74;
    u64 field_0xa8;
    u64 field_0xb0;
    u64 field_0xb8;
    __attribute__((always_inline)) UnkShakernautPiercingLaserAttackAssign_ovb0037& operator=(
        const UnkShakernautPiercingLaserAttackAssign_ovb0037&) = default;
};
static_assert(__builtin_offsetof(UnkShakernautPiercingLaserAttackAssign_ovb0037, field_0xb8) == 0xb8, "layout");

UnkShakernautPiercingLaserAttackAssign_ovb0037* fn_71006a977c(UnkShakernautPiercingLaserAttackAssign_ovb0037* self,
                                                              const UnkShakernautPiercingLaserAttackAssign_ovb0037* other) {
    *self = *other;
    return self;
}

extern CStrId g_unk_7172705c70 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705c78 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705ce8 __attribute__((visibility("hidden")));

struct UnkShakernautLaserState_ovb0037 {
    u8 pad_0x0[0x30];
    s32 state;  // 0x30
    u8 pad_0x34[0x7c];
    s32 field_0xb0;

    void SetState(s32 s) {
        if (state != s)
            state = s;
    }
};
static_assert(__builtin_offsetof(UnkShakernautLaserState_ovb0037, field_0xb0) == 0xb0, "layout");

void fn_71006a7afc(UnkShakernautLaserState_ovb0037* self, CStrId id, bool flag);
void fn_71006a7448(UnkShakernautLaserState_ovb0037* self, s32 state);

void fn_71006a8fd8(UnkShakernautLaserState_ovb0037* self) {
    switch (self->state) {
    case 1:
        if (self->field_0xb0 == 1)
            fn_71006a7afc(self, g_unk_7172705c78, true);
        else
            fn_71006a7afc(self, g_unk_7172705c70, true);
        self->SetState(2);
        break;
    case 2:
        fn_71006a7448(self, 3);
        break;
    case 4:
        fn_71006a7448(self, 5);
        break;
    case 7:
        fn_71006a7afc(self, g_unk_7172705ce8, true);
        break;
    }
}
