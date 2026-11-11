// CAbilityGhostAura's unit (tools/tu_map.py): the ability's constructor (fn_7100314ac8), the resize of its
// entry array (fn_7100314c88), the per-frame update (fn_7100309570) and dash/stop steps (fn_710030d260,
// fn_710031410c, fn_7100314288), the reaction to a "ghost aura" message (fn_7100314508) and the target
// visitor that keeps the closest target (fn_7100314e54). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/core/CBaseObject.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbility.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU365.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkGroundPlayer_b52;
struct UnkGhostAura_A2_b70;
struct UnkSelf;

extern u64 g_unk_7101648d80;

struct UnkGhostAuraSub_b0040 {
    u8 pad_0x0[0x3a8];
    bool active;  // 0x3a8
};

struct UnkHelper;
struct UnkAbility_b66a2;
struct UnkAbilityC_A2_b69;
struct UnkSmAbility_A2_b63;

struct UnkGhostAuraPose_b0040 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
    u8 pad_0x14[0x1b4 - 0x14];
    f32 field_0x1b4;
};

struct UnkGhostAuraTimeline_b0040 {
    u8 pad_0x0[0x3a8];
    bool active;  // 0x3a8
    u8 pad_0x3a9[0x7e4 - 0x3a9];
    bool field_0x7e4;
    u8 pad_0x7e5[0x2d08 - 0x7e5];
    UnkGhostAuraPose_b0040* pose;  // 0x2d08
};

struct UnkGhostAuraMeter_b0040 {
    u8 pad_0x0[0x40];
    f32 max;  // 0x40
    f32 value;  // 0x44
};

struct UnkGhostAuraMeterHolder_b0040 {
    u8 pad_0x0[0x10];
    UnkGhostAuraMeter_b0040* meter;  // 0x10
};

struct UnkGhostAuraSubState_b0040 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkGhostAuraActor_b0040 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
    u8 pad_0x68[0x1f0 - 0x68];
    UnkGroundPlayer_b52* ground;  // 0x1f0
    UnkGhostAuraSub_b0040* sub;  // 0x1f8
    u8 pad_0x200[0x8];
    UnkGhostAuraTimeline_b0040* timeline;  // 0x208
    u8 pad_0x210[0x10];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x38];
    void* field_0x260;
};

struct UnkGhostAuraLink_b0040 {
    u8 pad_0x0[0x30];
    UnkGhostAuraActor_b0040* actor;  // 0x30
    u8 pad_0x38[0xb0];
    UnkGhostAuraMeterHolder_b0040* meters;  // 0xe8
};

struct UnkGhostAura_b0040 {
    u8 pad_0x0[0x20];
    UnkGhostAuraSubState_b0040* sub;  // 0x20
    u8 pad_0x28[0x98];
    UnkGhostAuraLink_b0040* link;  // 0xc0
    u8 pad_0xc8[0x3d0 - 0xc8];
    u32 field_0x3d0;
    u8 pad_0x3d4[0x438 - 0x3d4];
    s32 field_0x438;
    u8 pad_0x43c[0x448 - 0x43c];
    s32 state;  // 0x448
    s32 field_0x44c;
    u8 pad_0x450[0x460 - 0x450];
    f32 field_0x460;
    u32 field_0x464;
    u8 pad_0x468[4];
    f32 field_0x46c;
    u8 pad_0x470[8];
    f32 field_0x478;
    f32 charge;  // 0x47c
    u8 pad_0x480[0x15];
    bool field_0x495;
    u8 pad_0x496;
    u8 field_0x497;
    bool field_0x498;
    u8 pad_0x499[2];
    u8 field_0x49b;
    bool field_0x49c;
    bool field_0x49d;
    u8 pad_0x49e[3];
    u8 field_0x4a1;
};
static_assert(__builtin_offsetof(UnkGhostAura_b0040, field_0x4a1) == 0x4a1, "UnkGhostAura_b0040 layout");

struct UnkGhostAuraPos_b0040 {
    u32 x = 0;
    u32 y = 0;
    u32 z = 0;
};

bool fn_71010d1598(UnkGroundPlayer_b52*);
bool fn_71002f927c(UnkAbility_b66a2* self);
void fn_71002f8de4(UnkSmAbility_A2_b63* self, f32 dt);
void fn_71003096fc(UnkGhostAura_b0040* self, f32 dt);
void fn_710030b448(UnkGhostAura_b0040* self, s32 state, bool flag);
void fn_71003139c0(UnkGhostAura_b0040* self, UnkGhostAuraPos_b0040* pos, void* a2, void* a3, f32 value);

