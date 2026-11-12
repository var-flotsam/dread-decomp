// Unit 341 (no reliable name; hint 'CActor'): the actor's part and component lists, the callback sets
// and their iterator values, the keyed arrays of reference-counted pointers, and the block-pool list's
// copy constructor. Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCloneCalls.h"
#include "base/UnkCountedResource.h"
#include "base/UnkErasedVector.h"
#include "base/UnkParallelFind.h"
#include "base/UnkRefHandle.h"
#include "base/UnkSList.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CGameObject.h"
#include "game/UnkCallbackSetVector.h"
#include "game/UnkTU32.h"
#include "game/UnkTU341.h"
#include "game/lua_lib.h"
#include "game_s2/UnkWeakHandle.h"
#include <new>
#include "types.h"

class UnkActorPart_71002324f0 {
public:
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
};

struct UnkActor_71002324f0 {
    u8 pad_0x0[0xa0];
    UnkActorPart_71002324f0** parts;  // 0xa0
    void* ids;                   // 0xa8
    u32 part_count;              // 0xb0
};

void fn_71002324f0(void* entity) {
    UnkActor_71002324f0* self = static_cast<UnkActor_71002324f0*>(entity);
    g_unk_717269c4d8 = true;
    for (UnkActorPart_71002324f0 **it = self->parts, **end = it + self->part_count; it != end; ++it)
        (*it)->vf_0x90();
    g_unk_717269c4d8 = false;
    for (s32 i = self->part_count - 1; i >= 0; --i)
        self->parts[static_cast<u32>(i)]->vf_0x78();
}


class UnkActorPart_710023272c {
public:
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
};

class UnkActor_710023272c {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    u8 pad_0x8[0x98];
    UnkActorPart_710023272c** parts;  // 0xa0
    void* ids;                   // 0xa8
    u32 part_count;              // 0xb0
};

void fn_710023272c(void* actor) {
    UnkActor_710023272c* self = static_cast<UnkActor_710023272c*>(actor);
    self->vf_0x40();
    g_unk_717269c4d8 = true;
    for (UnkActorPart_710023272c **it = self->parts, **end = it + self->part_count; it != end; ++it)
        (*it)->vf_0x90();
    g_unk_717269c4d8 = false;
    for (s32 i = self->part_count - 1; i >= 0; --i)
        self->parts[static_cast<u32>(i)]->vf_0x40();
}


class UnkPart_7100232c90 {
public:
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
    virtual void vf_0x98(f32);
    u8 pad_0x8[0x18];
    bool enabled;  // 0x20
    f32 accum;     // 0x24
    f32 period;    // 0x28
};

struct UnkActor_7100232c90 {
    u8 pad_0x0[0xa0];
    UnkPart_7100232c90** parts;  // 0xa0
    void* ids;              // 0xa8
    u32 part_count;         // 0xb0
    u8 pad_0xb4[0x101];
    bool frozen;  // 0x1b5
};

// Its caller (fn_7100d7b068) sets a third argument, a flag, for each call. Nothing here reads it: the
// first part's update is called with the register as it came, the later ones with whatever the
// previous call left there, so it is not an argument of that call.
void fn_7100232c90(UnkActor_7100232c90* self, f32 dt, bool) {
    if (self->frozen)
        return;
    for (UnkPart_7100232c90 **it = self->parts, **end = it + self->part_count; it != end; ++it) {
        UnkPart_7100232c90* part = *it;
        if (!part || !part->enabled)
            continue;
        f32 period = part->period;
        if (period == 0.0f) {
            part->accum = 1.0f;
            part->vf_0x98(dt);
        } else if (period > 0.0f) {
            s32 steps = (s32)((part->accum + dt) / period);
            if (steps >= 1)
                part->vf_0x98(period * (f32)steps);
        }
    }
}



class UnkPart_7100232d4c {
public:
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
    virtual void vf_0xa8(f32);
    virtual void vf_0xb0(f32);
    u8 pad_0x8[0x18];
    bool enabled;  // 0x20
    f32 accum;     // 0x24
    f32 period;    // 0x28
};

struct UnkActor_7100232d4c {
    u8 pad_0x0[0xa0];
    UnkPart_7100232d4c** parts;  // 0xa0
    void* ids;              // 0xa8
    u32 part_count;         // 0xb0
    u8 pad_0xb4[0x1c];
    char field_0xd0[0xe5];
    bool frozen;  // 0x1b5
};

#pragma pack(push, 4)
struct UnkStep_7100232d4c {
    UnkPart_7100232d4c* part;
    f32 dt;
    f32 pad;
};
struct alignas(8) UnkSteps_7100232d4c {
    u32 count;
    u8 raw[25 * sizeof(UnkStep_7100232d4c)];
    void Add(UnkPart_7100232d4c* part, f32 dt) {
        UnkStep_7100232d4c* steps = reinterpret_cast<UnkStep_7100232d4c*>(raw);
        steps[count].part = part;
        steps[count].dt = dt;
        count++;
    }
};
#pragma pack(pop)

