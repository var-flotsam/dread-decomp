// Unit 2609 of tools/tu_map.py (base::reflection names): arrays and pairs of owned id sets
// (src/game_s7/fn_7100ef1e1c.cpp destroys one, fn_7100083c00 frees it): clearing an array, and
// assigning a pair that either holds its key and set or refers to them.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkNamedIds;  // src/game/CSubAreaManager.cpp
void fn_7100ef1e1c(UnkNamedIds* self);

struct UnkOwnedIds_ovb0066 {
    union {  // no type-based alias information, as in the original
        UnkNamedIds* ptr;
    };

    UnkOwnedIds_ovb0066(UnkOwnedIds_ovb0066&& other) { ptr = other.Release(); }
    UnkOwnedIds_ovb0066& operator=(UnkOwnedIds_ovb0066&& other) {
        UnkNamedIds* p = other.ptr;
        other.ptr = nullptr;
        Reset(p);
        return *this;
    }
    ~UnkOwnedIds_ovb0066() { Reset(nullptr); }

    UnkNamedIds* Release() {
        UnkNamedIds* p = ptr;
        ptr = nullptr;
        return p;
    }
    void Reset(UnkNamedIds* value) {
        UnkNamedIds* p = ptr;
        ptr = value;
        if (p) {
            fn_7100ef1e1c(p);
            operator delete(p);
        }
    }
};

struct UnkOwnedIdsArray_ovb0066 {
    UnkOwnedIds_ovb0066* data;
    union {  // reloaded after the elements move
        u32 count;
    };
    u32 capacity;
    u8 unk_0x10[0x10];
    base::UnkAllocator* allocator;  // 0x20
};

void fn_7100fa0b50(UnkOwnedIdsArray_ovb0066* self) {
    if (!self->data)
        return;
    for (u64 i = 0; i < self->count; i++)
        self->data[i].~UnkOwnedIds_ovb0066();
    self->count = 0;
    self->allocator->vf_0x48(self->data);
    self->capacity = 0;
}

struct UnkIdsPair_ovb0066 {
    union {
        struct {
            u32 key;
            UnkOwnedIds_ovb0066 ids;
        } value;
        struct {
            u32* key;
            UnkOwnedIds_ovb0066* ids;
        } ref;
    };
    s32 state;  // 1: holds the value, 2: refers to it

    u32& Key() { return state == 2 ? *ref.key : value.key; }
    UnkOwnedIds_ovb0066& Ids() { return state == 2 ? *ref.ids : value.ids; }
};

UnkIdsPair_ovb0066* fn_7100fa032c(UnkIdsPair_ovb0066* self, UnkIdsPair_ovb0066* other) {
    switch (other->state) {
    case 1:
        self->Key() = other->value.key;
        self->Ids().Reset(other->value.ids.Release());
        break;
    case 2:
        self->Key() = *other->ref.key;
        UnkOwnedIds_ovb0066* ids = other->ref.ids;
        self->Ids().Reset(ids->Release());
        break;
    }
    return self;
}
