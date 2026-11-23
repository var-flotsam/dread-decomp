// CCharClassAttackComponent (unit 1185): the destructor. Frees the string-id array and the id table,
// releases the string ids, then runs the char-class component's destructor. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CCharClassComponent.h"
#include "types.h"

using base::global::CStrId;

template <base::global::UnkStringPool** Pool, bool Reread>
struct UnkReleasedStrIdT;
template <class Value>
struct UnkIdTable;
using UnkReleasedStrId = UnkReleasedStrIdT<&g_unk_7101d47108, true>;
void fn_71000d99d0(UnkIdTable<UnkReleasedStrId>* self);

// A table of values and keys whose clear is out of line; destroying it clears it and frees both arrays.
struct UnkAttackIdTable_l78d144 {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkAttackIdTable_l78d144() {
        fn_71000d99d0(reinterpret_cast<UnkIdTable<UnkReleasedStrId>*>(this));
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// A vector that frees its array when destroyed.
template <class T>
struct UnkOwnedVector_l78d144 : base::global::CRntVector<T> {
    ~UnkOwnedVector_l78d144() { this->Free(); }
};

struct UnkCharClassAttack_l78d144 {
    void** vtable;
    u8 pad_0x8[0x50];
    CStrId id_0x58;
    CStrId id_0x60;
    u8 pad_0x68[0x18];
    CStrId id_0x80;
    CStrId id_0x88;
    CStrId id_0x90;
    CStrId id_0x98;
    UnkAttackIdTable_l78d144 table_0xa0;
    CStrId id_0xc8;
    UnkOwnedVector_l78d144<UnkStrIdTwoStep> ids_0xd0;
};
static_assert(__builtin_offsetof(UnkCharClassAttack_l78d144, ids_0xd0) == 0xd0, "layout");

void fn_710078d144(UnkCharClassAttack_l78d144* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a6f348 + 0x10);
    self->ids_0xd0.~UnkOwnedVector_l78d144();
    self->id_0xc8.~CStrId();
    self->table_0xa0.~UnkAttackIdTable_l78d144();
    self->id_0x98.~CStrId();
    self->id_0x90.~CStrId();
    self->id_0x88.~CStrId();
    self->id_0x80.~CStrId();
    self->id_0x60.~CStrId();
    self->id_0x58.~CStrId();
    fn_71011acd74(reinterpret_cast<UnkIdObject_8da5c0*>(self));
}
