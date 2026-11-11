// CSamusGroundMovementState::CParams' unit (tools/tu_map.py): the ground state's default step. A
// default request (the shape of fn_7100990e14's, src/game/CSamusCrouchMovementState/CParams/
// CParams_Request_ovb0131.cpp, with vtable g_unk_7101a22ce8) goes to the movement (fn_71009ed058);
// when it takes it, the owner gets one of two keys (+0x3a8 of the movement set), the target at +0x8
// the request (fn_710098ae04), and the movement's flags at +0x2324/+0x2325 are set when fn_71010d1598
// or fn_71010d15ac holds. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;
extern const u64 g_unk_710164b380;  // the key while fn_7100df5874 holds
extern const u64 g_unk_710164b388;
struct UnkAirTarget_B71a6;
struct UnkAirRequest_B71a6;
void fn_710098ae04(UnkAirTarget_B71a6* target, UnkAirRequest_B71a6* req);
struct UnkPlayer;

struct UnkGroundTrackTime_ovb0138 {
    u64 key;
    u64 time;
};
struct UnkGroundTrackBlend_ovb0138 {
    u32 a, b, c;
};

struct UnkGroundTrackParams_ovb0138 {
    s64 id;
    UnkGroundTrackTime_ovb0138 time;
    UnkGroundTrackBlend_ovb0138 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkGroundTrackParams_ovb0138(UnkGroundTrackTime_ovb0138 time_,
                                                                UnkGroundTrackBlend_ovb0138 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkGroundTrackParams_ovb0138()
        : UnkGroundTrackParams_ovb0138(reinterpret_cast<const UnkGroundTrackTime_ovb0138&>(g_unk_710164b958),
                                       reinterpret_cast<const UnkGroundTrackBlend_ovb0138&>(g_unk_7172769740),
                                       base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

struct UnkGroundParamsVtable_ovb0138 {
    void* vtable;
    __attribute__((always_inline)) UnkGroundParamsVtable_ovb0138() { vtable = g_unk_7101a22cb8 + 2; }
};

struct UnkGroundRequest_ovb0138 : UnkGroundParamsVtable_ovb0138 {
    UnkGroundTrackParams_ovb0138 params;  // 0x8
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;

    __attribute__((always_inline)) UnkGroundRequest_ovb0138() {
        field_0x38 = 3;
        __builtin_memset(&field_0x3c, 0, 8);
        field_0x44 = 0;
        vtable = g_unk_7101a22ce8 + 0x10;
    }
};

struct UnkGroundMovement_ovb0138 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
    u8 pad_0x3a9[0x2324 - 0x3a9];
    u8 field_0x2324;
    u8 field_0x2325;
};

struct UnkGroundState_ovb0138 {
    u8 pad_0x0[0x8];
    UnkAirTarget_B71a6* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkActor* owner;                       // 0x30
    UnkGroundMovement_ovb0138* movement;  // 0x38
};

bool fn_71009ed058(UnkGroundMovement_ovb0138* movement, UnkGroundRequest_ovb0138* req);
void fn_71009ea070(UnkGroundMovement_ovb0138* movement);
struct UnkGroundPlayer_b52;
bool fn_71010d1598(UnkGroundPlayer_b52*);
bool fn_71010d15ac(UnkGroundMovement_ovb0138* movement);

struct UnkStandState_ovb0103;

// Declared as src/game/CSamusStandMovementState/CParams/CParams_Morph_ovb0103.cpp declares it.
void fn_710099d8f4(UnkStandState_ovb0103* object) {
    UnkGroundState_ovb0138* self = reinterpret_cast<UnkGroundState_ovb0138*>(object);
    UnkGroundRequest_ovb0138 req;
    if (!fn_71009ed058(self->movement, &req))
        return;
    if (self->movement->field_0x3a8) {
        UnkActor* owner = self->owner;
        bool alt = fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
        fn_7100d7c828(owner, alt ? &g_unk_710164b380 : &g_unk_710164b388, false);
    }
    fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&req));
    fn_71009ea070(self->movement);
    if (fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(self->movement)) || fn_71010d15ac(self->movement)) {
        // Byte stores (the original reloads the movement between them).
        __builtin_memset(&self->movement->field_0x2324, 1, 1);
        __builtin_memset(&self->movement->field_0x2325, 1, 1);
    }
}
