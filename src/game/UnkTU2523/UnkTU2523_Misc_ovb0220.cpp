// Unit 2523 (tools/tu_map.py hint 'CEntity'; no reliable name): a collection of the unit's matched functions,
// grouped in one file. Singly linked list insertion at an index (two copies), a script command that plays a
// cue, the release of a holder of callback sets, the entity's step, string-id keyed maps (swap-remove, copy
// assignment), the iterator values of arrays of callback sets (move assignment, equality) and a message send. Names and layouts are
// placeholders.
#include <new>

#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern void* g_unk_7101a20998[];  // the callback sets' base vtable
extern void* g_unk_7101a33fa0[];  // their vtable

// ---- Insertion at an index of two singly linked lists (the shape of fn_710023fedc,
// src/game/CActor/CActor_SListInsertAt_A1_b74.cpp) ----
struct UnkClone_fn_7100d83598;
struct UnkClone_fn_7100d836b0;
using UnkSListA_b0220 = base::UnkSList<UnkClone_fn_7100d83598>;
using UnkSListB_b0220 = base::UnkSList<UnkClone_fn_7100d836b0>;

extern template void UnkSListA_b0220::InsertBefore(const u64&, UnkSListA_b0220::Node*);
extern template void UnkSListB_b0220::InsertBefore(const u64&, UnkSListB_b0220::Node*);

template <class List>
static inline __attribute__((always_inline)) typename List::Node* At_b0220(List* self, u32 i) {
    typename List::Node* n = self->head;
    for (u32 k = 0; k < self->count; ++k, n = n->next)
        if (k == i)
            return n;
    return nullptr;
}

void fn_7100d82ee0(UnkSListA_b0220* self, const u32* it, const u64* v) {
    u32 i = *it;
    if (i >= self->count) {
        self->PushBack(*v);
        return;
    }
    self->InsertBefore(*v, At_b0220(self, i));
}

void fn_7100d82f9c(UnkSListB_b0220* self, const u32* it, const u64* v) {
    u32 i = *it;
    if (i >= self->count) {
        self->PushBack(*v);
        return;
    }
    self->InsertBefore(*v, At_b0220(self, i));
}

// ---- A script command (as fn_7100d7cd4c, UnkTU2523_PlayCue_ld7cd4c.cpp): plays a cue from a file path ----
struct UnkCueEntity_b0220 {
    u8 pad_0x0[0x208];
    void* field_0x208;
    void* field_0x210;
    u8 pad_0x218[0xa0];
    void* field_0x2b8;
};

struct UnkPlayer;
struct UnkEntitySoundComponent;
UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_710071a524(UnkEntitySoundComponent*, void*, f32, f32);
s64 fn_710007cf30(void* stream, const char* format, ...);

bool fn_7100d7cc68(UnkCueEntity_b0220* self, void* stream) {
    char buffer[128];
    fn_710007cf30(stream, "s", buffer);
    base::global::CFilePathStrId path(buffer, true);
    UnkCueEntity_b0220* entity = self;
    for (;;) {
        if (entity->field_0x2b8) {
            fn_710071a524(static_cast<UnkEntitySoundComponent*>(entity->field_0x2b8), &path, 0.0f, 0.0f);
            break;
        }
        if (!entity->field_0x208 && !entity->field_0x210)
            break;
        entity = reinterpret_cast<UnkCueEntity_b0220*>(fn_7100dec788(g_unk_7101d47310));
        if (!entity)
            break;
    }
    return true;
}

// ---- The release of a holder of a string id and an array of 0x40-byte callback sets: the elements are
// destroyed through their vtable (slot 0) ----
struct UnkElem64_b0220 {
    virtual ~UnkElem64_b0220();
    u8 pad_0x8[0x38];
};
static_assert(sizeof(UnkElem64_b0220) == 0x40, "UnkElem64_b0220 layout");

struct UnkOwner_b0220 {
    base::UnkAllocator* allocator;
};

struct UnkListHolder_b0220 {
    CStrId id;
    base::UnkVector<UnkElem64_b0220> items;
};

void fn_7100d89f0c(UnkOwner_b0220* owner, UnkListHolder_b0220* holder) {
    if (!holder)
        return;
    holder->items.Free();
    holder->id.Reset();
    owner->allocator->vf_0x48(holder);
}

// ---- The entity's step: counts down the timer at +0x3ac, then the state at +0x30c ----
struct UnkEntityUpd_b0220 {
    u8 pad_0x0[0x1b5];
    bool flag_0x1b5;
    u8 pad_0x1b6[0x228 - 0x1b6];
    void* field_0x228;
    void* field_0x230;
    u8 pad_0x238[0x308 - 0x238];
    u32 field_0x308;
    u32 field_0x30c;
    bool flag_0x310;
    bool flag_0x311;
    u8 pad_0x312[0x3ac - 0x312];
    f32 field_0x3ac;
};
static_assert(__builtin_offsetof(UnkEntityUpd_b0220, field_0x3ac) == 0x3ac, "layout");