void fn_7100232d4c(UnkActor_7100232d4c* self, f32 dt) {
    if (self->frozen)
        return;
    UnkSteps_7100232d4c list;
    list.count = 0;
    for (UnkPart_7100232d4c **it = self->parts, **end = it + self->part_count; it != end; ++it) {
        UnkPart_7100232d4c* part = *it;
        if (!part || !part->enabled)
            continue;
        f32 period = part->period;
        if (period == 0.0f) {
            part->vf_0xa8(dt);
            list.Add(part, dt);
        } else if (period > 0.0f) {
            f32 total = part->accum + dt;
            s32 n = (s32)(total / period);
            f32 used = period * (f32)n;
            part->accum = total - used;
            if (n >= 1) {
                part->vf_0xa8(used);
                list.Add(part, used);
            }
        }
    }
    for (UnkStep_7100232d4c *s = reinterpret_cast<UnkStep_7100232d4c*>(list.raw), *e = s + list.count; s != e; ++s)
        s->part->vf_0xb0(s->dt);
    fn_7100025a88(self->field_0xd0);
}


// The callback set's base-class destructor: sets the base vtable and destroys the callbacks.
void fn_710023b1c0(UnkCallbackBase* self) {
    self->~UnkCallbackBase();
}


using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkNameHolder_b0161;

struct UnkActorDef_7100232fcc {
    u8 pad_0x0[0x98];
    base::global::CStrId field_0x98;
};

struct UnkActorIds_7100232fcc {
    u8 pad_0x0[0x18];
    base::global::UnkStringData* name;  // 0x18
    base::global::UnkStringData* path;  // 0x20
    UnkReflObject* def;                 // 0x28
};

// The string id at +0x98 of the actor's definition (+0x28, cast to CActorDef), or the global empty
// id when either of the actor's own ids (+0x18, +0x20) is empty.
UnkNameHolder_b0161* fn_7100232fcc(void* object) {
    auto* self = static_cast<UnkActorIds_7100232fcc*>(object);
    const base::global::CStrId* value = &g_unk_7101d470e0;
    if (self->name != &g_unk_7101d47108->empty && self->path != &g_unk_7101d47200->empty) {
        UnkReflObject* def = self->def;
        if (def && !fn_710009d50c(reinterpret_cast<const CType*>(def->vf_0x0()),
                                  reinterpret_cast<const CType*>(fn_7100254b14())))
            def = nullptr;
        value = &reinterpret_cast<UnkActorDef_7100232fcc*>(def)->field_0x98;
    }
    return reinterpret_cast<UnkNameHolder_b0161*>(const_cast<base::global::CStrId*>(value));
}


struct UnkActor_710023259c;

struct UnkComponent_7100233230 {
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
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0(void*);
};

struct UnkSwapMap_7100233230 {
    UnkComponent_7100233230** values;
    void* ids;
    u32 count;
};

struct UnkActor_7100233230 {
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
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128(UnkComponent_7100233230*);
    u8 pad_0x8[0x98];
    UnkSwapMap_7100233230 components;  // 0xa0
};

__attribute__((always_inline)) static inline UnkComponent_7100233230** FindSlot(UnkSwapMap_7100233230* self,
                                                                          UnkComponent_7100233230* key) {
    for (u64 i = 0; i < self->count; ++i) {
        if (self->values[i] == key)
            return &self->values[i];
    }
    return &self->values[self->count];
}

struct UnkKeyedValues_71002eef6c;
using UnkValuesByKey = base::UnkKeyedArray<UnkKeyedValues_71002eef6c, u64, u64>;
u64* fn_71002332e0(UnkValuesByKey*, u64*);

bool fn_7100233230(UnkActor_710023259c* self, base::reflection::UnkReflObject* component) {
    UnkActor_7100233230* actor = reinterpret_cast<UnkActor_7100233230*>(self);
    UnkSwapMap_7100233230* map = &actor->components;
    UnkComponent_7100233230* target = reinterpret_cast<UnkComponent_7100233230*>(component);
    UnkComponent_7100233230** slot = FindSlot(map, target);
    if (slot == &map->values[map->count])
        return false;
    actor->vf_0x128(target);
    target->vf_0xe0(nullptr);
    fn_71002332e0(reinterpret_cast<UnkValuesByKey*>(map), reinterpret_cast<u64*>(slot));
    return true;
}


namespace base::reflection {
class UnkReflObject;
}
struct UnkStruct_7100233684;

struct UnkComponent_71002335d4 {
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
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0(void*, void*);
};

