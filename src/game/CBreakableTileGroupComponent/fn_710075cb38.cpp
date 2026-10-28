// Copies of fn_71009c21d4, fn_71009c2110, fn_71009c49d8, fn_71009c4920 (src/game/CNavMeshItemComponent/fn_71009c2f58.cpp), which the binary
// repeats at other addresses: fn_710075cb38, fn_710075cf90, fn_710075ca74, fn_710075cecc, fn_7100760ff4, fn_7100760f3c.
// Move-constructs a value-or-reference holder of a string id and an array: kind 1 holds them in
// place, kind 2 points at them; the new holder always holds them in place (kind 1).
// fn_7100a74794 is an identical copy.
#include <new>

#include "base/global/CStrId.h"
#include "base/UnkVector.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkMovedId {
    UnkStringData* data;

    UnkMovedId(UnkMovedId&& other) {
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkMovedId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkArray(UnkArray&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkArray() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkPair {
    UnkMovedId id;
    UnkArray array;
};

struct UnkPairRef {
    UnkMovedId* id;
    UnkArray* array;
};

// Untyped storage for either form.
struct UnkHolder {
    alignas(8) u8 storage[0x30];
    u32 kind;

    UnkPair& Value() { return *reinterpret_cast<UnkPair*>(storage); }
    UnkPairRef& Ref() { return *reinterpret_cast<UnkPairRef*>(storage); }

    UnkMovedId& Id() { return kind == 2 ? *Ref().id : Value().id; }
    UnkArray& Array() { return kind == 2 ? *Ref().array : Value().array; }

    void Reset() {
        if (kind == 1) {
            Value().~UnkPair();
            kind = 0;
        } else if (kind == 2) {
            kind = 0;
        }
    }

    UnkHolder& operator=(const UnkPairRef& ref) {
        if (static_cast<const void*>(this) != &ref) {
            UnkMovedId* id = ref.id;
            UnkArray* array = ref.array;
            Reset();
            kind = 2;
            Ref().id = id;
            Ref().array = array;
        }
        return *this;
    }

    // Assignment from a holder that refers to its id and array (kind 2) or is empty.
    void AssignRef(const UnkHolder& other) {
        if (this != &other) {
            Reset();
            kind = other.kind;
            Ref() = const_cast<UnkHolder&>(other).Ref();
        }
    }
};

static inline __attribute__((always_inline)) void MoveHolder(UnkHolder* self, UnkHolder* other) {
    self->kind = 0;
    if (other->kind == 1) {
        self->kind = 1;
        new (&self->Value().id) UnkMovedId(static_cast<UnkMovedId&&>(other->Value().id));
        new (&self->Value().array) UnkArray(static_cast<UnkArray&&>(other->Value().array));
    } else if (other->kind == 2) {
        self->kind = 1;
        UnkMovedId& id = *other->Ref().id;
        UnkArray& array = *other->Ref().array;
        new (&self->Value().id) UnkMovedId(static_cast<UnkMovedId&&>(id));
        new (&self->Value().array) UnkArray(static_cast<UnkArray&&>(array));
    }
}

void fn_7100a74794(UnkHolder* self, UnkHolder* other);

// Makes the holder refer to `ref`'s id and array (kind 2), destroying what it held.
// fn_71009c2568 is an identical copy.
UnkHolder* fn_710075ca74(UnkHolder* self, const UnkPairRef* ref) {
    return &(*self = *ref);
}

UnkHolder* fn_710075cecc(UnkHolder* self, const UnkPairRef* ref) {
    return &(*self = *ref);
}

UnkHolder* fn_71009c2568(UnkHolder* self, const UnkPairRef* ref);

// fn_71009c262c is an identical copy.
UnkHolder* fn_710075cb38(UnkHolder* self, const UnkHolder* other) {
    self->AssignRef(*other);
    return self;
}

UnkHolder* fn_710075cf90(UnkHolder* self, const UnkHolder* other) {
    self->AssignRef(*other);
    return self;
}

UnkHolder* fn_71009c262c(UnkHolder* self, const UnkHolder* other);

// The same reference in the other order (array first).
struct UnkPairRefRev {
    UnkArray* array;
    UnkMovedId* id;
};

// Makes `holder` refer to this id and array (kind 2), destroying what it held.
void fn_7100760f3c(const UnkPairRefRev* self, UnkHolder* holder) {
    UnkArray* array = self->array;
    UnkMovedId* id = self->id;
    holder->Reset();
    holder->kind = 2;
    holder->Ref().id = id;
    holder->Ref().array = array;
}

// An element of parallel arrays of arrays and ids.
struct UnkPairCursor {
    UnkArray* arrays;
    UnkMovedId* ids;
    u32 index;
};

// Makes `holder` refer to the cursor's id and array (kind 2), destroying what it held.
void fn_7100760ff4(const UnkPairCursor* self, UnkHolder* holder) {
    UnkMovedId* id = &self->ids[self->index];
    UnkArray* array = &self->arrays[self->index];
    holder->Reset();
    holder->kind = 2;
    holder->Ref().id = id;
    holder->Ref().array = array;
}

// Equality: the same id and the same array elements (compared as pointers). fn_71009c3590,
// fn_7100a74690 and fn_7100a74d7c are identical copies.
static inline __attribute__((always_inline)) bool Equal(UnkHolder* self, UnkHolder* other) {
    UnkMovedId& x = self->Id();
    UnkMovedId& y = other->Id();
    if (x.data != y.data)
        return false;
    UnkArray& a = self->Array();
    UnkArray& b = other->Array();
    if (a.count != b.count)
        return false;
    void** p = static_cast<void**>(a.data);
    void** q = static_cast<void**>(b.data);
    for (u64 n = a.count; n != 0; --n, ++p, ++q) {
        if (*q != *p)
            return false;
    }
    return true;
}

bool fn_71009c3590(UnkHolder* self, UnkHolder* other);

bool fn_7100a74690(UnkHolder* self, UnkHolder* other);

bool fn_7100a74d7c(UnkHolder* self, UnkHolder* other);