void fn_7100232c68(void* self);
void fn_7100232c90(UnkEntityUpd_b0220* self, f32 dt, bool flag);
bool fn_7101173964(void* object);
bool fn_71007298d0(void* object);
struct UnkActor;
bool fn_7100d7bea0(UnkActor* self, bool flag);

void fn_7100d7b068(UnkEntityUpd_b0220* self, f32 dt, bool flag) {
    if (self->field_0x3ac > 0.0f) {
        self->field_0x3ac -= dt;
        if (self->field_0x3ac <= 0.0f) {
            self->field_0x3ac = -1.0f;
            fn_7100232c68(self);
        }
    }
    if (self->flag_0x1b5)
        return;
    bool alive;
    if (self->field_0x228)
        alive = fn_7101173964(self->field_0x228);
    else if (self->field_0x230)
        alive = fn_71007298d0(self->field_0x230);
    else
        alive = false;
    u32 count = 0;
    if (alive) {
        count = self->field_0x30c + 1;
        self->field_0x30c = count;
    }
    self->field_0x30c = count;
    if (self->flag_0x310) {
        if (self->flag_0x311) {
            bool clear;
            if (self->field_0x228)
                clear = !fn_7101173964(self->field_0x228);
            else
                clear = fn_7100d7bea0(reinterpret_cast<UnkActor*>(self), true);
            if (clear) {
                if (self->flag_0x310)
                    self->flag_0x310 = false;
                fn_7100232c90(self, dt, flag);
                goto done;
            }
        }
        if (self->flag_0x310)
            goto done;
    }
    fn_7100232c90(self, dt, flag);
done:
    ++self->field_0x308;
}

// ---- Two parallel arrays, keys and string ids (count at +0x10): swap-remove of the entry a key pointer names,
// and copy assignment ----
struct UnkIdKeyMap_b0220 {
    u64* keys;
    CStrId* values;
    u32 count;
    u32 capacity;
};

// Swap-remove of the entry whose key `pos` points at; returns the slot now holding the last entry.
u64* fn_7100d86e94(UnkIdKeyMap_b0220* self, u64* pos) {
    u64 offset = ((u64)pos - (u64)self->keys) >> 3;
    u32 index = (u32)offset;
    self->values[index].Reset();
    if (self->count - 1 != (u32)offset) {
        __builtin_memcpy(&self->keys[index], &self->keys[self->count - 1], sizeof(u64));
        self->values[index].data = self->values[self->count - 1].data;
        {
            u64 empty = (u64)&g_unk_7101d47108->empty;
            __builtin_memcpy(&self->values[self->count - 1], &empty, sizeof(u64));
        }
        self->values[self->count - 1].Reset();
    }
    self->count = self->count - 1;
    return &self->keys[index];
}

void fn_7100d86d30(UnkIdKeyMap_b0220* self);
void fn_7100d8696c(UnkIdKeyMap_b0220* self, u32 capacity);

UnkIdKeyMap_b0220* fn_7100d86bc8(UnkIdKeyMap_b0220* self, UnkIdKeyMap_b0220* other) {
    if (self != other) {
        if (self->values) {
            for (u64 i = 0; i < self->count; ++i)
                self->values[i].Reset();
        }
        self->count = 0;
        if (other->count != self->capacity) {
            fn_7100d86d30(self);
            fn_7100d8696c(self, other->count);
        }
        __builtin_memcpy(self->keys, other->keys, (u64)other->count * sizeof(u64));
        self->count = other->count;
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i) {
            base::global::UnkStringData* data = other->values[i].data;
            __builtin_memcpy(&self->values[i], &data, sizeof(data));
            if (&g_unk_7101d47108->empty != data)
                ++data->ref_count;
        }
    }
    return self;
}

// ---- The cached (index, callback set) value of an iterator over an array of callback sets: empty (0), owned
// (1) or referring to the iterator's (2). Move assignment ----
struct UnkSetElem_b0220 {
    void** vtable;
    UnkErasedValue_0x10* list_data;  // 0x8, an UnkErasedVector written as flat members
    u32 list_count;
    u32 list_capacity;
    u32 list_step;
    bool list_grow;
    u32 list_flags;
    base::UnkAllocator* list_allocator;
    u32 field_0x30;
    base::UnkRefHandle owner;  // 0x38

    UnkErasedVector* List() { return reinterpret_cast<UnkErasedVector*>(&list_data); }

    // The assignment moves from `other`: its list is emptied and its owner released.
    __attribute__((always_inline)) void MoveAssign(UnkSetElem_b0220& other) {
        base::UnkRefHandle* src = &other.owner;
        fn_710031d208(&owner, src);
        UnkErasedVector* list = other.List();
        field_0x30 = other.field_0x30;
        fn_710023b29c(List(), list);
        other.field_0x30 = 0;
        fn_7100ad3588(list);
        if (base::UnkRefCounted* p = other.owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
            other.owner.ptr = nullptr;
        }
    }
};
static_assert(sizeof(UnkSetElem_b0220) == 0x40, "layout");

struct UnkIterValue_b0220 {
    struct Value {
        u32 index;
        UnkSetElem_b0220 element;
    };
    struct Ref {
        u32* index;
        UnkSetElem_b0220* element;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;