struct UnkActor_71002335d4 {
    u8 pad_0x0[0xa0];
    UnkComponent_71002335d4** components;  // 0xa0
    void* ids;                             // 0xa8
    u32 component_count;                   // 0xb0
    u8 pad_0xb4[0x1c];
    char field_0xd0[1];
};

// Tells the actor's main component (fn_7100233684) and then each other component.
void fn_71002335d4(UnkActor_71002335d4* self, void* a, void* b) {
    UnkComponent_71002335d4* main = reinterpret_cast<UnkComponent_71002335d4*>(
        fn_7100233684(reinterpret_cast<UnkStruct_7100233684*>(self)));
    if (main)
        main->vf_0xf0(a, b);
    for (UnkComponent_71002335d4 **it = self->components, **end = it + self->component_count; it != end; ++it) {
        if (*it != main)
            (*it)->vf_0xf0(a, b);
    }
    fn_7100025a88(self->field_0xd0);
}


struct UnkComponent_71002337f4 {
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
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8(void*);
    virtual void vf_0x100(void*);
    virtual void vf_0x108();
    virtual void vf_0x110();
    u8 pad_0x8[0x18];
    bool enabled;  // 0x20
};

struct UnkActor_71002337f4 {
    u8 pad_0x0[0xa0];
    UnkComponent_71002337f4** components;  // 0xa0
    void* ids;                             // 0xa8
    u32 component_count;                   // 0xb0
    u8 pad_0xb4[0xf4];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;
};

void fn_71002337f4(UnkActor_71002337f4* self, void* arg) {
    bool both = self->field_0x1b0 && self->field_0x1b4;
    for (UnkComponent_71002337f4 **it = self->components, **end = it + self->component_count; it != end; ++it) {
        UnkComponent_71002337f4* part = *it;
        if (self->field_0x1a8)
            part->vf_0xf8(self->field_0x1a8);
        else
            part->vf_0x100(arg);
        if (both && part->enabled) {
            if (self->field_0x1a8)
                part->vf_0x108();
            else
                part->vf_0x110();
        }
    }
}


struct UnkBlock_7100234320 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
};
static_assert(sizeof(UnkBlock_7100234320) == 0x5c, "UnkBlock_7100234320 layout");

// The object this function's `this` points 0x10 into; the block at +0x38 of it is copied unless it
// is the destination.
struct UnkObject_7100234320 {
    u8 pad_0x0[0x38];
    UnkBlock_7100234320 block;  // 0x38
};

bool fn_7100234320(void* self, UnkBlock_7100234320* dst) {
    char* base = static_cast<char*>(self);
    if (reinterpret_cast<UnkBlock_7100234320*>(base + 0x28) != dst) {
        char* raw = base - 0x10;
        // Keeps the object's address in its own register, as the original adjusts `this` first.
        asm("" : "+r"(raw));
        UnkBlock_7100234320* src = &reinterpret_cast<UnkObject_7100234320*>(raw)->block;
        dst->field_0x0 = src->field_0x0;
        dst->field_0x4 = src->field_0x4;
        dst->field_0x8 = src->field_0x8;
        dst->field_0xc = src->field_0xc;
        dst->field_0x10 = src->field_0x10;
        dst->field_0x14 = src->field_0x14;
        dst->field_0x18 = src->field_0x18;
        dst->field_0x1c = src->field_0x1c;
        dst->field_0x20 = src->field_0x20;
        dst->field_0x24 = src->field_0x24;
        dst->field_0x28 = src->field_0x28;
        dst->field_0x2c = src->field_0x2c;
        dst->field_0x30 = src->field_0x30;
        dst->field_0x34 = src->field_0x34;
        dst->field_0x38 = src->field_0x38;
        dst->field_0x3c = src->field_0x3c;
        dst->field_0x40 = src->field_0x40;
        dst->field_0x44 = src->field_0x44;
        dst->field_0x48 = src->field_0x48;
        dst->field_0x4c = src->field_0x4c;
        dst->field_0x50 = src->field_0x50;
        dst->field_0x54 = src->field_0x54;
        dst->field_0x58 = src->field_0x58;
    }
    return true;
}


struct lua_State;

using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkActor_7100233e4c {
    u8 pad_0x0[0xa0];
    UnkReflObject** values;         // 0xa0
    base::global::CStrId* ids;      // 0xa8
    u32 count;                      // 0xb0
};

// Pushes the actor component that the id names onto the script stack, when there is one.
bool fn_7100233e4c(UnkActor_7100233e4c* self, lua_State* L, const base::global::CStrId* key) {
    UnkReflObject* component = UnkParallelFind(self, *key);
    if (!component)
        return false;
    if (!fn_710009d50c(reinterpret_cast<const CType*>(component->vf_0x0()),
                       reinterpret_cast<const CType*>(fn_710024e3c8())))
        return false;
    fn_7100fbc4e0(L, component);
    return true;
}