void fn_710030d260(UnkGhostAura_b0040* self) {
    if (!self->link)
        return;
    UnkGhostAuraActor_b0040* actor = self->link->actor;
    if (!actor || !actor->ground)
        return;
    bool active = actor->sub && actor->sub->active;
    s32 state = self->state;
    if (fn_71010d1598(actor->ground)) {
        if (static_cast<u32>(state - 3) <= 3) {
            fn_7100309fd8(reinterpret_cast<UnkGhostAura_A2_b70*>(self), 0, 1);
            if (self->link && self->link->actor && self->link->actor->timeline)
                fn_7100d7c828(reinterpret_cast<UnkActor*>(self->link->actor), &g_unk_7101648d80, false);
            self->field_0x3d0 = 0;
            self->field_0x460 = 0.0f;
            self->field_0x464 = 0;
            self->field_0x46c = 0.0f;
            self->field_0x4a1 = 0;
            self->field_0x49b = 0;
            fn_71002f9460(reinterpret_cast<UnkSelf*>(self), 0);
        }
    } else {
        fn_710030b448(self, state, active);
    }
}

void fn_7100314288(UnkGhostAura_b0040* self) {
    UnkGhostAuraActor_b0040* actor = self->link ? self->link->actor : nullptr;
    UnkGhostAuraPos_b0040 pos;
    if (!actor) {
        fn_71003139c0(self, &pos, nullptr, nullptr, self->field_0x460);
        return;
    }
    u32 x = actor->field_0x44, y = actor->field_0x54, z = actor->field_0x64;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    UnkAnimationComponent* animation = actor->animation;
    if (animation && fn_710117b348(animation, fn_7100cfd938()))
        fn_710030b448(self, 5, fn_710030b8b4(reinterpret_cast<UnkSelf*>(self), &pos, nullptr));
}

static inline void Stop_b0040(UnkGhostAura_b0040* self) {
    fn_7100309fd8(reinterpret_cast<UnkGhostAura_A2_b70*>(self), 0, 1);
    if (self->link && self->link->actor && self->link->actor->timeline)
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->link->actor), &g_unk_7101648d80, false);
    self->field_0x3d0 = 0;
    self->field_0x460 = 0.0f;
    self->field_0x464 = 0;
    self->field_0x46c = 0.0f;
    self->field_0x4a1 = 0;
    self->field_0x49b = 0;
    fn_71002f9460(reinterpret_cast<UnkSelf*>(self), 0);
}

void fn_7100309570(UnkGhostAura_b0040* self, f32 dt) {
    if (self->sub && self->sub->state == 1 && fn_71002f927c(reinterpret_cast<UnkAbility_b66a2*>(self)))
        fn_71002f9694(reinterpret_cast<UnkAbilityC_A2_b69*>(self), true);
    if (self->state != 0 && self->link) {
        if (UnkGhostAuraActor_b0040* actor = self->link->actor) {
            if (UnkGhostAuraTimeline_b0040* timeline = actor->timeline) {
                u8* keep = &self->field_0x497;
                *keep = *keep && !timeline->active && !fn_71009e6434(reinterpret_cast<UnkHelper*>(timeline));
            }
            if (actor->timeline)
                fn_71003096fc(self, dt);
            else
                Stop_b0040(self);
        }
    }
    fn_71002f8de4(reinterpret_cast<UnkSmAbility_A2_b63*>(self), dt);
}


struct UnkGhostAuraEventData_b0040 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkGhostAuraEvent_b0040 {
    s32 kind;
    u8 pad_0x4[4];
    void* field_0x8;
    UnkGhostAuraEventData_b0040* data;  // 0x10
};

static inline void Release_b0040(UnkGhostAura_b0040* self, u32 a) {
    fn_7100309fd8(reinterpret_cast<UnkGhostAura_A2_b70*>(self), a, 1);
}

bool fn_7100314508(UnkGhostAura_b0040* self, const UnkGhostAuraEvent_b0040* event) {
    if (!event->field_0x8)
        return false;
    if (event->data->id != 0xf4061ff25a2163d6)
        return true;
    s32 state = self->state;
    switch (event->kind) {
    case 1:
        switch (state) {
        case 3:
            if (self->field_0x49d || self->field_0x49c)
                Release_b0040(self, 0);
            else
                Release_b0040(self, 4);
            break;
        case 6:
            Release_b0040(self, 0);
            reinterpret_cast<f32&>(self->field_0x464) = 1.0f;
            break;
        case 2:
            if (self->field_0x478 <= 0.0f)
                Release_b0040(self, 0);
            break;
        }
        break;
    case 2:
        if (self->field_0x438 == 2 && (self->field_0x49c || self->field_0x49d)) {
            Release_b0040(self, 0);
            break;
        }
        switch (state) {
        case 3:
            self->field_0x44c = 4;
            break;
        case 6:
            Release_b0040(self, 0);
            reinterpret_cast<f32&>(self->field_0x464) = 1.0f;
            break;
        }
        break;
    }
    return true;
}

