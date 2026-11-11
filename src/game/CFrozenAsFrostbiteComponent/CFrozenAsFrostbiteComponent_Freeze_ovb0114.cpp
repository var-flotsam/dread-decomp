// CFrozenAsFrostbiteComponent's unit (tools/tu_map.py): the frostbite freeze. Hits (fn_71007f5880) raise
// a freeze count up to the definition's limit and tint the actor's slots; the update (fn_71007f46e8) runs
// a delay and a time down and thaws; the definition's parts get one object each (fn_71007f4170,
// fn_71007f4514), handed back to the lazily created manager on thawing. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkStruct_71001436f4;
bool fn_71001436f4(UnkStruct_71001436f4* self, u32 a, const base::math::CVector4D* b, u32 c);
extern const base::math::CVector4D g_unk_710164ab98 __attribute__((visibility("hidden")));

struct UnkSlotOwner_b0025;
void fn_7100da8f00(UnkSlotOwner_b0025* self, UnkSwitchObject* object);

using base::reflection::UnkReflObject;

struct UnkFrostbiteDefHolder_ovb0114 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkFrostbiteDef_ovb0114 {
    u8 pad_0x0[0x5c];
    f32 field_0x5c;
    u8 pad_0x60[0x40];
    s32 field_0xa0;
    f32 field_0xa4;
    f32 field_0xa8;
    u8 pad_0xac[0x4];
    UnkFrostbiteDefHolder_ovb0114* holder2;  // 0xb0
    UnkFrostbiteDefHolder_ovb0114* holder;   // 0xb8
    u8 pad_0xc0[0x8];
    void** parts;                           // 0xc8
    u32 partCount;                          // 0xd0
};

struct UnkFrostbiteSlotsOwner_ovb0114 {
    u8 pad_0x0[0x50];
    UnkStruct_71001436f4* slots;
};

struct __attribute__((packed, aligned(4))) UnkFrozenAsFrostbite_ovb0114 {
    u8 pad_0x0[0x10];
    UnkReflObject* charClass;  // 0x10
    u8 pad_0x18[0x18];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0xc];
    f32 field_0x44;
    u8 pad_0x48[0x368];
    UnkFrostbiteDef_ovb0114* def;  // 0x3b0
    s32 count;                     // 0x3b8
    f32 timer;  // 0x3bc
    f32 delay;  // 0x3c0
    f32 field_0x3c4;
    u8 pad_0x3c8[0x18];
    f32 field_0x3e0;
    u8 pad_0x3e4[0x4];
    base::UnkVector<UnkSwitchObject*> objects;  // 0x3e8
};
static_assert(__builtin_offsetof(UnkFrozenAsFrostbite_ovb0114, objects) == 0x3e8, "layout");

// Clears the time and the delay (one 8-byte store once merged).
static inline void ClearTimers(UnkFrozenAsFrostbite_ovb0114* self) {
    self->timer = 0.0f;
    self->delay = 0.0f;
}

static inline void AddKey(UnkActor* actor, u64 key) {
    fn_7100d7c828(actor, &key, false);
}

// The manager, created on first use.
static inline UnkManager_71726af468* Manager() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

void fn_71007f97ec(UnkFrozenAsFrostbite_ovb0114* self);  // the base method

// After the base method: while frozen, gives the actor's timeline the definition's id; clears the
// count and +0x3bc, runs request 2 on the actor's slots, and hands every object back to the manager.
void fn_71007f4278(UnkFrozenAsFrostbite_ovb0114* self) {
    fn_71007f97ec(self);
    if (self->count >= 1) {
        AddKey(self->actor, self->def->holder->id);
    }
    self->count = 0;
    ClearTimers(self);
    UnkFrostbiteSlotsOwner_ovb0114* owner = static_cast<UnkFrostbiteSlotsOwner_ovb0114*>(self->actor->field_0x228);
    fn_71001436f4(owner->slots, 2, &g_unk_710164ab98, 1);
    for (UnkSwitchObject** it = self->objects.data, **end = it + self->objects.count; it != end; ++it) {
        if (UnkSwitchObject* object = *it)
            fn_7100da8f00(reinterpret_cast<UnkSlotOwner_b0025*>(Manager()), object);
    }
    self->objects.count = 0;
}

// After the base step: caches the character class (a CCharClassFrozenAsFrostbiteComponent) and
// makes room in the object list for one object per part of it.
bool fn_71007f4170(UnkFrozenAsFrostbite_ovb0114* self, void* arg) {
    if (!fn_71007f900c(self, arg))
        return false;
    UnkReflObject* object = self->charClass;
    self->def = reinterpret_cast<UnkFrostbiteDef_ovb0114*>(
        object && fn_710009d50c(object->vf_0x0(), fn_71007f0258()) ? object : nullptr);
    u32 n = self->def->partCount;
    base::UnkVector<UnkSwitchObject*>& objects = self->objects;
    if (n && objects.capacity < n) {
        UnkSwitchObject** data =
            static_cast<UnkSwitchObject**>(objects.allocator->vf_0x40((u64)n * sizeof(UnkSwitchObject*), 8, objects.flags));
        if (objects.data) {
            __builtin_memcpy(data, objects.data, (u64)objects.count * sizeof(UnkSwitchObject*));
            objects.allocator->vf_0x48(objects.data);
        }
        objects.data = data;
        objects.capacity = n;
    }
    return true;
}