// A callback set (src/game_71001e2acc/fn_71002475b0.cpp): 0x40 bytes with a virtual destructor.
struct UnkSet_71002486a8 {
    virtual ~UnkSet_71002486a8();
    u8 pad_0x8[0x38];
};

// A node of a hashed map (src/base/reflection/fn_710025ad0c.cpp): a string id, the array of sets
// it names, the next node further on. The map's first word is its allocator.
struct UnkNode_710025ad0c;
struct UnkNodeMap_710025ad0c;

struct UnkNodeBody_71002486a8 {
    base::global::CStrId key;
    base::UnkVector<UnkSet_71002486a8> sets;  // 0x8
};

struct UnkMapBody_71002486a8 {
    base::UnkAllocator* allocator;
};

// Removes a node: destroys its sets, releases its id and frees it.
void fn_71002486a8(UnkNodeMap_710025ad0c* map, UnkNode_710025ad0c* node) {
    if (!node)
        return;
    UnkNodeBody_71002486a8* body = reinterpret_cast<UnkNodeBody_71002486a8*>(node);
    body->sets.Free();
    body->key.Reset();
    reinterpret_cast<UnkMapBody_71002486a8*>(map)->allocator->vf_0x48(node);
}


struct UnkRcObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 field_0x8;
    };
};

struct UnkRcRef {
    UnkRcObject* ptr;

    ~UnkRcRef() { Release(); }
    UnkRcRef(const UnkRcRef& other) {
        __builtin_memset(&ptr, 0, sizeof(ptr));
        UnkRcObject* o = other.ptr;
        if (o) {
            ++o->field_0x8;
            Release();
            ptr = o;
        }
    }

    void Release() {
        UnkRcObject* o = ptr;
        if (o && --o->field_0x8 == 0)
            o->vf_0x10();
    }
};

struct UnkRcRefArray {
    UnkRcRef* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkTag_7100241b30;
template <class Tag>
struct UnkSlotArray;
void fn_7100241b30(UnkSlotArray<UnkTag_7100241b30>* self, u32 capacity);

struct UnkMember_710058644c;

// The copy assignment of an array of reference-counted pointers: the pointers released, room for
// the other's capacity, then a copy of each of its pointers appended. Returns the array.
UnkMember_710058644c* fn_7100241c78(UnkMember_710058644c* dst, const UnkMember_710058644c* src) {
    UnkRcRefArray* self = reinterpret_cast<UnkRcRefArray*>(dst);
    const UnkRcRefArray* other = reinterpret_cast<const UnkRcRefArray*>(src);
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkRcRef();
        self->count = 0;
        fn_7100241b30(reinterpret_cast<UnkSlotArray<UnkTag_7100241b30>*>(self), other->capacity);
        for (const UnkRcRef *it = other->data, *end = it + other->count; it != end; ++it) {
            u32 n = self->count;
            u32 cap = self->capacity;
            u32 new_capacity = cap;
            if (cap <= n && self->step != 0)
                new_capacity = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            fn_7100241b30(reinterpret_cast<UnkSlotArray<UnkTag_7100241b30>*>(self), new_capacity);
            new (&self->data[self->count]) UnkRcRef(*it);
            ++self->count;
        }
    }
    return dst;
}

// The copy constructor of an array of reference-counted pointers: the other's step, flags and
// allocator, room for its capacity, then a copy of each pointer appended.
void fn_7100241a1c(void* dst, const void* src) {
    UnkRcRefArray* self = static_cast<UnkRcRefArray*>(dst);
    const UnkRcRefArray* other = static_cast<const UnkRcRefArray*>(src);
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100241b30(reinterpret_cast<UnkSlotArray<UnkTag_7100241b30>*>(self), other->capacity);
    for (const UnkRcRef *it = other->data, *end = it + other->count; it != end; ++it) {
        u32 n = self->count;
        u32 cap = self->capacity;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_7100241b30(reinterpret_cast<UnkSlotArray<UnkTag_7100241b30>*>(self), new_capacity);
        new (&self->data[self->count]) UnkRcRef(*it);
        ++self->count;
    }
}


using base::global::UnkStringData;
using base::global::UnkStringPool;

extern char g_unk_7101a207b0[];

void fn_7100231fd4(void* member);  // the destructor of the component map at +0xa0
struct UnkCameraCtrl_ovb0049;

// A string id (of the pool `Pool` points to) whose destructor decrements and tests in two steps
// (the original reloads the id after the decrement).
template <UnkStringPool** Pool>
struct UnkReleasedId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedId() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};
using UnkReleasedStrId = UnkReleasedId<&g_unk_7101d47108>;
using UnkReleasedPathId = UnkReleasedId<&g_unk_7101d47200>;

struct UnkShared54Ptr {
    UnkCountedResource* ptr;

