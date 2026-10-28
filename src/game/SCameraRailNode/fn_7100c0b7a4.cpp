// Copy-assignments of growable arrays (base::UnkVector's layout) of plain elements copied one
// by one: the array is emptied, reserves the other's capacity and appends each element,
// growing like PushBack (including its hang).
#include <new>

#include "base/UnkVector.h"
#include "types.h"

template <class T>
struct UnkCopiedVector {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reallocate(u32 new_capacity) {
        T* p = static_cast<T*>(allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, flags));
        if (data) {
            T* src = data;
            T* dst = p;
            for (u32 i = 0; i < count; ++i, ++src, ++dst)
                new (dst) T(*src);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }

    __attribute__((always_inline)) UnkCopiedVector& operator=(const UnkCopiedVector& other) {
        if (this != &other) {
            count = 0;
            u32 reserve = other.capacity;
            if (capacity < reserve)
                Reallocate(reserve);
            for (const T *it = other.data, *end = it + other.count; it != end; ++it) {
                u32 n = count;
                u32 cap = capacity;
                u32 new_capacity = cap;
                if (cap <= n && step != 0)
                    new_capacity = (n / step + 1) * step;
                if ((s32)n >= (s32)new_capacity)
                    for (;;) {
                    }
                if (cap < new_capacity)
                    Reallocate(new_capacity);
                new (&data[count]) T(*it);
                ++count;
            }
        }
        return *this;
    }
};

// Two values copied one by one (as plain words: the original reloads the count after each
// element).
struct UnkValuePair {
    u32 values[2];

    UnkValuePair(const UnkValuePair& other) {
        __builtin_memcpy(&values[0], &other.values[0], sizeof(u32));
        __builtin_memcpy(&values[1], &other.values[1], sizeof(u32));
    }
};

UnkCopiedVector<UnkValuePair>* fn_7100c0b7a4(UnkCopiedVector<UnkValuePair>* self, const UnkCopiedVector<UnkValuePair>* other) {
    return &(*self = *other);
}

// A plain word, and two words and a value, copied piece by piece.
struct UnkWord {
    u64 value;

    UnkWord(const UnkWord& other) { __builtin_memcpy(&value, &other.value, sizeof(u64)); }
};

struct UnkWordsValue {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;

    UnkWordsValue(const UnkWordsValue& other) {
        __builtin_memcpy(&field_0x0, &other.field_0x0, sizeof(u64));
        __builtin_memcpy(&field_0x8, &other.field_0x8, sizeof(u64));
        __builtin_memcpy(&field_0x10, &other.field_0x10, sizeof(u32));
    }
};

UnkCopiedVector<UnkWord>* fn_7100b177a4(UnkCopiedVector<UnkWord>* self, const UnkCopiedVector<UnkWord>* other) {
    return &(*self = *other);
}
UnkCopiedVector<UnkWord>* fn_7100b18f80(UnkCopiedVector<UnkWord>* self, const UnkCopiedVector<UnkWord>* other) {
    return &(*self = *other);
}
UnkCopiedVector<UnkWordsValue>* fn_7100e05764(UnkCopiedVector<UnkWordsValue>* self,
                                              const UnkCopiedVector<UnkWordsValue>* other) {
    return &(*self = *other);
}

// Copy-assignment of a record of two arrays and four values: the first array (of three
// values per element) appends through its out-of-line PushBack; the second (of half-words)
// is copied as one block.
struct UnkValues3 {
    u32 values[3];

    UnkValues3(const UnkValues3& other) {
        __builtin_memcpy(&values[0], &other.values[0], sizeof(u32));
        __builtin_memcpy(&values[1], &other.values[1], sizeof(u32));
        __builtin_memcpy(&values[2], &other.values[2], sizeof(u32));
    }
};

struct UnkPushedVector : UnkCopiedVector<UnkValues3> {
    UnkPushedVector& operator=(const UnkPushedVector& other);
};
void fn_710058c768(UnkPushedVector* self, const UnkValues3* value);  // PushBack

__attribute__((always_inline)) inline UnkPushedVector& UnkPushedVector::operator=(const UnkPushedVector& other) {
    if (this != &other) {
        count = 0;
        u32 reserve = other.capacity;
        if (capacity < reserve)
            Reallocate(reserve);
        for (const UnkValues3 *it = other.data, *end = it + other.count; it != end; ++it)
            fn_710058c768(this, it);
    }
    return *this;
}

