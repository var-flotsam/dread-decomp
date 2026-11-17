// CBigkranXSpitAttack's unit: the copy-assignment of a record of the attack (its vtable stays): the
// plain members are copied, the two weak references take the weak handle of what the other's refer
// to, the two slots are assigned by fn_71002a5d1c and the list at +0x100 by fn_71000cf3b4.
// Names and layouts are placeholders.
#include "game/UnkTU365.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

template <class T>
struct UnkCopiedVector;
struct UnkWord;
UnkCopiedVector<UnkWord>* fn_71000cf3b4(UnkCopiedVector<UnkWord>* self, const UnkCopiedVector<UnkWord>* other);


struct UnkSpitSlot_3f3e04 {
    void* ptr;

    UnkSpitSlot_3f3e04& operator=(const UnkSpitSlot_3f3e04& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

// A growable array (base::global::CRntVector's layout) copied element by element.
struct UnkSpitList_3f3e04 {
    u8 storage[0x20];

    UnkSpitList_3f3e04& operator=(const UnkSpitList_3f3e04& other) {
        fn_71000cf3b4(reinterpret_cast<UnkCopiedVector<UnkWord>*>(this),
                      reinterpret_cast<const UnkCopiedVector<UnkWord>*>(&other));
        return *this;
    }
};

// Three floats assigned one by one.
struct UnkSpitVec3_3f3e04 {
    f32 x, y, z;

    UnkSpitVec3_3f3e04& operator=(const UnkSpitVec3_3f3e04& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkSpitPair_3f3e04 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkSpitBlock_3f3e04 {
    u64 field_0x0[6];
};

struct UnkSpitWords_3f3e04 {
    u32 field_0x0[7];
};

// The aim: two slots, four vectors and a float; assigning to itself does nothing.
struct UnkSpitAim_3f3e04 {
    UnkSpitSlot_3f3e04 field_0x0;
    UnkSpitSlot_3f3e04 field_0x8;
    UnkSpitVec3_3f3e04 field_0x10;
    UnkSpitVec3_3f3e04 field_0x1c;
    UnkSpitVec3_3f3e04 field_0x28;
    UnkSpitVec3_3f3e04 field_0x34;
    f32 field_0x40;

    UnkSpitAim_3f3e04& operator=(const UnkSpitAim_3f3e04& other) {
        if (this != &other) {
            field_0x0 = other.field_0x0;
            field_0x8 = other.field_0x8;
            field_0x10 = other.field_0x10;
            field_0x1c = other.field_0x1c;
            field_0x28 = other.field_0x28;
            field_0x34 = other.field_0x34;
            field_0x40 = other.field_0x40;
        }
        return *this;
    }
};

struct UnkSpitRecord_3f3e04 {
    virtual ~UnkSpitRecord_3f3e04();

    UnkSpitBlock_3f3e04 field_0x8;
    u64 field_0x38;  // not copied by the assignment
    UnkSpitPair_3f3e04 field_0x40;
    UnkWeakHandle* field_0x50;  // a weak reference
    UnkSpitVec3_3f3e04 field_0x58;
    UnkSpitVec3_3f3e04 field_0x64;
    UnkSpitVec3_3f3e04 field_0x70;
    f32 field_0x7c;
    bool field_0x80;
    UnkSpitAim_3f3e04 field_0x88;
    UnkSpitWords_3f3e04 field_0xd0;
    UnkSpitVec3_3f3e04 field_0xec;
    UnkWeakHandle* field_0xf8;  // a weak reference
    UnkSpitList_3f3e04 field_0x100;
};
static_assert(__builtin_offsetof(UnkSpitRecord_3f3e04, field_0x40) == 0x40, "layout");
static_assert(__builtin_offsetof(UnkSpitRecord_3f3e04, field_0xd0) == 0xd0, "layout");
static_assert(__builtin_offsetof(UnkSpitRecord_3f3e04, field_0xec) == 0xec, "layout");
static_assert(__builtin_offsetof(UnkSpitRecord_3f3e04, field_0xf8) == 0xf8, "layout");
static_assert(__builtin_offsetof(UnkSpitRecord_3f3e04, field_0x100) == 0x100, "layout");

UnkSpitRecord_3f3e04* fn_71003f3e04(UnkSpitRecord_3f3e04* self, const UnkSpitRecord_3f3e04* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x40 = other->field_0x40;
    if (self != other)
        CopyWeakRef(&self->field_0x50, other->field_0x50);
    self->field_0x58 = other->field_0x58;
    self->field_0x64 = other->field_0x64;
    self->field_0x70 = other->field_0x70;
    self->field_0x7c = other->field_0x7c;
    self->field_0x80 = other->field_0x80;
    self->field_0x88 = other->field_0x88;
    self->field_0xd0 = other->field_0xd0;
    self->field_0xec = other->field_0xec;
    if (self != other)
        CopyWeakRef(&self->field_0xf8, other->field_0xf8);
    self->field_0x100 = other->field_0x100;
    return self;
}
