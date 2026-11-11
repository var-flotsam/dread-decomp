// Unit 1118 (no class hint): an AI state object. Names and layouts are placeholders.
#include "game/UnkAIComponent.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

void fn_71002adc8c(void* member);

struct UnkStateOwnerSub_b0169 {
    u8 pad_0x0[0x1f0];
    u8 flag;  // 0x1f0
};

struct UnkStateOwner_b0169 {
    u8 pad_0x0[0x1f0];
    UnkStateOwnerSub_b0169* sub;  // 0x1f0
};

struct __attribute__((packed)) UnkPacked64_b0169 {
    u64 value;
};

// An object the state asks whether it is ready.
struct UnkReadyObject_b0169 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
};

struct UnkAIState_b0169 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x74];
    s32 field_0xac;
    s32 field_0xb0;
    s32 field_0xb4;
    u8 pad_0xb8[4];
    s32 field_0xbc;
    u8 pad_0xc0[8];
    u8 field_0xc8;
    u8 pad_0xc9[0xb];
    UnkPacked64_b0169 field_0xd4;
    UnkPacked64_b0169 field_0xdc;
    UnkPacked64_b0169 field_0xe4;
    u8 pad_0xec[0x4c];
    UnkReadyObject_b0169* field_0x138;
    u8 field_0x140;
    u8 pad_0x141[0xc7];
    u8 member_0x208[8];
};
static_assert(__builtin_offsetof(UnkAIState_b0169, field_0x138) == 0x138, "layout");
static_assert(__builtin_offsetof(UnkAIState_b0169, member_0x208) == 0x208, "layout");

// Resets the tracked values and the owner's flag.
void fn_71006fce60(UnkAIState_b0169* self) {
    self->field_0xac = 0x80000001;
    self->field_0xb4 = 0x80000001;
    self->field_0xbc = 0;
    self->field_0xc8 = 0;
    self->field_0xd4.value = 0;
    self->field_0xdc.value = 0;
    self->field_0xe4.value = 0;
    fn_71002adc8c(self->member_0x208);
    UnkStateOwnerSub_b0169* sub = reinterpret_cast<UnkStateOwner_b0169*>(self->owner)->sub;
    if (sub)
        sub->flag = 0;
}

struct UnkUnit1119Event_b54;
u64 fn_71007038f4(void*, void*, bool);
void fn_7100387b8c(void* member, void* event, bool flag);
void fn_71006fc7c4(void* self, void* arg);

struct UnkStateEvent_b0169 {
    u8 pad_0x0[0x34];
    u32 kind;  // 0x34
    UnkReadyObject_b0169* object;  // 0x38
    u8 active;  // 0x40
};

// Hands the event to the base state and the member at +0x100; an active event of kind 2 or 4 whose object is
// ready resets the tracked values and, when the owner's navigation points at this state, restarts it.
void fn_71006fe894(char* self_, UnkUnit1119Event_b54* event_, bool flag) {
    UnkAIState_b0169* self = reinterpret_cast<UnkAIState_b0169*>(self_);
    UnkStateEvent_b0169* event = reinterpret_cast<UnkStateEvent_b0169*>(event_);
    fn_71007038f4(self, event, flag);
    if (event->kind <= 4 && ((1u << event->kind) & 0x15)) {
        fn_7100387b8c(self_ + 0x100, event, flag);
        if (event->kind == 4 || event->kind == 2) {
            if (event->active && event->object && event->object->vf_0x20()) {
                self->field_0xac = 0x80000001;
                self->field_0xb0 = 0x80000001;
                self->field_0xb4 = 0x80000001;
                fn_71002adc8c(self->member_0x208);
                void* current = nullptr;
                if (self->owner && self->owner->field_0x240) {
                    if (UnkNavigation* nav = fn_71010fccf8(static_cast<UnkAIComponent*>(self->owner->field_0x240))) {
                        void** list = static_cast<void**>(nav->field_0xb0);
                        if (*list)
                            current = *reinterpret_cast<void**>(static_cast<u8*>(*list) + 0x10);
                    }
                }
                if (current == self) {
                    UnkReadyObject_b0169* target = nullptr;
                    if (self->field_0x140) {
                        UnkReadyObject_b0169* object = self->field_0x138;
                        if (object && object->vf_0x20())
                            target = object;
                    }
                    fn_71006fc7c4(self, target);
                }
            }
        }
    }
}

struct UnkUnit1124Object;
void fn_7100703814(UnkUnit1124Object*, u32);
void fn_71002acfe4(void*);
extern void* g_unk_7101a6b910[];
extern void* g_unk_7101a264d0[];

