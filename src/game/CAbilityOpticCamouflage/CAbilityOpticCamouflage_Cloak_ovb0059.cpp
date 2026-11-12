// CAbilityOpticCamouflage (tools/tu_map.py): the copy constructor of a vector of weak-handle
// references (fn_710031d904), the destructors (the listener at +0x3a0 detaches from the gun component
// its weak handle at +0x18 points at, through fn_710083b0bc, and drops the handle; then the base
// destructor fn_71002fa2bc, inlined in the deleting one) and the cloak's colour pulse on the actor's
// model (+0x280): a phase at +0x394 advancing at 10 or 20 per second (20 while fn_7100a1f84c holds
// for the special energy, +0xc0's +0xe8 part, its +0x10 component), blending two colours, flat while
// fn_7100301700 reports the actor's +0x2e0 above zero. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CAbilityOpticCamouflage.h"
#include "game/CFXComponent.h"
#include "game/CGameObject.h"
#include "game/CGunComponent.h"
#include "game/UnkTU39.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a230a8[];  // the ability's vtable
extern void* g_unk_7101a225f8[];  // the value holders' vtable
extern void* g_unk_7101a22650[];  // the vtable of the member at +0x10

struct UnkListener_b40;
struct UnkSpecialEnergyComponent;
struct UnkComponent;
struct UnkObject;

struct UnkV4_b47 {
    f32 x, y, z, w;
    UnkV4_b47() {}
    UnkV4_b47(f32 x_, f32 y_, f32 z_, f32 w_) : x(x_), y(y_), z(z_), w(w_) {}
    UnkV4_b47(const UnkV4_b47& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

struct UnkCameraCtrl_ovb0049;
bool fn_7100a1f84c(UnkSpecialEnergyComponent* self);
f32 fn_7100301700(void* p);
struct UnkPart;
struct UnkWorld;
bool fn_710114e674(UnkObject* model, u8 index);
void fn_71001bc10c(void* p, const UnkV4_b47* colour, bool flag);
extern const UnkV4_b47 g_unk_7101648e38 __attribute__((visibility("hidden")));  // the cloak colour
extern const UnkV4_b47 g_unk_7101648e48 __attribute__((visibility("hidden")));  // the other one

static inline UnkV4_b47 operator*(const UnkV4_b47& a, f32 s) { return fn_7100030830(&a, s); }
static inline UnkV4_b47 operator+(const UnkV4_b47& a, const UnkV4_b47& b) { return fn_7100030770(&a, &b); }

struct UnkEnergyPart_ovb0059 {
    u8 pad_0x0[0x10];
    UnkSpecialEnergyComponent* energy;  // 0x10
};

struct UnkOpticActor_ovb0059 {
    u8 pad_0x0[0x280];
    UnkObject* model;  // 0x280
    u8 pad_0x288[0x58];
    void* field_0x2e0;
};

struct UnkOpticOwner_ovb0059 {
    u8 pad_0x0[0x30];
    UnkOpticActor_ovb0059* actor;  // 0x30
    u8 pad_0x38[0xb0];
    UnkEnergyPart_ovb0059* part;  // 0xe8
};

// A part of the model: its index (+0xb) and what fn_71001bc10c colours (+0x150).
struct UnkOpticModelPart_ovb0059 {
    u8 pad_0x0[0xb];
    u8 index;
    u8 pad_0xc[0x144];
    void* field_0x150;
};

struct UnkClearedPair_ovb0059 {
    u64 a, b;

    ~UnkClearedPair_ovb0059() {
        a = 0;
        b = 0;
    }
};

struct UnkOpticValueHolder_ovb0059 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkClearedPair_ovb0059 pairs[4];

    __attribute__((always_inline)) void Destroy() {
        for (int i = 3; i >= 0; --i)
            pairs[i].~UnkClearedPair_ovb0059();
        vtable = &g_unk_7101a225f8[2];
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkOpticValueHolder_ovb0059) == 0x70, "holder size");

struct UnkOpticMember_ovb0059 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
};

struct UnkOpticListener_ovb0059 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* link;  // 0x18
};

struct UnkOpticAbility_ovb0059 {
    void** vtable;
    u64 field_0x8;
    UnkOpticMember_ovb0059 member;  // 0x10
    u8 pad_0x38[0x88];
    UnkOpticOwner_ovb0059* owner;  // 0xc0
    u8 pad_0xc8[0x20];
    UnkOpticValueHolder_ovb0059 holders[6];  // 0xe8
    u8 pad_0x388[0xc];
    f32 phase;  // 0x394
    u8 pad_0x398[0x8];
    UnkOpticListener_ovb0059 listener;  // 0x3a0
};
static_assert(__builtin_offsetof(UnkOpticAbility_ovb0059, holders) == 0xe8, "layout");
static_assert(__builtin_offsetof(UnkOpticAbility_ovb0059, phase) == 0x394, "layout");
static_assert(__builtin_offsetof(UnkOpticAbility_ovb0059, listener) == 0x3a0, "layout");

