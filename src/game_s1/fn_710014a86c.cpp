// Destructors (complete and deleting) of a class (vtable unk_7101a1cda0) with a member at 0x38
// (destroyed by fn_7100014e88) above a base (vtable unk_7101a1cd10) holding an array at 0x10
// (data, count at 0x18, capacity at 0x1c, allocator at 0x30): fn_710014a86c and fn_710014a8cc.
// The deleting one frees the array without resetting its capacity. fn_71006fc63c is the same for
// a larger class.

#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern char unk_7101a1cda0[];
extern char unk_7101a1cd10[];
void fn_7100014e88(void* member);

// The base: a vtable, a word and the array.
struct UnkArrayBase_710014a86c {
    char* vtable;
    u64 field_0x8;
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x20[0x10];
    base::UnkAllocator* allocator;
};

template <bool Deleting>
__attribute__((always_inline)) static inline void DestroyBase(UnkArrayBase_710014a86c* self) {
    self->vtable = unk_7101a1cd10 + 0x10;
    if (self->data) {
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        if (!Deleting)
            self->capacity = 0;
    }
}

struct UnkArrayHolder_710014a86c : UnkArrayBase_710014a86c {
    u8 member[0x8];
};

template <bool Deleting>
__attribute__((always_inline)) static inline void Destroy(UnkArrayHolder_710014a86c* self) {
    self->vtable = unk_7101a1cda0 + 0x10;
    fn_7100014e88(self->member);
    DestroyBase<Deleting>(self);
}

void fn_710014a86c(UnkArrayHolder_710014a86c* self) { Destroy<false>(self); }

void fn_710014a8cc(UnkArrayHolder_710014a86c* self) {
    Destroy<true>(self);
    operator delete(self);
}

// Constructor of another class above that base (vtable unk_7101a1ce30), the array allocated
// with flags 10 and a cleared word at 0x38 (fn_710014abcc).
extern char unk_7101a1ce30[];

struct UnkArrayHolder_710014abcc {
    char* vtable;
    u64 field_0x8;
    base::UnkVector<void*> array;
    u32 field_0x38;
};

void fn_710014abcc(UnkArrayHolder_710014abcc* self) {
    self->vtable = unk_7101a1cd10 + 0x10;
    self->field_0x8 = 0;
    new (&self->array) base::UnkVector<void*>(10);
    self->field_0x38 = 0;
    self->vtable = unk_7101a1ce30 + 0x10;
}

// The same for a class (vtable unk_7101a1cca8) with two such members, at 0x40 and 0x78:
// fn_7100149cc0, fn_7100149d28.
extern char unk_7101a1cca8[];

struct UnkTwoMembers_7100149cc0 : UnkArrayBase_710014a86c {
    u8 member_0x38[0x8];
    u8 member_0x40[0x38];
    u8 member_0x78[0x8];
};

template <bool Deleting>
__attribute__((always_inline)) static inline void Destroy(UnkTwoMembers_7100149cc0* self) {
    self->vtable = unk_7101a1cca8 + 0x10;
    fn_7100014e88(self->member_0x78);
    fn_7100014e88(self->member_0x40);
    DestroyBase<Deleting>(self);
}

void fn_7100149cc0(UnkTwoMembers_7100149cc0* self) { Destroy<false>(self); }

void fn_7100149d28(UnkTwoMembers_7100149cc0* self) {
    Destroy<true>(self);
    operator delete(self);
}

// And for a class (vtable unk_7101a1d258) owning an array of objects at 0x38, each deleted through
// its vtable: fn_710014c3d4, fn_710014c4a4.
extern char unk_7101a1d258[];

struct UnkOwnedObject {
    virtual ~UnkOwnedObject();
};

struct UnkOwningArray_710014c3d4 : UnkArrayBase_710014a86c {
    UnkOwnedObject** objects;
    u32 object_count;
    u32 object_capacity;
    u8 pad_0x48[0x10];
    base::UnkAllocator* object_allocator;
};

template <bool Deleting>
__attribute__((always_inline)) static inline void Destroy(UnkOwningArray_710014c3d4* self) {
    self->vtable = unk_7101a1d258 + 0x10;
    UnkOwnedObject** p = self->objects;
    for (UnkOwnedObject** end = p + self->object_count; p != end; ++p) {
        if (*p) {
            delete *p;
            *p = nullptr;
        }
    }
    self->object_count = 0;
    if (self->objects) {
        self->object_count = 0;
        self->object_allocator->vf_0x48(self->objects);
        self->object_capacity = 0;
    }
    DestroyBase<Deleting>(self);
}

void fn_710014c3d4(UnkOwningArray_710014c3d4* self) { Destroy<false>(self); }

