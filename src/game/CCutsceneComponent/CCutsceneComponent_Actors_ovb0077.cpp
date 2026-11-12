// CCutsceneComponent's unit (tools/tu_map.py): the component's actor entries (a base::UnkVector of 0x88-byte
// polymorphic entries at +0xd0, each with a reflected property that resolves once into a weak handle to an
// actor), their updates and the cutscene manager calls on them; the entries' copy (fn_71007abba4), copy
// assignment and the vector's copy, assignment, PushBack and Insert; a reflected CStrId getter, and a CStrId
// set from a number's text. Names and layouts are placeholders.
// The flag array's element accessor (base::UnkVector::GrowAt) is inlined in this unit.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include <new>
#include <string>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CCutsceneComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU341.h"
#include "game/UnkTU83.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStrId;
using base::reflection::UnkReflValue;

// A reflected getter: a copy of the CStrId at +0x148 of the object at 8, as a reflected value.
UnkReflValue fn_71007ad4d0(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    base::global::CStrId value = *reinterpret_cast<base::global::CStrId*>(target + 0x148);
    const base::reflection::CType* type = fn_7100009658();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

// Releases the id, then takes the other's string (the other gets the empty id).
static inline void Take(CStrId* self, UnkStrId&& other) {
    self->Reset();
    self->data = other.data;
    other.data = &g_unk_7101d47108->empty;
}

// The id set from an unsigned number's decimal text.
CStrId* fn_71007ab7d8(CStrId* self, const u32* value) {
    self->Reset();
    Take(self, UnkStrId(std::to_string(*value).c_str(), true));
    return self;
}

// A byte array with the engine array's layout, assigned with memcpy.
struct UnkByteArray_ovb0077 {
    u8* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkByteArray_ovb0077& operator=(const UnkByteArray_ovb0077& o) {
        if (this != &o) {
            count = 0;
            u32 n = o.capacity;
            u8* p;
            if (capacity < n) {
                p = static_cast<u8*>(allocator->vf_0x40(n, 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, count);
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = n;
            } else {
                p = data;
            }
            __builtin_memcpy(p, o.data, o.count);
            count = o.count;
        }
        return *this;
    }
};

class UnkSub_ovb0077 {
public:
    virtual void vf_0x0();

    UnkSub_ovb0077& operator=(const UnkSub_ovb0077& o) {
        const base::UnkRefHandle* src = &o.handle;
        fn_7100234f78(&handle, src);
        field_0x68 = o.field_0x68;
        fn_710023b29c(&values, &o.values);
        return *this;
    }

    UnkErasedVector values;
    u32 field_0x68;
    base::UnkRefHandle handle;
};

// The entry fn_71007abba4 copies (its copy assignment).
class UnkCutsceneEntry_ovb0077 {
public:
    virtual void vf_0x0();

    __attribute__((always_inline)) UnkCutsceneEntry_ovb0077& operator=(const UnkCutsceneEntry_ovb0077&) = default;

    UnkByteArray_ovb0077 buffer;
    CStrId id;
    UnkSub_ovb0077 sub;
    u32 field_0x78;
    u32 field_0x7c;
    u8 field_0x80;
    u8 field_0x81;
    u8 field_0x82;
};
static_assert(__builtin_offsetof(UnkCutsceneEntry_ovb0077, field_0x78) == 0x78, "layout");

// Declared with a void return elsewhere (src/clones/clones_02.cpp); the original returns self.
void* fn_71007abd58(void* self, const void* other) {
    return &(*static_cast<UnkCutsceneEntry_ovb0077*>(self) = *static_cast<const UnkCutsceneEntry_ovb0077*>(other));
}

struct UnkCutsceneEntry_A5_b68;

// A 0x88-byte polymorphic entry seen from outside: copied by fn_71007abba4 and destroyed
// through its vtable's slot 1.
struct UnkCutsceneEntryRef_ovb0077 {
    void (**vtable)(UnkCutsceneEntryRef_ovb0077*);
    u8 pad_0x8[0x80];

    __attribute__((always_inline)) UnkCutsceneEntryRef_ovb0077(const UnkCutsceneEntryRef_ovb0077& other) {
        fn_71007abba4(reinterpret_cast<UnkCutsceneEntry_A5_b68*>(this),
                      reinterpret_cast<UnkCutsceneEntry_A5_b68*>(const_cast<UnkCutsceneEntryRef_ovb0077*>(&other)));
    }
    __attribute__((always_inline)) ~UnkCutsceneEntryRef_ovb0077() { vtable[1](this); }
};

using Entry = UnkCutsceneEntryRef_ovb0077;

// base::UnkVector's MoveTo for these entries (copied by fn_71007abba4, then destroyed).
__attribute__((always_inline)) static inline Entry* MoveTo(base::UnkVector<Entry>* self, u32 capacity) {
    Entry* p = static_cast<Entry*>(self->allocator->vf_0x40((u64)capacity * sizeof(Entry), 8, self->flags));
    if (self->data) {
        Entry* src = self->data;
        Entry* dst = p;
        for (u32 i = 0; i < self->count; ++i) {
            new (dst) Entry(*src);
            Entry* old = src++;
            old->~Entry();
            ++dst;
        }
        self->allocator->vf_0x48(self->data);
    }
    return p;
}

// base::UnkVector::PushBackCopy.
__attribute__((always_inline)) static inline void PushBack(base::UnkVector<Entry>* self, const Entry* value) {
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    if (old_capacity < new_capacity) {
        Entry* p = MoveTo(self, new_capacity);
        n = self->count;
        self->data = p;
        self->capacity = new_capacity;
    }
    new (&self->data[n]) Entry(*value);
    ++self->count;
}

void fn_71007acd14(base::UnkVector<Entry>* self, const Entry* value) {
    PushBack(self, value);
}

// The copy constructor: the other's settings, its capacity reserved and each entry pushed.
void fn_71007ac4ac(base::UnkVector<Entry>* self, const base::UnkVector<Entry>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    if (u32 n = other->capacity) {
        Entry* p = MoveTo(self, n);
        self->data = p;
        self->capacity = n;
    }
    for (const Entry *p = other->data, *end = p + other->count; p != end; ++p)
        PushBack(self, p);
}

void fn_71007acb7c(base::UnkVector<Entry>* self, const Entry* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_71007acd14(self, value);
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0) {
        u32 grown = (n / self->step + 1) * self->step;
        if (cap < grown) {
            Entry* p = MoveTo(self, grown);
            n = self->count;
            self->data = p;
            self->capacity = grown;
        }
    }
    for (u32 i = n - 1; i >= index; --i) {
        Entry* q = &self->data[i];
        new (q + 1) Entry(*q);
        self->data[i].~Entry();
        if (i == 0)
            break;
    }
    new (&self->data[index]) Entry(*value);
    ++self->count;
}

// Copy assignment: the entries are destroyed, the other's capacity reserved and each entry pushed.
base::UnkVector<Entry>* fn_71007ac6c8(base::UnkVector<Entry>* self, const base::UnkVector<Entry>* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~Entry();
        self->count = 0;
        u32 n = other->capacity;
        if (self->capacity < n) {
            Entry* p = MoveTo(self, n);
            self->data = p;
            self->capacity = n;
        }
        for (const Entry *p = other->data, *end = p + other->count; p != end; ++p)
            PushBack(self, p);
    }
    return self;
}