extern f32 g_unk_7101a22a70;
extern f32 g_unk_7101a22a74;
extern f32 g_unk_7101a22a78;
extern f32 g_unk_7101a22a7c;

static inline bool KeepsDash_b0040(UnkGhostAura_b0040* self) {
    return self->state == 2 && !(self->field_0x478 <= 0.0f);
}

void fn_710031410c(UnkGhostAura_b0040* self) {
    if (static_cast<u32>(self->state) < 2)
        return;
    if (self->state == 4) {
        f32 elapsed = self->field_0x46c;
        f32 limit = 0.0f;
        if (self->link && self->link->actor) {
            s32 mode = self->field_0x438;
            bool boosted = self->field_0x495;
            switch (mode) {
            case 1:
                if (boosted)
                    limit = g_unk_7101a22a7c;
                else
                    limit = g_unk_7101a22a70;
                break;
            case 2:
                if (boosted)
                    limit = g_unk_7101a22a78;
                else
                    limit = g_unk_7101a22a74;
                break;
            }
        }
        if (elapsed <= limit) {
            fn_7100314288(self);
            if (!self->field_0x498)
                self->field_0x44c = 5;
        }
        if (!(self->field_0x46c <= 0.0f))
            return;
        if (self->state != 5) {
            fn_7100314288(self);
            self->field_0x44c = 5;
        }
        self->field_0x44c = 6;
        return;
    }
    UnkGhostAuraActor_b0040* actor = self->link->actor;
    UnkAnimationComponent* animation = actor->animation;
    if (animation && !fn_710117b348(animation, fn_7100cfd938())) {
        if (!KeepsDash_b0040(self))
            self->field_0x44c = 0;
    }
    if (actor->timeline && fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(actor->timeline))) {
        if (!KeepsDash_b0040(self))
            self->field_0x44c = 0;
    }
}

// ----

struct UnkNode;
struct UnkGhostAuraBase_b0040;

void fn_71002f8414(UnkGhostAuraBase_b0040* self);
void fn_7100314d94(void* self);
extern void* g_unk_7101a22a88[];