void fn_710014c4a4(UnkOwningArray_710014c3d4* self) {
    Destroy<true>(self);
    operator delete(self);
}

// The complete destructor of a class (vtables unk_7101a1d150 + 0x10 and + 0x60) with that one as
// its second base at 8: an array at 0x80, a hashed list at 0xa8 (nodes linked at 0x10 from 0xe8,
// buckets in an array at 0xb8) and an array at 0xf0 (fn_710014c1e8).
extern char unk_7101a1d150[];

struct UnkArray_710014c1e8 {
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkArray_710014c1e8() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkListNode_710014c1e8 {
    u8 unk_0x0[0x10];
    UnkListNode_710014c1e8* next;
};

struct UnkHashedList_710014c1e8 {
    base::UnkAllocator* allocator;
    u32 field_0x8;
    u32 size;
    UnkArray_710014c1e8 buckets;
    UnkListNode_710014c1e8* tail;
    UnkListNode_710014c1e8* head;

    __attribute__((always_inline)) ~UnkHashedList_710014c1e8() {
        if (size) {
            for (UnkListNode_710014c1e8* n = head; n;) {
                UnkListNode_710014c1e8* next = n->next;
                allocator->vf_0x48(n);
                n = next;
            }
            __builtin_memset(buckets.data, 0, buckets.capacity * 8ul);
            size = 0;
            tail = nullptr;
            head = nullptr;
        }
    }
};

struct UnkTwoBases_710014c1e8 {
    char* vtable;
    UnkOwningArray_710014c3d4 base;
    u8 unk_0x68[0x18];
    UnkArray_710014c1e8 array_0x80;
    UnkHashedList_710014c1e8 list;
    UnkArray_710014c1e8 array_0xf0;
};
static_assert(offsetof(UnkTwoBases_710014c1e8, array_0x80) == 0x80, "layout");
static_assert(offsetof(UnkTwoBases_710014c1e8, array_0xf0) == 0xf0, "layout");

void fn_710014c1e8(UnkTwoBases_710014c1e8* self) {
    self->vtable = unk_7101a1d150 + 0x10;
    self->base.vtable = unk_7101a1d150 + 0x60;
    self->array_0xf0.~UnkArray_710014c1e8();
    self->list.~UnkHashedList_710014c1e8();
    self->array_0x80.~UnkArray_710014c1e8();
    Destroy<false>(&self->base);
}

// Its constructor: a first base (vtable unk_7101a1d208) at 0, then the second base, whose own
// base reserves one element (fn_710014b810). The hashed list starts with 11 empty buckets.
extern char unk_7101a1d208[];

struct UnkTwoBasesInit_710014b810 {
    char* vtable;
    char* base_vtable;
    u64 field_0x10;
    base::UnkVector<void*> array;
    base::UnkVector<UnkOwnedObject*> objects;
    bool field_0x68;
    u64 field_0x70;
    u64 field_0x78;
    base::UnkVector<void*> array_0x80;
    base::UnkAllocator* list_allocator;
    u32 list_field_0x8;
    u32 list_size;
    base::UnkVector<void*> buckets;
    void* list_tail;
    void* list_head;
    base::UnkVector<void*> array_0xf0;
};
static_assert(offsetof(UnkTwoBasesInit_710014b810, array_0xf0) == 0xf0, "layout");

template <class T>
__attribute__((always_inline)) static inline void ResizeZeroed(base::UnkVector<T>* v, u32 n) {
    if (v->count < n) {
        v->Reserve(n);
        __builtin_memset(v->data + v->count, 0, (u64)(n - v->count) * sizeof(T));
        v->count = n;
    } else if (v->count != n) {
        v->count = n;
    }
}

void fn_710014b810(UnkTwoBasesInit_710014b810* self) {
    self->vtable = unk_7101a1d208 + 0x10;
    self->base_vtable = unk_7101a1cd10 + 0x10;
    self->field_0x10 = 0;
    new (&self->array) base::UnkVector<void*>(10);
    self->array.Reserve(1);
    self->base_vtable = unk_7101a1d258 + 0x10;
    new (&self->objects) base::UnkVector<UnkOwnedObject*>(0x40);
    self->field_0x68 = true;
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    self->vtable = unk_7101a1d150 + 0x10;
    self->base_vtable = unk_7101a1d150 + 0x60;
    new (&self->array_0x80) base::UnkVector<void*>(0x40);
    self->list_allocator = fn_7100083990();
    self->list_field_0x8 = 0xe;
    self->list_size = 0;
    new (&self->buckets) base::UnkVector<void*>(0xe);
    self->list_tail = nullptr;
    self->list_head = nullptr;
    ResizeZeroed(&self->buckets, 11);
    new (&self->array_0xf0) base::UnkVector<void*>(0xf);
}