    u32* Index() {
        return state == 2 ? reinterpret_cast<Ref*>(storage)->index : &reinterpret_cast<Value*>(storage)->index;
    }
    UnkSetElem_b0220* Element() {
        return state == 2 ? reinterpret_cast<Ref*>(storage)->element : &reinterpret_cast<Value*>(storage)->element;
    }
};

UnkIterValue_b0220* fn_7100d878b0(UnkIterValue_b0220* self, UnkIterValue_b0220* other) {
    if (other->state == 1) {
        UnkIterValue_b0220::Value* from = reinterpret_cast<UnkIterValue_b0220::Value*>(other->storage);
        *self->Index() = from->index;
        self->Element()->MoveAssign(from->element);
    } else if (other->state == 2) {
        UnkIterValue_b0220::Ref* ref = reinterpret_cast<UnkIterValue_b0220::Ref*>(other->storage);
        *self->Index() = *ref->index;
        UnkSetElem_b0220* element = ref->element;
        self->Element()->MoveAssign(*element);
    }
    return self;
}

// ---- Handle sources behind a lazily resolved handle (state 1 becomes 2): equality of the iterator values
// of two arrays of them, and the copy assignment of one ----
// As src/game/CAccessPointCommanderComponent/CAccessPointCommanderComponent_Handle_ovb0085.cpp declares them.
struct UnkCountedA5_b76 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

struct UnkRef {
    UnkCountedA5_b76* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkHandleSource_b0220 {
    void* vtable;
    u8 items[0x28];             // 0x8
    u32 state;                  // 0x30
    base::UnkRefHandle handle;  // 0x38

    // The handle, resolved from the source once (state 1 becomes 2).
    __attribute__((always_inline)) void Resolve() {
        if (state == 1) {
            {
                UnkRef fresh = fn_7100353be8(&items);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
    }
    __attribute__((always_inline)) void* Object() {
        Resolve();
        auto* c = static_cast<UnkCountedA5_b76*>(handle.ptr);
        return c ? c->field_0x10 : nullptr;
    }
};
static_assert(sizeof(UnkHandleSource_b0220) == 0x40, "layout");

struct UnkSourceIterValue_b0220 {
    struct Value {
        u32 index;
        UnkHandleSource_b0220 source;
    };
    struct Ref {
        u32* index;
        UnkHandleSource_b0220* source;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;

    u32* Index() {
        return state == 2 ? reinterpret_cast<Ref*>(storage)->index : &reinterpret_cast<Value*>(storage)->index;
    }
    UnkHandleSource_b0220* Source() {
        return state == 2 ? reinterpret_cast<Ref*>(storage)->source : &reinterpret_cast<Value*>(storage)->source;
    }
};
static_assert(sizeof(UnkSourceIterValue_b0220) == 0x50, "layout");

static inline __attribute__((always_inline)) bool Equal_b0220(UnkSourceIterValue_b0220* a,
                                                              UnkSourceIterValue_b0220* b) {
    if (*a->Index() != *b->Index())
        return false;
    UnkHandleSource_b0220* sa = a->Source();
    UnkHandleSource_b0220* sb = b->Source();
    void* ob = sb->Object();
    void* oa = sa->Object();
    return oa == ob;
}

bool fn_7100d87700(UnkSourceIterValue_b0220* a, UnkSourceIterValue_b0220* b) { return Equal_b0220(a, b); }

bool fn_7100d87be0(UnkSourceIterValue_b0220* a, UnkSourceIterValue_b0220* b) { return Equal_b0220(a, b); }

// ---- A message to the entity's sub-object at +0x220, sent when the part at +0x1f0 is notified with a set flag ----
struct UnkIdHolder_b0220 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkIdTable_b0220 {
    UnkIdTable_b0220();  // fn_7100031894
    ~UnkIdTable_b0220();
    u8 pad_0x0[0x1d78];
    UnkIdHolder_b0220* field_0x1d78;
};

inline UnkIdTable_b0220& IdTable_b0220() {
    static UnkIdTable_b0220 s_table;
    return s_table;
}

struct UnkEntityPart_b0220 {
    void* vtable;
    u8 pad_0x8[0xf9 - 0x8];
    bool field_0xf9;

    void Notify(bool flag) {
        (*reinterpret_cast<void (**)(UnkEntityPart_b0220*, bool)>(reinterpret_cast<char*>(vtable) + 0x1e8))(this,
                                                                                                         flag);
    }
};

struct UnkEntitySend_b0220 {
    u8 pad_0x0[0x1f0];
    UnkEntityPart_b0220* part;
    u8 pad_0x1f8[0x220 - 0x1f8];
    void* field_0x220;
};

void fn_7100d7babc(UnkEntitySend_b0220* self, bool flag) {
    UnkEntityPart_b0220* part = self->part;
    if (!part)
        return;
    part->Notify(flag);
    if (flag && !part->field_0xf9 && self->field_0x220)
        UnkSpitSend(self->field_0x220, IdTable_b0220().field_0x1d78->id, 0xf4061ff25a2163d6);
}