    __attribute__((always_inline)) ~UnkShared54Ptr() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

// The string id, file path and resource a path reference holds; fn_7100027044 runs before the three
// are released.
struct UnkPathRes_7100231e80 {
    UnkReleasedStrId id;      // 0x0
    UnkReleasedPathId path;   // 0x8
    UnkShared54Ptr resource;  // 0x10

    __attribute__((always_inline)) ~UnkPathRes_7100231e80() {
        fn_7100027044(reinterpret_cast<UnkStrIdPathResource*>(this));
    }
};

struct UnkComponentMap_7100231e80 {
    u8 data[0x30];
    __attribute__((always_inline)) ~UnkComponentMap_7100231e80() { fn_7100231fd4(this); }
};

struct UnkMember_7100231e80 {
    u8 data[0x1];
    __attribute__((always_inline)) ~UnkMember_7100231e80() { fn_710002572c(this); }
};

struct UnkEntityBase_7100231e80 {
    char* vtable;                    // 0x0
    u8 pad_0x8[0x8];
    char* vtable_0x10;               // 0x10
    UnkPathRes_7100231e80 path_res;  // 0x18
    UnkReleasedStrId name;           // 0x30
    u8 pad_0x38[0x68];
    UnkComponentMap_7100231e80 components;  // 0xa0
    UnkMember_7100231e80 member;            // 0xd0

    __attribute__((always_inline)) ~UnkEntityBase_7100231e80() {
        vtable = g_unk_7101a207b0 + 0x10;
        vtable_0x10 = g_unk_7101a207b0 + 0x168;
    }
};

struct UnkEntityDtor;

// The entity base destructor: its members, then the base class's.
void fn_7100231e80(UnkEntityDtor* self) {
    UnkEntityBase_7100231e80* entity = reinterpret_cast<UnkEntityBase_7100231e80*>(self);
    entity->~UnkEntityBase_7100231e80();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}


// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkXformStaticZero32 {
    u32 value;
    UnkXformStaticZero32() : value(0) {}
};

inline const UnkXformStaticZero32& UnkXformStaticZero() {
    static UnkXformStaticZero32 s_value asm("g_unk_7101d4ec40");
    return s_value;
}

// The complement of that zero's low four bits.
inline u32 UnkXformDefaultFlags() {
    static u32 s_mask asm("g_unk_7101d4ec30") = ~UnkXformStaticZero().value & 0xf;
    return s_mask;
}

struct UnkXformRecord_710023307c {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
};
static_assert(sizeof(UnkXformRecord_710023307c) == 0x5c, "UnkXformRecord_710023307c layout");

struct UnkNode_710023307c {
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
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118(const void*, const u32*);
};

// Copies the record into the node's transform (the one at +0x38), unless it is that one, and tells
// the node when asked.
void fn_710023307c(UnkNode_710023307c* self, const UnkXformRecord_710023307c* src, bool notify) {
    UnkXformRecord_710023307c* dst = reinterpret_cast<UnkXformRecord_710023307c*>(reinterpret_cast<char*>(self) + 0x38);
    if (dst != src) {
        dst->field_0x0 = src->field_0x0;
        dst->field_0x4 = src->field_0x4;
        dst->field_0x8 = src->field_0x8;
        dst->field_0xc = src->field_0xc;
        dst->field_0x10 = src->field_0x10;
        dst->field_0x14 = src->field_0x14;
        dst->field_0x18 = src->field_0x18;
        dst->field_0x1c = src->field_0x1c;
        dst->field_0x20 = src->field_0x20;
        dst->field_0x24 = src->field_0x24;
        dst->field_0x28 = src->field_0x28;
        dst->field_0x2c = src->field_0x2c;
        dst->field_0x30 = src->field_0x30;
        dst->field_0x34 = src->field_0x34;
        dst->field_0x38 = src->field_0x38;
        dst->field_0x3c = src->field_0x3c;
        dst->field_0x40 = src->field_0x40;
        dst->field_0x44 = src->field_0x44;
        dst->field_0x48 = src->field_0x48;
        dst->field_0x4c = src->field_0x4c;
        dst->field_0x50 = src->field_0x50;
        dst->field_0x54 = src->field_0x54;
        dst->field_0x58 = src->field_0x58;
    }
    if (notify) {
        alignas(8) u32 mask = UnkXformDefaultFlags();
        self->vf_0x118(dst, &mask);
    }
}


// The statics block (src/game_s7/fn_710024cc40.cpp): its first word is the mask of flags to clear.
struct UnkStatics_710024cc40 {
    u32 clear_mask;
    u8 pad_0x4[0x34];
};

struct UnkXformRecord_7100233f84 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
};
static_assert(sizeof(UnkXformRecord_7100233f84) == 0x5c, "UnkXformRecord_7100233f84 layout");