template <class T>
static inline T& At_b0040(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

struct UnkGhostAuraBase_b0040 {
    void** vtable;
};

void fn_7100314ac8(UnkGhostAuraBase_b0040* self) {
    fn_71002f8414(self);
    self->vtable = g_unk_7101a22a88 + 2;
    At_b0040<void**>(self, 0x390) = g_unk_7101a22d48 + 2;
    At_b0040<u64>(self, 0x398) = 0;
    At_b0040<u64>(self, 0x3a0) = 0;
    At_b0040<u64>(self, 0x3a8) = 0;
    At_b0040<u64>(self, 0x3b0) = 0;
    At_b0040<void*>(self, 0x3b8) = self;
    At_b0040<void (*)(void*)>(self, 0x3c0) = fn_7100314d94;
    At_b0040<u64>(self, 0x3c8) = 0;
    At_b0040<u64>(self, 0x3d0) = 0;
    At_b0040<u32>(self, 0x3d8) = 10;
    At_b0040<bool>(self, 0x3dc) = true;
    At_b0040<u32>(self, 0x3e0) = 0xf;
    At_b0040<base::UnkAllocator*>(self, 0x3e8) = fn_7100083990();
    At_b0040<u32>(self, 0x438) = 0;
    At_b0040<u64>(self, 0x454) = 0;
    At_b0040<u32>(self, 0x45c) = 0;
    At_b0040<f32>(self, 0x460) = 1.0f;
    At_b0040<f32>(self, 0x464) = 1.0f;
    for (u64 offset = 0x3f0; offset < 0x430; offset += 8)
        At_b0040<u64>(self, offset) = 0;
    At_b0040<u32>(self, 0x430) = 0;
    At_b0040<u64>(self, 0x440) = 0;
    At_b0040<u64>(self, 0x448) = 0;
    At_b0040<u64>(self, 0x468) = 0;
    At_b0040<u64>(self, 0x470) = 0;
    At_b0040<u32>(self, 0x478) = 0;
    At_b0040<f32>(self, 0x47c) = -1.0f;
    At_b0040<u32>(self, 0x480) = 0;
    At_b0040<f32>(self, 0x484) = -1.0f;
    At_b0040<f32>(self, 0x488) = -1.0f;
    At_b0040<f32>(self, 0x48c) = -1.0f;
    At_b0040<f32>(self, 0x490) = -1.0f;
    __builtin_memset(&At_b0040<u8>(self, 0x494), 0, 14);
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}

// ----

struct UnkGhostAuraEntry_b0040 {
    u32 words[3];
    f32 field_0xc;
    u16 field_0x10;

    UnkGhostAuraEntry_b0040() {
        const UnkGhostAuraEntry_b0040 init = {{0, 0, 0}, -1.0f, 0};
        __builtin_memcpy(this, &init, 0x12);
    }
    constexpr UnkGhostAuraEntry_b0040(const u32 (&w)[3], f32 c, u16 h) : words{w[0], w[1], w[2]}, field_0xc(c), field_0x10(h) {}
    UnkGhostAuraEntry_b0040(UnkGhostAuraEntry_b0040&& o) {
        words[0] = o.words[0];
        words[1] = o.words[1];
        words[2] = o.words[2];
        __builtin_memcpy(&field_0xc, &o.field_0xc, sizeof(field_0xc) + sizeof(field_0x10));
    }
};

void fn_7100314c88(base::UnkVector<UnkGhostAuraEntry_b0040>* self, u32 n) {
    self->Reserve(n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkGhostAuraEntry_b0040();
    self->count = n;
}

// ----

struct UnkAssignedSlot {
    void* ptr;

    UnkAssignedSlot& operator=(const UnkAssignedSlot& other);
};
__attribute__((always_inline)) inline UnkAssignedSlot& UnkAssignedSlot::operator=(const UnkAssignedSlot& other) {
    fn_71002a5d1c(this, &other);
    return *this;
}

struct UnkGhostAuraTargetOwner_b0040 {
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
    virtual u32 vf_0x58();  // category mask
};

struct UnkGhostAuraTargetRef_b0040 {
    u8 pad_0x0[0x10];
    UnkGhostAuraTargetOwner_b0040* owner;  // 0x10
};

struct UnkGhostAuraTarget_b0040 {
    union {
        UnkAssignedSlot slot;
        UnkGhostAuraTargetRef_b0040* ref;
    };
    UnkAssignedSlot slot_0x8;
    u32 words[13];  // 0x10: position at 0x14, normal at 0x2c, flags at 0x3c

    UnkGhostAuraTarget_b0040& operator=(const UnkGhostAuraTarget_b0040& other) {
        if (this != &other) {
            slot = other.slot;
            slot_0x8 = other.slot_0x8;
            for (u32 i = 0; i < 13; ++i)
                words[i] = other.words[i];
        }
        return *this;
    }
};

struct UnkGhostAuraSortCtx_b0040;
struct UnkGhostAuraTargetA_b0040;

s32 fn_7100314fb4(const UnkGhostAuraSortCtx_b0040* ctx, const UnkGhostAuraTargetA_b0040* a,
                  const UnkGhostAuraTargetA_b0040* b);

struct UnkGhostAuraBest_b0040 {
    u8 pad_0x0[8];
    UnkGhostAuraTarget_b0040 best;  // 0x8
    u8 pad_0x50[0x28];
    bool has_best;  // 0x78
    u8 pad_0x79[7];
    u8 ctx[0x18];  // 0x80
    u32 ignore_mask;  // 0x98
};


extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_b0040() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

UnkVisitResult_b0040 fn_7100314e54(UnkGhostAuraBest_b0040* self, const UnkGhostAuraTarget_b0040* target) {
    u32 mask = self->ignore_mask;
    if (mask && target->ref && target->ref->owner && (target->ref->owner->vf_0x58() & mask))
        goto skip;
    if (self->has_best &&
        fn_7100314fb4(reinterpret_cast<const UnkGhostAuraSortCtx_b0040*>(self->ctx),
                      reinterpret_cast<const UnkGhostAuraTargetA_b0040*>(target),
                      reinterpret_cast<const UnkGhostAuraTargetA_b0040*>(&self->best)) < 1)
        goto skip;
    self->has_best = true;
    self->best = *target;
    return UnkVisitResult_b0040(1);
skip:
    return Skip_b0040();
}
