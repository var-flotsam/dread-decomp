// In the CSamusMagnetGloveMovementState::CParams unit: fn_71009a9434 builds the fall request (the reflected
// params with the wall or ceiling track id), lets the current CCharClassTweakFallActionTrack fill it in, forces
// flag bit 0 and the time descriptor g_unk_710164b968, and passes it to the target at +0x8 (fn_710098ae04).
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;
extern UnkDesc_SpitSend g_unk_7172769740;
extern UnkDesc_SpitSend g_unk_710164b968;
extern void* g_unk_7101a22cb8[];
extern char g_unk_7101a22ce8[];
struct UnkAirTarget_B71a6;
struct UnkAirRequest_B71a6;
void fn_710098ae04(UnkAirTarget_B71a6* target, UnkAirRequest_B71a6* req);

struct UnkTrackTime_ovb0082 {
    u64 key;
    u64 time;
};
struct UnkTrackBlend_ovb0082 {
    u32 a, b, c;
};

struct UnkTrackParams_ovb0082 {
    s64 id;
    UnkTrackTime_ovb0082 time;
    UnkTrackBlend_ovb0082 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkTrackParams_ovb0082(UnkTrackTime_ovb0082 time_, UnkTrackBlend_ovb0082 blend_,
                                                          u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkTrackParams_ovb0082()
        : UnkTrackParams_ovb0082(reinterpret_cast<const UnkTrackTime_ovb0082&>(g_unk_710164b958),
                                 reinterpret_cast<const UnkTrackBlend_ovb0082&>(g_unk_7172769740),
                                 base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

struct UnkTrackRecord_ovb0082 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkTrackParams_ovb0082* params);  // fills in the request
};

// The request passed to the movement: the reflected params base (vtable g_unk_7101a22cb8) holding the track
// request, then a derived part (vtable g_unk_7101a22ce8).
struct UnkParamsVtable_ovb0082 {
    void* vtable;
    __attribute__((always_inline)) UnkParamsVtable_ovb0082() { vtable = g_unk_7101a22cb8 + 2; }
};

struct UnkTrackId_ovb0082 {
    u64 hash;
    UnkTrackId_ovb0082(u64 h) : hash(h) {}
    UnkTrackId_ovb0082(const UnkTrackId_ovb0082& other) : hash(other.hash) {}
};

struct UnkFallRequest_ovb0082 : UnkParamsVtable_ovb0082 {
    UnkTrackParams_ovb0082 params;  // 0x8
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;

    __attribute__((always_inline)) UnkFallRequest_ovb0082() {
        field_0x38 = 3;
        __builtin_memset(&field_0x3c, 0, 8);
        field_0x44 = 0;
        vtable = g_unk_7101a22ce8 + 0x10;
    }
};

struct UnkTrackOwner_ovb0082 {
    u8 unk_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkMagnetFallState_ovb0082 {
    u8 unk_0x0[0x8];
    UnkAirTarget_B71a6* target;  // 0x8
    u8 unk_0x10[0x20];
    UnkTrackOwner_ovb0082* owner;  // 0x30
    u8 unk_0x38[0x188];
    s32 surface;  // 0x1c0
};

void fn_71009a9434(UnkMagnetFallState_ovb0082* self) {
    UnkTrackId_ovb0082 ceiling(0x519ba7a1e35e9102);
    UnkTrackId_ovb0082 wall(0xbf7cd046ae83b513);
    UnkFallRequest_ovb0082 req;
    req.params.id = (self->surface == 0 ? ceiling : wall).hash;
    UnkTrackQuery query = {fn_7100d2fa08(), nullptr, -1.0f};
    fn_710117b08c(self->owner->animation, &query);
    u32 flags;
    if (query.result) {
        reinterpret_cast<UnkTrackRecord_ovb0082*>(query.result)->vf_0x18(&req.params);
        flags = req.params.flags | 1;
    } else {
        flags = 1;
    }
    req.field_0x40 = 0;
    req.params.time = reinterpret_cast<const UnkTrackTime_ovb0082&>(g_unk_710164b968);
    req.params.flags = flags;
    fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&req));
}