struct UnkParent;
void fn_71007f4514(UnkFrozenAsFrostbite_ovb0114* self);

extern UnkTunable g_unk_7172720dc0;

// The tunable's frost colour and how the last component fades with the remaining time.
struct UnkFrostbiteTunable_ovb0114 {
    u8 pad_0x0[0x24];
    f32 r, g, b;   // 0x24
    f32 alphaMin;  // 0x30
    f32 alphaMax;  // 0x34
};

static inline f32 NonNegative(f32 v) {
    return v <= 0.0f ? 0.0f : v;
}

// Hands every object back to the manager and empties the list.
static inline void ReleaseObjects(UnkFrozenAsFrostbite_ovb0114* self) {
    for (UnkSwitchObject** it = self->objects.data, **end = it + self->objects.count; it != end; ++it) {
        if (UnkSwitchObject* object = *it)
            fn_7100da8f00(reinterpret_cast<UnkSlotOwner_b0025*>(Manager()), object);
    }
    self->objects.count = 0;
}

void fn_71007f4378(UnkFrozenAsFrostbite_ovb0114* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (self->field_0x3c4 > 0.0f) {
        self->field_0x3c4 -= dt;
        if (self->field_0x3c4 <= 0.0f)
            ReleaseObjects(self);
    } else if (fn_71007f9f20(self)) {
        fn_71007f4514(self);
    }
    if (self->field_0x3e0 > 0.0f) {
        f32 t = NonNegative(self->field_0x3e0 - dt);
        self->field_0x3e0 = t;
        if (self->def->partCount) {
            f32 ratio = t / 0.8f;
            UnkFrostbiteSlotsOwner_ovb0114* owner = static_cast<UnkFrostbiteSlotsOwner_ovb0114*>(self->actor->field_0x228);
            const UnkFrostbiteTunable_ovb0114* tunable =
                reinterpret_cast<const UnkFrostbiteTunable_ovb0114*>(&g_unk_7172720dc0);
            f32 alpha = tunable->alphaMin + ratio * (tunable->alphaMax - tunable->alphaMin);
            UnkStruct_71001436f4* slots = owner->slots;
            base::math::CVector4D colour = {NonNegative(tunable->r), NonNegative(tunable->g), NonNegative(tunable->b),
                                            NonNegative(alpha)};
            fn_71001436f4(slots, 2, &colour, 1);
        }
    }
}

struct UnkEntity;
bool fn_71011735e8(UnkEntity* entity, const void* key, UnkMatrix44* out, u32 flags);
UnkVec3f fn_7100076acc(const UnkXform* x);
UnkSwitchObject*& fn_71007f4d8c(base::UnkVector<UnkSwitchObject*>* self, u32 index);
void fn_7100145f28(UnkSwitchObject* self, const UnkVec3f* position);
void fn_7100145f3c(UnkSwitchObject* self, const UnkVec3f* angles);

struct UnkFrostbitePartName_ovb0114 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkFrostbitePart_ovb0114 {
    u8 pad_0x0[0x10];
    UnkFrostbitePartName_ovb0114* name;
};

// Once there is one object per part: places each object at the transform of its part's node in
// the actor's model.
void fn_71007f4514(UnkFrozenAsFrostbite_ovb0114* self) {
    UnkFrostbiteDef_ovb0114* def = self->def;
    if (def->partCount != self->objects.count)
        return;
    for (u64 i = 0; i < def->partCount; ++i) {
        if (!fn_71007f4d8c(&self->objects, i))
            continue;
        UnkMatrix44 m;
        UnkCopyMatrix(&m, fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38)));
        {
            UnkEntity* entity = static_cast<UnkEntity*>(self->actor->field_0x228);
            u64 key = static_cast<UnkFrostbitePart_ovb0114*>(def->parts[i])->name->key;
            fn_71011735e8(entity, &key, &m, 0);
        }
        {
            UnkSwitchObject* object = fn_71007f4d8c(&self->objects, i);
            UnkVec3f position(m.m[3], m.m[7], m.m[11]);
            fn_7100145f28(object, &position);
        }
        {
            UnkSwitchObject* object = fn_71007f4d8c(&self->objects, i);
            UnkVec3f angles = fn_7100076acc(reinterpret_cast<const UnkXform*>(&m));
            fn_7100145f3c(object, &angles);
        }
    }
}

void fn_71007fa610(UnkFrozenAsFrostbite_ovb0114* self, f32 dt);  // the base method