using base::reflection::UnkReflObject;

struct UnkElem_0x88;
UnkElem_0x88& fn_71007a6b58(base::UnkVector<UnkElem_0x88>* v, u32 index);

struct UnkStruct_71002334fc;
void fn_71011991c4(void* gui, UnkReflObject* entity);
void fn_7101198450(void* gui, UnkReflObject* entity);
u32 fn_71011986e0(void* manager, const CFilePathStrId* path);
u32 fn_710119882c(void* manager, const CFilePathStrId* path);

struct UnkCutsceneActorObject_ovb0077 : UnkReflObject {
    u8 pad_0x8[0x28];
    base::global::CStrId name;  // 0x30
    u8 pad_0x38[0x17b];
    bool field_0x1b3;
};

// A weak handle's view: the object it points at (null once it is gone).
struct UnkCutsceneActorHandle_ovb0077 {
    u8 pad_0x0[0x10];
    UnkCutsceneActorObject_ovb0077* target;  // 0x10
};

// A reflected property naming an actor, resolved once into a weak handle to it.
struct UnkCutsceneActorRef_ovb0077 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual bool vf_0x18();
    virtual bool vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();

    u8 values[0x28];  // 0x8
    s32 state;        // 0x30: 0 none, 1 unresolved, 2 resolved
    union {
        base::UnkRefHandle handle;  // 0x38
        UnkCutsceneActorHandle_ovb0077* actor;
    };
};

// A cutscene actor entry.
struct UnkCutsceneActor_ovb0077 {
    u8 pad_0x0[0x30];
    base::global::CStrId name;        // 0x30
    UnkCutsceneActorRef_ovb0077 ref;  // 0x38
    u8 pad_0x78[0x8];
    bool field_0x80;
    bool field_0x81;
};

