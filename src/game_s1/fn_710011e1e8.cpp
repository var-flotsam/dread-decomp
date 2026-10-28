// A handler taking reflected values and returning a status record {0, 0, 2, 1}: as
// fn_710105d328 (src/game/SLogicPathNode/fn_710105d328.cpp), it calls fn_7100122b2c with a value and an
// optional pointer and marks the owner dirty (another owner layout): fn_710011e1e8.
#include "types.h"

struct UnkValueType {
    u8 unk_0x0[0x18];
    u64 size;
};

// The value is stored in place when its type fits in 8 bytes, unless kind is 1 (by reference).
// The values a handler gets are consecutive.
struct UnkValueArg {
    void* storage;
    UnkValueType* type;
    u8 unk_0x10[0x4];
    s32 kind;

    template <class T>
    const T& Get() const {
        if (kind == 1 || type->size >= 9)
            return *static_cast<const T*>(storage);
        return *reinterpret_cast<const T*>(this);
    }
    // No value without a type.
    template <class T>
    const T* GetPtr() const {
        if (!type)
            return nullptr;
        if (kind == 1 || type->size >= 9)
            return static_cast<const T*>(storage);
        return reinterpret_cast<const T*>(this);
    }
};

struct UnkValueHolder {
    u8 unk_0x0[0x8];
    char* owner;
};

struct UnkSetStatus {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

static inline UnkSetStatus Done() {
    UnkSetStatus status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}

struct UnkArgObj;

struct UnkInsertTarget;

// The owner of fn_710105d328 with the target object at 0 and the flag at 0x2d.
struct UnkInsertOwner {
    u8 target[0x2d];  // the object itself
    bool dirty;
};

void fn_7100122b2c(UnkInsertTarget* target, const UnkArgObj* obj, u32 value);
UnkSetStatus fn_710011e1e8(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    UnkInsertOwner* owner = reinterpret_cast<UnkInsertOwner*>(holder->owner);
    const UnkValueArg* args = *arg;
    u32 value = args[0].Get<u32>();
    const UnkArgObj* obj = args[1].GetPtr<UnkArgObj>();
    fn_7100122b2c(reinterpret_cast<UnkInsertTarget*>(owner->target), obj, value);
    owner->dirty = true;
    return Done();
}
