// CChozoZombieXPoisonClawsAttack's unit (tools/tu_map.py unit 735): the attack's start and the length it
// reads from its animation. Names and layouts are placeholders.
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"


// As src/game/CInfesterReloadAttack/CInfesterReloadAttack_Start_A3_b73.cpp declares them.
struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_710049f42c();

struct UnkPoisonClawsLink_ovb0122 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

static inline UnkWeakTarget*& Track_ovb0122(char* owner) {
    return *reinterpret_cast<UnkWeakTarget**>(owner + 0x220);
}

struct UnkPoisonClawsStart_ovb0122 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u8 pad_0x34[0x4];
    UnkPoisonClawsLink_ovb0122 link;  // 0x38
    UnkWeakHandle* handle;           // 0x50
    UnkWeakTarget* field_0x58;
};

// The attack's start: after the base (fn_71003608ac) and with its step at 1, keeps the object the owner
// (+0x8) tracks (+0x220) at +0x58 and hooks the callback fn_710049f42c (link at +0x38, handle at +0x50) up
// to it, as fn_71005c54e4 does; then hands the owner's navigation object (+0x298) a kind-5 parameter block
// with the flags at +0x38 and +0x39 set.
void fn_710049e6b8(UnkPoisonClawsStart_ovb0122* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x30 = 1;
    self->field_0x58 = Track_ovb0122(self->owner);
    if (Track_ovb0122(self->owner)) {
        UnkPoisonClawsLink_ovb0122* link = &self->link;
        link->context = self;
        link->callback = fn_710049f42c;
        UnkWeakTarget* track = Track_ovb0122(self->owner);
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        if (track) {
            asm("" : "+r"(self));
            fn_7100316018(track, &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
    }
    void* nav = *reinterpret_cast<void**>(self->owner + 0x298);
    UnkParams params;
    params.field_0x30 = 0;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    params.pad_0x38 = 1;
    params.field_0x39 = 1;
    fn_71006f4b0c(nav, &params, 1);
}


// As src/game/CTakumakuAIComponent/CTakumakuAIComponent_Layer_ovb0012.cpp declares it (the key is a hash).
struct UnkStruct_7101176ea8;
void* fn_7101176d34(UnkStruct_7101176ea8* self, void* arg);

// As src/game/CSamusFloorSlideMovementState/CParams/CParams_Slide_ovb0004.cpp defines them.
struct UnkFlags_71007142e8 {
    u32 bits;
    UnkFlags_71007142e8() : bits(0) {}
    UnkFlags_71007142e8(u32 b) : bits(b) {}
    UnkFlags_71007142e8(const UnkFlags_71007142e8& o) : bits(o.bits) {}
};

inline const UnkFlags_71007142e8& UnkFlagsNone_71007142e8() {
    static UnkFlags_71007142e8 s_none;
    return s_none;
}

inline const UnkFlags_71007142e8& UnkFlagsAll_71007142e8() {
    static UnkFlags_71007142e8 s_all(~UnkFlagsNone_71007142e8().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

void fn_71007142e8(UnkStruct_71007142e8* self);
void fn_7100b8b3bc(UnkTimeline* self, void* target, UnkMotionTracks* tracks, const u32* flags, f32 from,
                   f32 to);

struct UnkPoisonClawsOwner_ovb0122 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
};

struct UnkPoisonClawsAttack_ovb0122 {
    void** vtable;
    UnkPoisonClawsOwner_ovb0122* owner;  // 0x8
    u8 pad_0x10[0x50];
    f32 field_0x60;
};

// The value at +0x2c of the motion of the animation 0x5c1162fdbc776b27 over its whole length
// (fn_7100b8b3bc, all channels) goes to +0x60, when the owner's animation component (+0x220) has it.
void fn_710049e578(UnkPoisonClawsAttack_ovb0122* self) {
    UnkStruct_7101176ea8* animation = self->owner->animation;
    if (!animation)
        return;
    UnkTimeline* timeline =
        static_cast<UnkTimeline*>(fn_7101176d34(animation, reinterpret_cast<void*>(0x5c1162fdbc776b27ull)));
    if (!timeline)
        return;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    self->field_0x60 = motion.field_0x2c;
}