static inline void Resolve(UnkCutsceneActorRef_ovb0077* e) {
    {
        UnkRefPtr r = fn_710023b580(e->values);
        fn_7100234f78(&e->handle, reinterpret_cast<const base::UnkRefHandle*>(&r));
    }
    e->state = 2;
}

static inline UnkCutsceneActorObject_ovb0077* Target(UnkCutsceneActorHandle_ovb0077* h) {
    return h ? h->target : nullptr;
}

static inline void Update(UnkCutsceneActorRef_ovb0077* e) {
    if (e->state == 1)
        Resolve(e);
}

static inline UnkCutsceneActorObject_ovb0077* Get(UnkCutsceneActorRef_ovb0077* e) {
    Update(e);
    return Target(e->actor);
}

struct UnkCutsceneRecord_ovb0077 {
    u8 pad_0x0[0x10];
    struct {
        u8 pad_0x0[0x20];
        bool enabled;
    } * object;  // 0x10
    u8 pad_0x18[0x8];
    UnkReflObject* key;  // 0x20
    u8 pad_0x28[0x8];
};

// A shared object (an atomic count at +8, released through vtable slot 2).
struct UnkCutsceneRecordList_ovb0077 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
    u8 pad_0xc[0x124];
    u32 field_0x130;  // 0x130
    u8 pad_0x134[0x1c];
    UnkCutsceneRecord_ovb0077* data;  // 0x150
    u32 count;                        // 0x158
    u8 pad_0x15c[0x1b4];
    u8 field_0x310[0x8];  // 0x310
};

struct UnkVec3_cs;
struct UnkPathIdMember;
void fn_710119a110(void* gui, const UnkPathIdMember* object);

struct UnkCutsceneComponent_ovb0077 {
    u8 pad_0x0[0x58];
    u8 offsets[0x8];  // 0x58: an array of three-word offsets
    u32 offset_count;  // 0x60
    u8 pad_0x64[0x6c];
    base::UnkVector<UnkElem_0x88> actors;  // 0xd0
    u8 pad_0xf8[0x58];
    base::global::CFilePathStrId path;  // 0x150
    u8 pad_0x158[0x8];
    base::UnkVector<u8> flags;  // 0x160
    u8 pad_0x188[0x50];
    bool field_0x1d8;  // 0x1d8
    bool field_0x1d9;  // 0x1d9
    u8 pad_0x1da[0x6];
    UnkCutsceneActorHandle_ovb0077* main_actor;  // 0x1e0
    u8 pad_0x1e8[0x18];
    UnkCutsceneRecordList_ovb0077* records;  // 0x200
    u8 pad_0x208[0x7];
    bool field_0x20f;     // 0x20f
    bool has_main_actor;  // 0x210
};

static inline UnkCutsceneActor_ovb0077* ActorAt(UnkCutsceneComponent_ovb0077* self, u32 index) {
    return reinterpret_cast<UnkCutsceneActor_ovb0077*>(&fn_71007a6b58(&self->actors, index));
}

static inline void Disable(UnkCutsceneRecordList_ovb0077* list, UnkCutsceneActorObject_ovb0077* target) {
    for (UnkCutsceneRecord_ovb0077 *p = list->data, *end = p + list->count; p != end; ++p) {
        if (p->key != nullptr ? target == p->key : false) {
            p->object->enabled = false;
            return;
        }
    }
}

// Whether the property names an actor that still exists (resolving it first).
static inline bool IsValid(UnkCutsceneActorRef_ovb0077* e) {
    switch (e->state) {
    case 0:
        return false;
    case 1:
        Resolve(e);
        return e->actor && e->actor->target;
    case 2:
        return e->actor && e->actor->target;
    }
    return e->actor && e->actor->target;
}

void fn_71007a95f8(UnkCutsceneComponent_ovb0077* self, u32 index, bool flag, bool alt, bool from_entry) {
    bool f = from_entry ? ActorAt(self, index)->field_0x81 : flag;
    UnkCutsceneActor_ovb0077* e = ActorAt(self, index);
    if (!IsValid(&e->ref))
        return;
    UnkCutsceneActorObject_ovb0077* actor = Get(&e->ref);
    if (!actor)
        return;
    fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(actor), f);
    UnkReflObject* entity = fn_710009d50c(actor->vf_0x0(), fn_7100d70c20()) ? actor : nullptr;
    if (alt)
        fn_71011991c4(g_unk_7101d47330, entity);
    else
        fn_7101198450(g_unk_7101d47330, entity);
}

