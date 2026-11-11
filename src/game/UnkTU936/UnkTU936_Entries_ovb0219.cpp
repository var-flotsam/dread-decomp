// Unit 936 (tools/tu_map.py): a base::UnkVector of 0x20-byte polymorphic entries (a vtable, a string id
// and two words): the entries' copy, destruction through vtable slot 1, and the vector's copy, assignment,
// PushBack and Insert; three more copies of the owning-pointer vector erase by index
// (src/game/querysystem/UnkOwnedPtrVectorErase_b41.cpp); and a constructor (fn_7100416db8's derived class
// with two empty arrays and the post-construction virtual call). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include "game/UnkTU936.h"
#include "game_s2/UnkOwnedList.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkEntry_ovb0219 {
    void (**vtable)(UnkEntry_ovb0219*);
    base::global::UnkStringData* id;  // released by the destructor behind vtable slot 1
    u64 words[2];

    __attribute__((always_inline)) UnkEntry_ovb0219(const UnkEntry_ovb0219& other) {
        vtable = reinterpret_cast<void (**)(UnkEntry_ovb0219*)>(g_unk_7101a56178 + 0x10);
        id = other.id;
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
        __builtin_memcpy(words, other.words, 16);
    }
    __attribute__((always_inline)) ~UnkEntry_ovb0219() { vtable[1](this); }
};

using Entry = UnkEntry_ovb0219;

__attribute__((always_inline)) static inline Entry* MoveTo(base::UnkVector<Entry>* self, u32 capacity) {
    Entry* p = static_cast<Entry*>(self->allocator->vf_0x40((u64)capacity * sizeof(Entry), 8, self->flags));
    if (self->data) {
        Entry* src = self->data;
        Entry* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) Entry(*src);
            src->~Entry();
        }
        self->allocator->vf_0x48(self->data);
    }
    return p;
}

__attribute__((noinline)) void fn_710061c9ec(base::UnkVector<Entry>* self, const Entry* value);

void fn_710061c9ec(base::UnkVector<Entry>* self, const Entry* value) {
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

// The copy constructor: the other's settings, its capacity reserved and each entry pushed.
void fn_710061c898(base::UnkVector<Entry>* self, const base::UnkVector<Entry>* other) {
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
        fn_710061c9ec(self, p);
}

// Copy assignment: the entries are destroyed, the other's capacity reserved and each entry pushed.
base::UnkVector<Entry>* fn_710061cb84(base::UnkVector<Entry>* self, const base::UnkVector<Entry>* other) {
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
            fn_710061c9ec(self, p);
    }
    return self;
}

void fn_710061cd0c(base::UnkVector<Entry>* self, const Entry* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_710061c9ec(self, value);
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


namespace {

// Moves the pointer in `src` into the empty slot `dst`, leaving `src` empty.
inline void MoveInto_ovb0219(UnkOwnedPtr* dst, UnkOwnedPtr* src) {
    UnkOwned* t = src->ptr;
    src->ptr = nullptr;
    dst->ptr = t;
}

}  // namespace

struct UnkOwnedPtrVector_ovb0219 : base::UnkVector<UnkOwnedPtr> {};

static inline __attribute__((always_inline)) void Erase_ovb0219(UnkOwnedPtrVector_ovb0219* self, const u32* index) {
    u32 at = *index;
    if (at < self->count) {
        u64 i = at;
        self->data[i].~UnkOwnedPtr();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwnedPtr* cur = &self->data[i];
            MoveInto_ovb0219(cur, &cur[1]);
            (&self->data[i])[1].~UnkOwnedPtr();
        }
        self->count = self->count - 1;
    }
}

#define UNK_ERASE_OVB0219(addr) \
    void fn_##addr(UnkOwnedPtrVector_ovb0219* self, const u32* index) { Erase_ovb0219(self, index); }

UNK_ERASE_OVB0219(710061a0cc)
UNK_ERASE_OVB0219(710061b210)
UNK_ERASE_OVB0219(710061de3c)

struct UnkObject_7100416db8;
void fn_710060a504(void* self);
void fn_7100624194(void* self);

struct UnkComponent_ovb0219 {
    void** vtable;
    u8 pad_0x8[0x508];
    u8 field_0x510[0x128];
    u8 field_0x638[0x138];
    void** data_0x770;
    u32 count_0x778;
    u32 capacity_0x77c;
    u32 step_0x780;
    bool field_0x784;
    u32 flags_0x788;
    base::UnkAllocator* allocator_0x790;
    void** data_0x798;
    u32 count_0x7a0;
    u32 capacity_0x7a4;
    u32 step_0x7a8;
    bool field_0x7ac;
    u32 flags_0x7b0;
    base::UnkAllocator* allocator_0x7b8;
};
static_assert(__builtin_offsetof(UnkComponent_ovb0219, data_0x770) == 0x770, "layout");
static_assert(__builtin_offsetof(UnkComponent_ovb0219, data_0x798) == 0x798, "layout");

void fn_710061e89c(UnkComponent_ovb0219* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a561c8 + 2;
    fn_710060a504(self->field_0x510);
    fn_7100624194(self->field_0x638);
    self->data_0x770 = nullptr;
    self->count_0x778 = 0;
    self->capacity_0x77c = 0;
    self->step_0x780 = 10;
    self->field_0x784 = true;
    self->flags_0x788 = 0xf;
    self->allocator_0x790 = fn_7100083990();
    self->data_0x798 = nullptr;
    self->count_0x7a0 = 0;
    self->capacity_0x7a4 = 0;
    self->step_0x7a8 = 10;
    self->field_0x7ac = true;
    self->flags_0x7b0 = 0xf;
    self->allocator_0x7b8 = fn_7100083990();
    reinterpret_cast<void (*)(UnkComponent_ovb0219*)>(self->vtable[3])(self);
}