struct UnkBlockVector : UnkCopiedVector<u16> {
    __attribute__((always_inline)) UnkBlockVector& operator=(const UnkBlockVector& other) {
        if (this != &other) {
            count = 0;
            u32 reserve = other.capacity;
            if (capacity < reserve) {
                u16* p = static_cast<u16*>(allocator->vf_0x40((u64)reserve * sizeof(u16), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(u16));
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = reserve;
            }
            __builtin_memcpy(data, other.data, (u64)other.count * sizeof(u16));
            count = other.count;
        }
        return *this;
    }
};

struct UnkArraysRecord {
    UnkPushedVector first;
    UnkBlockVector second;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
};

UnkArraysRecord* fn_7100e4f444(UnkArraysRecord* self, const UnkArraysRecord* other) {
    self->first = other->first;
    self->second = other->second;
    self->field_0x50 = other->field_0x50;
    self->field_0x54 = other->field_0x54;
    self->field_0x58 = other->field_0x58;
    self->field_0x5c = other->field_0x5c;
    return self;
}

// Copy-assignment of an array of polymorphic records (destroyed through vtable slot 1): the
// elements are destroyed, the array reserves the other's capacity (moving each record over and
// destroying the old one) and each of the other's records is appended by fn_7100e4ea08.
extern char g_unk_7101cac8f0[];

struct UnkPolyValues {
    char* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;

    void Destroy() { reinterpret_cast<void (**)(UnkPolyValues*)>(vtable)[1](this); }
};

struct UnkPolyVector : UnkCopiedVector<UnkPolyValues> {};
void fn_7100e4ea08(UnkPolyVector* self, const UnkPolyValues* value);  // PushBack

UnkPolyVector* fn_7100e4e898(UnkPolyVector* self, const UnkPolyVector* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].Destroy();
        self->count = 0;
        u32 reserve = other->capacity;
        if (self->capacity < reserve) {
            UnkPolyValues* p = static_cast<UnkPolyValues*>(
                self->allocator->vf_0x40((u64)reserve * sizeof(UnkPolyValues), 8, self->flags));
            if (UnkPolyValues* src = self->data) {
                UnkPolyValues* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                    dst->vtable = g_unk_7101cac8f0 + 0x10;
                    dst->field_0x8 = src->field_0x8;
                    dst->field_0xc = src->field_0xc;
                    dst->field_0x10 = src->field_0x10;
                    dst->field_0x14 = src->field_0x14;
                    dst->field_0x18 = src->field_0x18;
                    src->Destroy();
                }
                self->allocator->vf_0x48(self->data);
            }
            self->data = p;
            self->capacity = reserve;
        }
        for (const UnkPolyValues *it = other->data, *end = it + other->count; it != end; ++it)
            fn_7100e4ea08(self, it);
    }
    return self;
}

// Copy-assignment of a singly linked list of words: the nodes are freed, then a node is
// allocated and appended for each of the other's values.
// The link is a union member (no type-based alias info: the count is reloaded after it is set).
struct UnkWordNode {
    u64 value;
    union {
        UnkWordNode* next;
    };
};

struct UnkWordList {
    u64 unk_0x0;
    u32 count;
    UnkWordNode* head;
    UnkWordNode* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    void PushBack(const u64& value) {
        auto* node = static_cast<UnkWordNode*>(allocator->vf_0x40(sizeof(UnkWordNode), 8, flags));
        if (node) {
            node->value = value;
            node->next = nullptr;
        }
        if (count == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->next = nullptr;
        }
        tail = node;
        ++count;
    }

    UnkWordList& operator=(const UnkWordList& other) {
        if (this != &other) {
            for (UnkWordNode* node = head; node;) {
                UnkWordNode* next = node->next;
                allocator->vf_0x48(node);
                node = next;
            }
            count = 0;
            head = nullptr;
            tail = nullptr;
            for (UnkWordNode* node = other.head; node; node = node->next)
                PushBack(node->value);
        }
        return *this;
    }
};

// NON_MATCHING(minor): the shared end of the append (setting the tail and the count) is duplicated
// into the empty-list case; the original branches to it.
UnkWordList* fn_7100d833f8(UnkWordList* self, const UnkWordList* other) { return &(*self = *other); }
// NON_MATCHING(minor): as fn_7100d833f8.
UnkWordList* fn_7100d834c8(UnkWordList* self, const UnkWordList* other) { return &(*self = *other); }