void fn_71007a6214(UnkCutsceneComponent_ovb0077* self) {
    u32 n = self->actors.count;
    for (u32 i = 0; i < n; ++i)
        fn_71007a95f8(self, i, false, false, true);
    if (self->has_main_actor) {
        if (UnkCutsceneActorObject_ovb0077* actor = Target(self->main_actor)) {
            fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(actor), false);
            UnkReflObject* entity = fn_710009d50c(actor->vf_0x0(), fn_7100d70c20()) ? actor : nullptr;
            fn_7101198450(g_unk_7101d47330, entity);
        }
    }
    for (u32 i = 0; i < self->offset_count; ++i) {
        u32* dst = reinterpret_cast<u32*>(fn_71000cc374(self->offsets, i));
        const u32* src = reinterpret_cast<const u32*>(&g_unk_710164779c);
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
    }
    if (self->records && __atomic_load_n(&self->records->ref_count, __ATOMIC_SEQ_CST) != 0) {
        fn_710119a110(g_unk_7101d47330, reinterpret_cast<const UnkPathIdMember*>(self->records->field_0x310));
        if (UnkCutsceneRecordList_ovb0077* p = self->records) {
            __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
                p->vf_0x10();
            self->records = nullptr;
        }
    }
}

void fn_71007a9270(UnkCutsceneComponent_ovb0077* self) {
    if (self->has_main_actor)
        Disable(self->records, Target(self->main_actor));
    UnkCutsceneActor_ovb0077* e = reinterpret_cast<UnkCutsceneActor_ovb0077*>(self->actors.data);
    for (UnkCutsceneActor_ovb0077* end = e + self->actors.count; e != end; ++e)
        Disable(self->records, Get(&e->ref));
}

void fn_71007a9728(UnkCutsceneComponent_ovb0077* self) {
    u32 n = self->actors.count;
    for (u32 i = 0; i < n; ++i) {
        UnkCutsceneActorRef_ovb0077* ref = &ActorAt(self, i)->ref;
        if (Get(ref) && ref->vf_0x18())
            ActorAt(self, i)->field_0x81 = Get(ref)->field_0x1b3;
    }
    if (self->has_main_actor)
        self->field_0x1d9 = self->main_actor->target->field_0x1b3;
}

void fn_71007a941c(UnkCutsceneComponent_ovb0077* self) {
    if (self->field_0x20f)
        return;
    for (u32 i = 0; i < self->actors.count; ++i) {
        UnkCutsceneActor_ovb0077* e = ActorAt(self, i);
        UnkCutsceneActorRef_ovb0077* ref = &e->ref;
        if (ref->vf_0x20())
            continue;
        if (!ref->vf_0x18())
            ref->vf_0x30();
        if (UnkCutsceneActorObject_ovb0077* actor = Get(ref)) {
            if (e->name.data == g_unk_7101d470e0.data)
                e->name = actor->name;
            e->field_0x80 = actor->field_0x1b3;
        }
    }
    self->field_0x20f = true;
    if (self->has_main_actor)
        self->field_0x1d8 = self->main_actor->target->field_0x1b3;
}

// Whether the cutscene manager has the component's path loaded (fn_71007a6d6c).
static inline bool IsLoaded(UnkCutsceneComponent_ovb0077* self) {
    void* manager = g_unk_7101d47330;
    base::global::CFilePathStrId path(self->path);
    return fn_71011986e0(manager, &path) != 0;
}

static inline bool IsReady(UnkCutsceneComponent_ovb0077* self) {
    void* manager = g_unk_7101d47330;
    base::global::CFilePathStrId path(self->path);
    return fn_710119882c(manager, &path) != 0;
}

bool fn_71007aa610(UnkCutsceneComponent_ovb0077* self) {
    if (self->records && (!IsLoaded(self) || (IsLoaded(self) && !IsReady(self)))) {
        fn_71007a8280(self);
        return true;
    }
    return false;
}

// Resizes the array to n elements, zeroing the new ones.
static inline void Resize(base::UnkVector<u8>* v, u32 n) {
    if (v->count < n) {
        if (v->capacity < n) {
            u8* p = static_cast<u8*>(v->allocator->vf_0x40(n, 8, v->flags));
            if (v->data) {
                __builtin_memcpy(p, v->data, v->count);
                v->allocator->vf_0x48(v->data);
            }
            v->data = p;
            v->capacity = n;
        }
        __builtin_memset(v->data + v->count, 0, n - v->count);
        v->count = n;
    } else if (v->count > n) {
        v->count = n;
    }
}

void fn_71007a9048(UnkCutsceneComponent_ovb0077* self) {
    u32 n = self->records->field_0x130;
    Resize(&self->flags, n);
    for (u32 i = 0; i < n; ++i)
        self->flags.GrowAt(i) = 1;
}