void fn_71003247cc(UnkOpticAbility_ovb0059* self) {
    self->vtable = &g_unk_7101a230a8[2];
    UnkOpticListener_ovb0059* listener = &self->listener;
    listener->vtable = &g_unk_7101a23280[2];
    if (UnkWeakHandle* link = listener->link) {
        if (UnkWeakTarget* owner = link->target) {
            fn_710083b0bc(owner, reinterpret_cast<UnkListener_b40*>(listener));
            link = listener->link;
        }
        if (link && --link->ref_count == 0)
            link->vf_0x10();
    }
    fn_71002fa2bc(self);
}

void fn_7100324060(UnkOpticAbility_ovb0059* self, f32 dt) {
    UnkOpticOwner_ovb0059* owner = self->owner;
    UnkOpticActor_ovb0059* actor = owner->actor;
    UnkSpecialEnergyComponent* energy = owner->part ? owner->part->energy : nullptr;
    f32 speed = fn_7100a1f84c(energy) ? 20.0f : 10.0f;
    self->phase += speed * dt;
    f32 t = sinf(self->phase) * 0.5f + 0.5f;
    t *= fn_7100301700(actor->field_0x2e0) > 0.0f ? 0.0f : 1.0f;
    UnkV4_b47 c = g_unk_7101648e38 * t + g_unk_7101648e48 * (1.0f - t);
    UnkObject* model = self->owner->actor->model;
    UnkOpticModelPart_ovb0059* part =
        reinterpret_cast<UnkOpticModelPart_ovb0059*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(model),
                                                                   0x979ff4c6002c84dcull));
    if (part && fn_710114e674(model, part->index))
        fn_71001bc10c(part->field_0x150, &c, true);
}

// The deleting destructor, with the base destructor (fn_71002fa2bc) inlined: CAbility's vtable, its six
// value holders (+0xe8, 0x70 bytes each, last first) and the member at +0x10, then fn_7101043934.
void fn_710032485c(UnkOpticAbility_ovb0059* self) {
    self->vtable = &g_unk_7101a230a8[2];
    UnkOpticListener_ovb0059* listener = &self->listener;
    listener->vtable = &g_unk_7101a23280[2];
    if (UnkWeakHandle* link = listener->link) {
        if (UnkWeakTarget* owner = link->target) {
            fn_710083b0bc(owner, reinterpret_cast<UnkListener_b40*>(listener));
            link = listener->link;
        }
        if (link && --link->ref_count == 0)
            link->vf_0x10();
    }
    self->vtable = &g_unk_7101a223f8[2];
    for (int i = 5; i >= 0; --i)
        self->holders[i].Destroy();
    self->member.vtable = &g_unk_7101a22650[2];
    self->member.field_0x20 = 0;
    self->member.field_0x10 = 0;
    self->member.field_0x18 = 0;
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
    operator delete(self);
}

// A counted weak-handle reference, the element of the vector (as CBossCameraCtrl's targets,
// src/game/CBossCameraCtrl/CBossCameraCtrl_Targets_ovb0008.cpp).
struct UnkHandlePtr_ovb0008;
struct UnkOpticHandlePtr_ovb0059 {
    UnkWeakHandle* p;
};

template <class Tag>
struct UnkSlotArray;
struct UnkTag_710031da18;
void fn_710031da18(UnkSlotArray<UnkTag_710031da18>* v, u32 capacity);

// The vector's copy constructor: the same growth settings and allocator, then each reference
// appended (fn_710031da18 grows the array).
void fn_710031d904(base::UnkVector<UnkOpticHandlePtr_ovb0059>* self,
                   const base::UnkVector<UnkOpticHandlePtr_ovb0059>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_710031da18(reinterpret_cast<UnkSlotArray<UnkTag_710031da18>*>(self), other->capacity);
    const UnkOpticHandlePtr_ovb0059* end = other->data + other->count;
    for (const UnkOpticHandlePtr_ovb0059* it = other->data; it != end; ++it) {
        u32 n = self->count;
        u32 capacity = self->capacity;
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)capacity)
            for (;;) {
            }
        fn_710031da18(reinterpret_cast<UnkSlotArray<UnkTag_710031da18>*>(self), capacity);
        // The slot is written bitwise: the original reloads the count after the writes.
        UnkWeakHandle** slot = &self->data[self->count].p;
        __builtin_memset(slot, 0, sizeof(*slot));
        UnkWeakHandle* p = it->p;
        if (*slot != p) {
            if (p)
                ++p->ref_count;
            if (UnkWeakHandle* old = *slot) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            __builtin_memcpy(slot, &p, sizeof(p));
        }
        ++self->count;
    }
}