struct UnkNode_7100233f84 {
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
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118(const void*, const u32*);
};

// Copies the record's matrix into the node's transform (the one at +0x38), sets the flag 8 and
// clears the global mask's bits in its flags, and tells the node when asked.
void fn_7100233f84(UnkNode_7100233f84* self, const UnkXformRecord_7100233f84* src, bool notify) {
    UnkXformRecord_7100233f84* dst = reinterpret_cast<UnkXformRecord_7100233f84*>(reinterpret_cast<char*>(self) + 0x38);
    dst->field_0x0 = src->field_0x0;
    dst->field_0x4 = src->field_0x4;
    dst->field_0x8 = src->field_0x8;
    dst->field_0xc = src->field_0xc;
    dst->field_0x10 = src->field_0x10;
    dst->field_0x14 = src->field_0x14;
    dst->field_0x18 = src->field_0x18;
    dst->field_0x1c = src->field_0x1c;
    dst->field_0x20 = src->field_0x20;
    dst->field_0x24 = src->field_0x24;
    dst->field_0x28 = src->field_0x28;
    dst->field_0x2c = src->field_0x2c;
    dst->field_0x30 = src->field_0x30;
    dst->field_0x34 = src->field_0x34;
    dst->field_0x38 = src->field_0x38;
    dst->field_0x3c = src->field_0x3c;
    dst->field_0x58 = (dst->field_0x58 | 8) & ~g_unk_717269f420.clear_mask;
    if (notify) {
        alignas(8) u32 mask = UnkXformDefaultFlags();
        self->vf_0x118(dst, &mask);
    }
}


using base::global::CStrId;
using base::global::UnkStringData;

struct UnkKeyedStrIdMapA_ovb0062;
void fn_71000269c4(UnkStrIdPathResource* self, UnkCountedResource* resource);

struct UnkActor_7100232160;
struct UnkEntryDef_7100232160 {
    u8 pad_0x0[8];
    CStrId name;  // 0x8
};

struct UnkEntryComponent_7100232160 {
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
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0(UnkActor_7100232160*);
    u8 pad_0x8[8];
    UnkEntryDef_7100232160* def;  // 0x10
};

struct UnkEntry_7100232160 {
    u8 pad_0x0[0x21];
    bool active;  // 0x21
};

UnkEntryComponent_7100232160* fn_710024f71c(UnkEntry_7100232160** entry);

struct UnkResourceList_7100232160 {
    u8 pad_0x0[0xa0];
    UnkEntry_7100232160** entries;  // 0xa0
    u32 entry_count;                // 0xa8
};

struct UnkSource_7100232160 {
    CStrId name;                              // 0x0
    UnkResourceList_7100232160* resource;     // 0x8
};

struct UnkActor_7100232160 {
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
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120(UnkEntryComponent_7100232160*);
    u8 pad_0x8[0x10];
    UnkStrIdPathResource path_res;  // 0x18
    CStrId name;                    // 0x30
    u8 pad_0x38[0x68];
    UnkEntryComponent_7100232160** values;  // 0xa0
    CStrId* keys;                           // 0xa8
    u32 count;                              // 0xb0
    u32 capacity;                           // 0xb4
    u32 step;                               // 0xb8
};