struct UnkAIStateInit_b0169 {
    void* vtable;  // 0x0
    u8 pad_0x8[0xa4];
    s32 field_0xac;  // 0xac
    u8 pad_0xb0[0x4];
    s32 field_0xb4;  // 0xb4
    u8 pad_0xb8[0x4];
    u8 zero_0xbc[13];  // 0xbc
    u8 pad_0xc9[0x3];
    u8 zero_0xcc[48];  // 0xcc
    u8 pad_0xfc[0x4];
    void* vtable_0x100;  // 0x100
    u8 zero_0x108[13];  // 0x108
    u8 pad_0x115[0x7];
    u8 field_0x11c;  // 0x11c
    u8 pad_0x11d[0x3];
    u8 zero_0x120[16];  // 0x120
    u8 field_0x130;  // 0x130
    u8 pad_0x131[0x3];
    s32 field_0x134;  // 0x134
    u8 pad_0x138[0x8];
    u8 flag_0x140;  // 0x140
    u8 pad_0x141[0x7];
    u8 flag_0x148;  // 0x148
    u8 pad_0x149[0x7];
    u8 flag_0x150;  // 0x150
    u8 pad_0x151[0x7];
    u8 flag_0x158;  // 0x158
    u8 pad_0x159[0x7];
    u8 flag_0x160;  // 0x160
    u8 pad_0x161[0x7];
    u8 flag_0x168;  // 0x168
    u8 pad_0x169[0x7];
    u8 flag_0x170;  // 0x170
    u8 pad_0x171[0x7];
    u8 flag_0x178;  // 0x178
    u8 pad_0x179[0x3];
    u8 zero_0x17c[13];  // 0x17c
    u8 pad_0x189[0x7];
    u8 flag_0x190;  // 0x190
    u8 pad_0x191[0x1];
    u8 flag_0x192;  // 0x192
    u8 pad_0x193[0x1];
    u8 flag_0x194;  // 0x194
    u8 pad_0x195[0x7];
    u8 flag_0x19c;  // 0x19c
    u8 pad_0x19d[0x1];
    u8 flag_0x19e;  // 0x19e
    u8 pad_0x19f[0x1];
    u8 flag_0x1a0;  // 0x1a0
    u8 pad_0x1a1[0x7];
    u8 zero_0x1a8[16];  // 0x1a8
    u8 flag_0x1b8;  // 0x1b8
    u8 pad_0x1b9[0x7];
    u8 zero_0x1c0[16];  // 0x1c0
    u8 flag_0x1d0;  // 0x1d0
    u8 pad_0x1d1[0x7];
    u8 zero_0x1d8[16];  // 0x1d8
    s32 field_0x1e8;  // 0x1e8
    u8 field_0x1ec;  // 0x1ec
    u8 pad_0x1ed[0x3];
    s32 field_0x1f0;  // 0x1f0
    u8 pad_0x1f4[0x4];
    base::UnkAllocator* allocator;  // 0x1f8
    u8 flag_0x200;  // 0x200
    u8 pad_0x201[0x1];
    u8 flag_0x202;  // 0x202
    u8 pad_0x203[0x1];
    u8 flag_0x204;  // 0x204
    u8 pad_0x205[0x1];
    u8 flag_0x206;  // 0x206
    u8 pad_0x207[0x1];
    u8 member_0x208[8];  // 0x208
    u8 pad_0x210[0x12a80];
    u8 zero_0x12c90[13];  // 0x12c90
    u8 pad_0x12c9d[0x3];
    u32 field_0x12ca0;  // 0x12ca0
};
static_assert(__builtin_offsetof(UnkAIStateInit_b0169, member_0x208) == 0x208, "layout");

// Constructs the state: the base part, then the tracked values, the member at +0x100 and the flag tables.
void fn_71006fc4fc(UnkAIStateInit_b0169* self, u32 arg) {
    fn_7100703814(reinterpret_cast<UnkUnit1124Object*>(self), arg);
    self->field_0xb4 = 0x80000001;
    self->field_0xac = 0x80000001;
    self->vtable = reinterpret_cast<char*>(g_unk_7101a6b910) + 0x10;
    self->field_0x11c = 0;
    self->field_0x134 = 0;
    __builtin_memset(self->zero_0x108, 0, 13);
    self->field_0x130 = 0;
    __builtin_memset(self->zero_0xbc, 0, 13);
    __builtin_memset(self->zero_0xcc, 0, 0x30);
    __builtin_memset(self->zero_0x120, 0, 16);
    self->vtable_0x100 = reinterpret_cast<char*>(g_unk_7101a264d0) + 0x10;
    self->field_0x1e8 = 10;
    self->flag_0x140 = 0;
    self->flag_0x148 = 0;
    self->flag_0x150 = 0;
    self->flag_0x158 = 0;
    self->flag_0x160 = 0;
    self->flag_0x168 = 0;
    self->flag_0x170 = 0;
    self->flag_0x178 = 0;
    self->flag_0x190 = 0;
    self->flag_0x192 = 0;
    self->flag_0x194 = 0;
    self->flag_0x19c = 0;
    self->flag_0x19e = 0;
    self->flag_0x1a0 = 0;
    __builtin_memset(self->zero_0x1d8, 0, 16);
    __builtin_memset(self->zero_0x17c, 0, 13);
    self->flag_0x1b8 = 0;
    self->flag_0x1d0 = 0;
    __builtin_memset(self->zero_0x1a8, 0, 16);
    __builtin_memset(self->zero_0x1c0, 0, 16);
    self->field_0x1ec = 1;
    self->field_0x1f0 = 0xf;
    self->allocator = fn_7100083990();
    self->flag_0x200 = 0;
    self->flag_0x202 = 0;
    self->flag_0x204 = 0;
    self->flag_0x206 = 0;
    self->field_0x134 = 2;
    fn_71002acfe4(self->member_0x208);
    __builtin_memset(self->zero_0x12c90, 0, 13);
    self->field_0x12ca0 = 0x7f7fffff;
}

