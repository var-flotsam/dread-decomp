// CEmmyAIComponent's unit (tools/tu_map.py unit 811): copy constructors of two definition classes holding
// callback lists (the lists src/game/CEmmyOverrideDeathPositionDef/CEmmyOverrideDeathPositionDef_Dtor_A3_b69.cpp
// destroys). Each list is built empty (base vtable 0x7101a20998, an owning array with flags 0xf), then given
// the list vtable 0x7101a20950, the other's owner reference, its u32 and a copy of its callables
// (fn_710023b29c). Names are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU341.h"
#include "types.h"

#include <new>

extern void* g_unk_7101a48248[];
extern void* g_unk_7101a48270[];
extern void* g_unk_7101a48298[];

void fn_7100246a94(void*, const void*);  // copy of a 0x28-byte array
using UnkDefObjects_A3_b69 = base::UnkVector<UnkObject_0x100>;
void fn_7100563d8c(UnkDefObjects_A3_b69* self, const UnkDefObjects_A3_b69* other);


struct UnkDefListVt_A3_b69 {
    char* vtable;
    UnkDefListVt_A3_b69() { vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10; }
};

struct UnkDefListBase_A3_b69 : UnkDefListVt_A3_b69 {
    UnkOwnedErasedVector callables;
    u32 field_0x30 = 0;
};

struct UnkDefList_A3_b69 : UnkDefListBase_A3_b69 {
    base::UnkRefHandle owner;

    UnkDefList_A3_b69(const UnkDefList_A3_b69& other) {
        vtable = reinterpret_cast<char*>(g_unk_7101a20950) + 0x10;
        owner.ptr = nullptr;
        owner.Set(other.owner.ptr);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callables, &other.callables);
    }
};
static_assert(sizeof(UnkDefList_A3_b69) == 0x40, "UnkDefList_A3_b69 layout");

// The class with vtable 0x7101a48248 (CEmmyOverrideDeathPositionDef's destructor fn_7100531ffc).
struct UnkDefAVt_A3_b69 {
    char* vtable;
    UnkDefAVt_A3_b69() { vtable = reinterpret_cast<char*>(g_unk_7101a48248) + 0x10; }
};

struct UnkDefA_A3_b69 : UnkDefAVt_A3_b69 {
    UnkDefList_A3_b69 first;
    UnkDefList_A3_b69 second;
    UnkDefA_A3_b69(const UnkDefA_A3_b69& other) : UnkDefAVt_A3_b69(), first(other.first), second(other.second) {}
};
static_assert(sizeof(UnkDefA_A3_b69) == 0x88, "UnkDefA_A3_b69 layout");

__attribute__((flatten)) void fn_7100561588(UnkDefA_A3_b69* self, const UnkDefA_A3_b69* other) {
    new (self) UnkDefA_A3_b69(*other);
}

// The class with vtable 0x7101a48298: a list, a 0x28-byte array (fn_7100246a94), a list, a byte and an
// array of 0x100-byte objects at +0xb8 (fn_7100563d8c, tail-called).
struct UnkDefArray_A3_b69 {
    u8 raw[0x28];
    UnkDefArray_A3_b69(const UnkDefArray_A3_b69& other) { fn_7100246a94(this, &other); }
};

// Raw storage, so that the array's default member initialisers don't run before the copy.
struct alignas(8) UnkDefTail_A3_b69 {
    u8 raw[sizeof(UnkDefObjects_A3_b69)];
    UnkDefTail_A3_b69(const UnkDefTail_A3_b69& other) {
        fn_7100563d8c(reinterpret_cast<UnkDefObjects_A3_b69*>(raw),
                      reinterpret_cast<const UnkDefObjects_A3_b69*>(other.raw));
    }
};

struct UnkDefBVt_A3_b69 {
    char* vtable;
    UnkDefBVt_A3_b69() { vtable = reinterpret_cast<char*>(g_unk_7101a48298) + 0x10; }
};

struct UnkDefB_A3_b69 : UnkDefBVt_A3_b69 {
    UnkDefList_A3_b69 first;
    UnkDefArray_A3_b69 array;
    UnkDefList_A3_b69 second;
    bool flag;
    UnkDefTail_A3_b69 tail;
    UnkDefB_A3_b69(const UnkDefB_A3_b69& other)
        : UnkDefBVt_A3_b69(), first(other.first), array(other.array), second(other.second), flag(other.flag),
          tail(other.tail) {}
};
static_assert(__builtin_offsetof(UnkDefB_A3_b69, tail) == 0xb8, "UnkDefB_A3_b69 layout");

__attribute__((flatten)) void fn_7100563c14(UnkDefB_A3_b69* self, const UnkDefB_A3_b69* other) {
    new (self) UnkDefB_A3_b69(*other);
}

// The copy constructor of an array of 0x100-byte objects (copied by fn_71002b58c0, destroyed by
// fn_71002b6094): the other's step, flags and allocator, its capacity reserved, then each object appended.
__attribute__((noinline)) void fn_7100563d8c(UnkDefObjects_A3_b69* self, const UnkDefObjects_A3_b69* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkObject_0x100 *it = other->data, *end = other->data + other->count; it != end; ++it)
        self->PushBackCopy(*it);
}

// The class with vtable 0x7101a48270 (fn_7100562854): two lists, a 0x28-byte array (fn_7100246a94), an
// array of string ids (reserve out of line) and a byte.
struct UnkDefCVt_A3_b69 {
    char* vtable;
    UnkDefCVt_A3_b69() { vtable = reinterpret_cast<char*>(g_unk_7101a48270) + 0x10; }
};

struct UnkDefC_A3_b69 : UnkDefCVt_A3_b69 {
    UnkDefList_A3_b69 first;
    UnkDefList_A3_b69 second;
    UnkDefArray_A3_b69 array;
    base::UnkStrIdVec ids;
    bool flag;
    UnkDefC_A3_b69(const UnkDefC_A3_b69& other)
        : UnkDefCVt_A3_b69(), first(other.first), second(other.second), array(other.array), ids(other.ids),
          flag(other.flag) {}
};
static_assert(__builtin_offsetof(UnkDefC_A3_b69, flag) == 0xd8, "UnkDefC_A3_b69 layout");

__attribute__((flatten)) void fn_7100562854(UnkDefC_A3_b69* self, const UnkDefC_A3_b69* other) {
    new (self) UnkDefC_A3_b69(*other);
}