// Takes the source's name and resource, then adds a component for each active entry the resource lists.
bool fn_7100232160(UnkActor_7100232160* self, const UnkSource_7100232160* other) {
    if (self->path_res.ids.id.data != &g_unk_7101d47108->empty && self->path_res.ids.path.data != &g_unk_7101d47200->empty)
        return false;
    self->name = other->name;
    fn_71000269c4(&self->path_res, reinterpret_cast<UnkCountedResource*>(other->resource));
    UnkResourceList_7100232160* list = other->resource;
    for (UnkEntry_7100232160 **it = list->entries, **end = it + list->entry_count; it != end; ++it) {
        if (!(*it)->active)
            continue;
        UnkEntry_7100232160* entry = *it;
        UnkEntryComponent_7100232160* component = fn_710024f71c(&entry);
        if (!component)
            continue;
        component->vf_0xe0(self);
        UnkEntryDef_7100232160* def = component->def;
        if (self->count == self->capacity)
            fn_7100248ce8(reinterpret_cast<UnkKeyedStrIdMapA_ovb0062*>(&self->values), self->step + self->count);
        CStrId* keys = self->keys;
        UnkStringData* data = def->name.data;
        __builtin_memcpy(&keys[self->count], &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
        // The count is reloaded after this store.
        __builtin_memcpy(&self->values[self->count], &component, sizeof(component));
        ++self->count;
        self->vf_0x120(component);
    }
    return true;
}


struct UnkTag_7100240594;
template <>
struct base::UnkPoolListTraits<UnkTag_7100240594> {
    static char* vtable() { return g_unk_7101a20a30; }
    static char* base_vtable() { return g_unk_7101a20a50; }
};

using UnkPoolList_7100240594 = base::UnkPoolList<UnkTag_7100240594, 8, 0x10>;

// A list node: a value and the next node.
struct UnkNode_7100240594 {
    u64 value;
    UnkNode_7100240594* next;
};

// The copy constructor of a list over its own block pool: a pool of the other's size, then a node
// for each of the other's values. A list with no pool of its own shares the other's allocator.
void fn_7100240594(UnkPoolList_7100240594* self, const UnkPoolList_7100240594* other) {
    self->UnkPoolListBase<UnkTag_7100240594, 8>::Init(0x13, nullptr);
    self->vtable = g_unk_7101a20a30 + 0x10;
    self->pool.Init();
    self->pool.field_0x38 = 0;
    self->pool.field_0x3c = false;
    u32 flags = other->flags;
    self->flags = flags;
    base::UnkAllocator* parent = other->pool.parent;
    if (!parent) {
        self->allocator = other->allocator;
        return;
    }
    u64 other_size = other->pool.blocks.size;
    self->pool.Release();
    u64 size = other_size & 0xffffffff0;
    if (size) {
        self->pool.parent = parent;
        fn_710008378c(&self->pool.blocks, parent->vf_0x40(size, 8, flags), size, 0x10);
    }
    self->allocator = reinterpret_cast<base::UnkAllocator*>(reinterpret_cast<u64>(self) + 0x30);
    if (self == other)
        return;
    self->FreeNodes();
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    base::UnkSList<UnkTag_7100240594>* list = reinterpret_cast<base::UnkSList<UnkTag_7100240594>*>(self);
    for (const UnkNode_7100240594* n = reinterpret_cast<const UnkNode_7100240594*>(other->head); n; n = n->next)
        list->PushBack(n->value);
}


struct UnkLinkHandle_b54a1 {
    u8 pad_0x0[0x10];
    void* target;
};

// A link, as src/game/CActor/UnkLinkVectorEqual_b54.cpp has it.
struct UnkLink_7100245c4c {
    u8 pad_0x0[0x8];
    u8 key[0x28];
    u32 state;
    u32 pad_0x34;
    UnkLinkHandle_b54a1* handle;

    void* Get() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(key);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&handle),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return handle ? handle->target : nullptr;
    }
};
static_assert(sizeof(UnkLink_7100245c4c) == 0x40, "UnkLink_7100245c4c layout");

// The cached (index, link) value of an iterator: owning the pair, or referring to it.
struct UnkIterValue_7100245c4c {
    struct Value {
        u32 index;
        UnkLink_7100245c4c element;
    };
    struct Ref {
        const u32* index;
        UnkLink_7100245c4c* element;
    };
    union {
        Value value;
        Ref ref;
    };
    s32 state;  // 0 empty, 1 owns `value`, 2 refers through `ref`

    u32 Index() const { return state == 2 ? *ref.index : value.index; }
    UnkLink_7100245c4c* Element() { return state == 2 ? ref.element : &value.element; }
};
static_assert(sizeof(UnkIterValue_7100245c4c) == 0x50, "UnkIterValue_7100245c4c layout");

// Whether two iterator values are at the same index and hold links to the same target.
bool fn_7100245c4c(UnkIterValue_7100245c4c* a, UnkIterValue_7100245c4c* b) {
    if (a->Index() != b->Index())
        return false;
    UnkLink_7100245c4c* x = a->Element();
    UnkLink_7100245c4c* y = b->Element();
    void* target = y->Get();
    return x->Get() == target;
}

bool fn_710024612c(UnkIterValue_7100245c4c* a, UnkIterValue_7100245c4c* b) {
    if (a->Index() != b->Index())
        return false;
    UnkLink_7100245c4c* x = a->Element();
    UnkLink_7100245c4c* y = b->Element();
    void* target = y->Get();
    return x->Get() == target;
}


// The cached (index, callback set) value of an iterator: owning the pair, or referring to it.
struct UnkIterValue_71002462dc {
    struct Value {
        u32 index;
        UnkCallbackSet_7101a20950 element;

        Value(u32 i, const UnkCallbackSet_7101a20950& e) : index(i), element(e) {}
    };
    struct Ref {
        const u32* index;
        UnkCallbackSet_7101a20950* element;
    };
    union {
        Value value;
        Ref ref;
    };
    s32 state;  // 0 empty, 1 owns `value`, 2 refers through `ref`
};
static_assert(sizeof(UnkIterValue_71002462dc) == 0x50, "UnkIterValue_71002462dc layout");