// The owner actor as the state sees its position and mover.
struct UnkMoverActor_b0169 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0(base::math::CVector3D* target, bool flag);
    u8 pad_0x8[0x3c];
    f32 x_0x44;
    u8 pad_0x48[0xc];
    f32 y_0x54;
    u8 pad_0x58[0xc];
    f32 z_0x64;
};

struct UnkLimitSource_b0169 {
    u8 pad_0x0[0x64];
    f32 limit;  // 0x64
};

struct UnkAIStateDist_b0169 {
    u8 pad_0x0[0x30];
    UnkMoverActor_b0169* actor;  // 0x30
    u8 pad_0x38[0x18];
    f32 field_0x50;
    u8 pad_0x54[0x14];
    UnkLimitSource_b0169* source;  // 0x68
    u8 pad_0x70[0x64];
    f32 target_x;  // 0xd4
    f32 target_y;  // 0xd8
    u8 pad_0xdc[8];
    f32 dir_x;  // 0xe4
    f32 dir_y;  // 0xe8
    u8 pad_0xec[0x60];
    f32 own_limit;  // 0x14c
    u8 use_own_limit;  // 0x150
    u8 pad_0x151[0xb2];
    u8 flag_0x203;
    u8 flag_0x204;
    u8 pad_0x205[0x12a9b];
    f32 nearest;  // 0x12ca0
};
static_assert(__builtin_offsetof(UnkAIStateDist_b0169, nearest) == 0x12ca0, "layout");

// Whether the target is within the limit (or already passed in the opposite direction); a target that is
// passed is handed back to the actor, otherwise the nearest distance seen is tracked.
bool fn_71006fda48(UnkAIStateDist_b0169* self) {
    const f32* source_limit = &self->source->limit;
    const f32* limit_source = self->use_own_limit ? &self->own_limit : source_limit;
    f32 limit = *limit_source;
    base::math::CVector2D pos2;
    {
        UnkMoverActor_b0169* actor = self->actor;
        base::math::CVector3D pos;
        u32 px = *reinterpret_cast<const a_u32*>(&actor->x_0x44);
        u32 py = *reinterpret_cast<const a_u32*>(&actor->y_0x54);
        u32 pz = *reinterpret_cast<const a_u32*>(&actor->z_0x64);
        __builtin_memcpy(&pos.x, &px, 4);
        __builtin_memcpy(&pos.y, &py, 4);
        __builtin_memcpy(&pos.z, &pz, 4);
        fn_710007b430(&pos2, &pos);
    }
    f32 dx = self->target_x - pos2.x;
    f32 dy = self->target_y - pos2.y;
    f32 dist = __builtin_sqrtf(dx * dx + dy * dy);
    f32 proj = dx * self->dir_x + dy * self->dir_y;
    bool within = dist <= limit;
    if (self->flag_0x204 && self->flag_0x203) {
        within = (proj < -0.001f) | !(dist > limit);
    } else if (proj < -0.001f) {
        UnkMoverActor_b0169* actor = self->actor;
        self->field_0x50 = 0.0f;
        base::math::CVector3D target;
        u32 tx = *reinterpret_cast<const a_u32*>(&self->target_x);
        u32 tz = *reinterpret_cast<const a_u32*>(&actor->z_0x64);
        __builtin_memcpy(&target.x, &tx, 4);
        u32 ty = *reinterpret_cast<const a_u32*>(&self->target_y);
        __builtin_memcpy(&target.y, &ty, 4);
        __builtin_memcpy(&target.z, &tz, 4);
        within = true;
        actor->vf_0xd0(&target, true);
    } else if (dist < self->nearest) {
        self->nearest = dist;
    }
    return within;
}