// Tints the actor's slots with the tunable's frost colour, its alpha at `ratio` between the two ends.
static inline void Tint(UnkFrozenAsFrostbite_ovb0114* self, f32 ratio) {
    UnkStruct_71001436f4* slots = static_cast<UnkFrostbiteSlotsOwner_ovb0114*>(self->actor->field_0x228)->slots;
    const UnkFrostbiteTunable_ovb0114* tunable = reinterpret_cast<const UnkFrostbiteTunable_ovb0114*>(&g_unk_7172720dc0);
    f32 alpha = tunable->alphaMin + ratio * (tunable->alphaMax - tunable->alphaMin);
    base::math::CVector4D colour = {NonNegative(tunable->r), NonNegative(tunable->g), NonNegative(tunable->b),
                                    NonNegative(alpha)};
    fn_71001436f4(slots, 2, &colour, 1);
}

// Without parts, tints the actor by the elapsed share of the definition's duration (+0x5c); then the
// base update.
void fn_71007f4f64(UnkFrozenAsFrostbite_ovb0114* self, f32 dt) {
    UnkFrostbiteDef_ovb0114* def = self->def;
    if (!def->partCount) {
        f32 duration = def->field_0x5c;
        if (duration > 0.0f)
            Tint(self, self->field_0x44 / duration);
        else
            Tint(self, 0.0f);
    }
    fn_71007fa610(self, dt);
}

void fn_71007fa07c(UnkFrozenAsFrostbite_ovb0114* self, f32 dt);  // the base method

// While frozen: after the delay (+0x3c0) runs out, tints the actor by the remaining time and count;
// once the time (+0x3bc) runs out too, thaws (as fn_71007f4860). Then the base update.
void fn_71007f46e8(UnkFrozenAsFrostbite_ovb0114* self, f32 dt) {
    if (self->count >= 1 && self->timer > 0.0f) {
        f32 delay = self->delay;
        if (delay > 0.0f) {
            delay -= dt;
            self->delay = delay;
            if (delay <= 0.0f) {
                self->timer += delay;
                self->delay = 0.0f;
                delay = 0.0f;
            }
        }
        if (delay == 0.0f) {
            f32 count = (f32)self->count;
            UnkFrostbiteDef_ovb0114* def = self->def;
            Tint(self, (count / (f32)def->field_0xa0) * (self->timer / def->field_0xa4));
            self->timer -= dt;
            if (self->timer <= 0.0f) {
                if (self->count >= 1)
                    AddKey(self->actor, self->def->holder->id);
                self->count = 0;
                ClearTimers(self);
                UnkFrostbiteSlotsOwner_ovb0114* owner =
                    static_cast<UnkFrostbiteSlotsOwner_ovb0114*>(self->actor->field_0x228);
                fn_71001436f4(owner->slots, 2, &g_unk_710164ab98, 1);
            }
        }
    }
    fn_71007fa07c(self, dt);
}

struct UnkFrostbiteComponent;
struct UnkFrostbiteRequest;
struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);

// A damage request as the hit handler sees it.
struct UnkFrostbiteDamage_ovb0114 {
    u8 pad_0x0[0xf4];
    bool field_0xf4;
    u8 pad_0xf5[0x3];
    f32 amount;  // 0xf8
};

struct UnkFrostbiteLife_ovb0114 {
    u8 pad_0x0[0x54];
    f32 life;
};

// A hit: a frozen component that is not breaking thaws (fn_71007fa130), or else goes to state 2;
// otherwise the hit adds to the freeze count and tints the actor, until the count reaches the
// definition's limit (+0xa0), where the request is capped to leave the actor one point of life and
// the component goes to state 2. A refused request marks it (+0xf4) when the component is frozen
// and not breaking. Always answers false.
bool fn_71007f5880(UnkFrozenAsFrostbite_ovb0114* self, UnkFrostbiteDamage_ovb0114* request) {
    UnkFrostbiteComponent* c = reinterpret_cast<UnkFrostbiteComponent*>(self);
    bool ok = fn_71007fb280(c, reinterpret_cast<UnkFrostbiteRequest*>(request));
    bool frozen = fn_71007f9f20(self);
    if (!ok) {
        if (frozen && !fn_71007f9fb0(c) && !fn_71007f9fd0(c))
            request->field_0xf4 = true;
        return false;
    }
    if (frozen) {
        if (!fn_71007f9fb0(c) && !fn_71007f9fd0(c)) {
            fn_71007fa130(c);
            return false;
        }
    } else {
        s32 count = ++self->count;
        UnkFrostbiteDef_ovb0114* def = self->def;
        if (count < def->field_0xa0) {
            // Copied untyped: the limit is read again afterwards.
            __builtin_memcpy(&self->delay, &def->field_0xa8, sizeof(f32));
            __builtin_memcpy(&self->timer, &def->field_0xa4, sizeof(f32));
            Tint(self, (f32)count / (f32)def->field_0xa0);
            self->field_0x3e0 = 0.0f;
            if (self->count == 1)
                AddKey(self->actor, self->def->holder2->id);
            return false;
        }
        f32 most = static_cast<UnkFrostbiteLife_ovb0114*>(self->actor->field_0x1b8)->life + -1.0f;
        f32 amount = request->amount;
        fn_71008c1538(reinterpret_cast<UnkRequest*>(request),
                      amount <= 0.0f ? 0.0f : (amount >= most ? most : amount));
    }
    fn_71007f9428(c, 2);
    return false;
}
