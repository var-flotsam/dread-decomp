// Copies of fn_7100b1a860 (src/game_aca57c/fn_7100b1ae74.cpp) for records of a word and a 0x100-byte
// object (copied by fn_71002b58c0, destroyed by fn_71002b6094): base::UnkVector's PushBack of a
// copy, where growing copies each record into the new storage and destroys the old one:
// fn_71002a1778, fn_7100279ca0.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkObject_0x100;
void fn_71002b58c0(UnkObject_0x100* self, const UnkObject_0x100* other);
void fn_71002b6094(UnkObject_0x100* self);

struct UnkObject_0x100 {
    u64 field_0x0[0x20];

    __attribute__((always_inline)) UnkObject_0x100(const UnkObject_0x100& other) { fn_71002b58c0(this, &other); }
    __attribute__((always_inline)) ~UnkObject_0x100() { fn_71002b6094(this); }
};

struct UnkWordObject {
    u32 field_0x0;
    UnkObject_0x100 object;
};

__attribute__((flatten)) void fn_71002a1778(base::UnkVector<UnkWordObject>* self, const UnkWordObject* value) {
    self->PushBackCopy(*value);
}

__attribute__((flatten)) void fn_7100279ca0(base::UnkVector<UnkWordObject>* self, const UnkWordObject* value) {
    self->PushBackCopy(*value);
}