// The copy constructor: an owned copy of the pair the other one owns or refers to.
__attribute__((disable_tail_calls)) void fn_71002462dc(UnkIterValue_71002462dc* self, const UnkIterValue_71002462dc* other) {
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        new (&self->value) UnkIterValue_71002462dc::Value(other->value.index, other->value.element);
    } else if (other->state == 2) {
        self->state = 1;
        UnkIterValue_71002462dc::Ref r = other->ref;
        new (&self->value) UnkIterValue_71002462dc::Value(*r.index, *r.element);
    }
}


// The cached (index, callback set) value of an iterator: owning the pair, or referring to it.
struct UnkIterValue_7100245dfc {
    struct Value {
        u32 index;
        UnkCallbackSet_7101a20950 element;
    };
    struct Ref {
        const u32* index;
        UnkCallbackSet_7101a20950* element;
    };
    union {
        Value value;
        Ref ref;
    };
    s32 state;  // 0 empty, 1 owns `value`, 2 refers through `ref`

    u32& Index() { return state == 2 ? *const_cast<u32*>(ref.index) : value.index; }
    UnkCallbackSet_7101a20950& Element() { return state == 2 ? *ref.element : value.element; }
};
static_assert(sizeof(UnkIterValue_7100245dfc) == 0x50, "UnkIterValue_7100245dfc layout");

// The set's move assignment: shares the owner, takes the flag and the callbacks, and leaves the
// source empty.
static inline void MoveAssign(UnkCallbackSet_7101a20950& dst, UnkCallbackSet_7101a20950& src) {
    base::UnkRefHandle* handle = reinterpret_cast<base::UnkRefHandle*>(&src.owner);
    fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&dst.owner), handle);
    dst.field_0x30 = src.field_0x30;
    UnkErasedVector* callbacks = &src.callbacks;
    fn_710023b29c(&dst.callbacks, callbacks);
    src.field_0x30 = 0;
    fn_7100ad3588(callbacks);
    if (UnkRefCounted* p = src.owner) {
        if (--p->ref_count == 0)
            p->Delete();
        handle->ptr = nullptr;
    }
}

// The move assignment: the pair the other one owns or refers to goes to the pair this one owns
// or refers to.
UnkIterValue_7100245dfc* fn_7100245dfc(UnkIterValue_7100245dfc* self, UnkIterValue_7100245dfc* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        MoveAssign(self->Element(), other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        UnkCallbackSet_7101a20950& source = *other->ref.element;
        MoveAssign(self->Element(), source);
    }
    return self;
}


struct UnkCountedLink_b0226 : base::UnkRefCounted {
    void* field_0x10;
};

// The callback set behind a lazily resolved handle: callbacks at +0x8, the state at +0x30 (1: not
// resolved yet, 2: resolved) and the handle at +0x38. The base copy constructor is
// fn_7100da4dcc (the same as src/game/CLogicCamera/CLogicCamera_Value_ovb0070.cpp's).
struct UnkCameraCallbackBase_ovb0070 {
    void** vtable;
    UnkOwnedErasedVector callbacks;
    u32 state;

    UnkCameraCallbackBase_ovb0070(const UnkCameraCallbackBase_ovb0070& other) asm("fn_7100da4dcc");
    ~UnkCameraCallbackBase_ovb0070() { vtable = &g_unk_7101a20998[2]; }
};

struct UnkLinkCallbackSet_b0226 : UnkCameraCallbackBase_ovb0070 {
    base::UnkRefHandle handle;

    // The copy resolves its own handle from the source.
    UnkLinkCallbackSet_b0226(const UnkLinkCallbackSet_b0226& other) : UnkCameraCallbackBase_ovb0070(other) {
        vtable = &g_unk_7101a20950[2];
        handle.ptr = nullptr;
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(&callbacks);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            if (handle.ptr && static_cast<UnkCountedLink_b0226*>(handle.ptr)->field_0x10)
                state = 2;
        }
    }
    ~UnkLinkCallbackSet_b0226() {
        vtable = &g_unk_7101a20950[2];
        if (base::UnkRefCounted* p = handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkLinkCallbackSet_b0226& operator=(UnkLinkCallbackSet_b0226&& other) {
        fn_7100234f78(&handle, &other.handle);
        state = other.state;
        UnkErasedVector* from = &other.callbacks;
        fn_710023b29c(&callbacks, from);
        other.state = 0;
        fn_7100ad3588(from);
        if (base::UnkRefCounted* p = other.handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
            other.handle.ptr = nullptr;
        }
        return *this;
    }
};

// The copy assignment: through a resolved copy of `other`, moved in.
UnkLinkCallbackSet_b0226* fn_7100248b38(UnkLinkCallbackSet_b0226* self, const UnkLinkCallbackSet_b0226* other) {
    if (self != other) {
        UnkLinkCallbackSet_b0226 copy(*other);
        *self = static_cast<UnkLinkCallbackSet_b0226&&>(copy);
    }
    return self;
}
