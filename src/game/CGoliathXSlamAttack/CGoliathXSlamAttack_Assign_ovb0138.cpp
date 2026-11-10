// CGoliathXSlamAttack's unit (tools/tu_map.py): copy assignment of the attack's data: plain fields
// at +0x8, the name at +0x40, two words at +0x50, the weak reference to the target at +0x60 (it takes
// the weak handle of what the other refers to, created on first use) and a word at +0x68. Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A weak reference to an actor: assigning takes the weak handle of what the other refers to.
struct UnkSlamWeakRef_ovb0138 {
    UnkWeakHandle* handle;

    __attribute__((always_inline)) UnkSlamWeakRef_ovb0138& operator=(const UnkSlamWeakRef_ovb0138& other) {
        if (this != &other)
            CopyWeakRef(&handle, other.handle);
        return *this;
    }
};

// Plain fields copied as two blocks (0x20 bytes, then 0x14).
struct UnkSlamBlock_ovb0138 {
    u64 words[6];
    u32 field_0x30;
};

struct UnkSlamAttackData_ovb0138 {
    void* vtable;
    UnkSlamBlock_ovb0138 field_0x8;
    base::UnkStrIdMember name;  // 0x40
    u8 pad_0x48[8];
    u64 field_0x50;
    u64 field_0x58;
    UnkSlamWeakRef_ovb0138 target;  // 0x60
    u64 field_0x68;
};
static_assert(__builtin_offsetof(UnkSlamAttackData_ovb0138, field_0x68) == 0x68, "layout");

UnkSlamAttackData_ovb0138* fn_7100587694(UnkSlamAttackData_ovb0138* self, const UnkSlamAttackData_ovb0138* other) {
    __builtin_memcpy(&self->field_0x8, &other->field_0x8, 0x20);
    __builtin_memcpy(&self->field_0x8.words[4], &other->field_0x8.words[4], 0x14);
    self->name = other->name;
    __builtin_memcpy(&self->field_0x50, &other->field_0x50, 0x10);
    self->target = other->target;
    self->field_0x68 = other->field_0x68;
    return self;
}
