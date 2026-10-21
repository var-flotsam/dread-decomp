// Four setters taking a reflected value (no value without a type): mark the owner dirty
// (+0x5f), then assign a string id (at +0x8 or +0x10) or a string (at +0x30 or +0x40). They
// return a status record {0, 0, 2, 1}.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkValueType {
    u8 unk_0x0[0x18];
    u64 size;
};

// The value is stored in place when its type fits in 8 bytes, unless kind is 1 (by reference).
struct UnkValueArg {
    void* storage;
    UnkValueType* type;
    u8 unk_0x10[0x4];
    s32 kind;

    template <class T>
    const T* Get() const {
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

void fn_710007b518(CRntString* self, const CRntString* other);  // assignment

template <u32 Offset>
__attribute__((always_inline)) static inline UnkSetStatus SetStrId(UnkValueHolder* holder, UnkValueArg* const* arg) {
    char* owner = holder->owner;
    const CStrId* value = (*arg)->Get<CStrId>();
    owner[0x5f] = true;
    *reinterpret_cast<CStrId*>(owner + Offset) = *value;
    return Done();
}

template <u32 Offset>
__attribute__((always_inline)) static inline UnkSetStatus SetString(UnkValueHolder* holder, UnkValueArg* const* arg) {
    char* owner = holder->owner;
    const CRntString* value = (*arg)->Get<CRntString>();
    owner[0x5f] = true;
    fn_710007b518(reinterpret_cast<CRntString*>(owner + Offset), value);
    return Done();
}

UnkSetStatus fn_7100d9c2b8(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x8>(holder, arg);
}
UnkSetStatus fn_7100d9c3cc(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetStrId<0x10>(holder, arg);
}
UnkSetStatus fn_7100d9c638(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetString<0x30>(holder, arg);
}
UnkSetStatus fn_7100d9c6ec(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return SetString<0x40>(holder, arg);
}
