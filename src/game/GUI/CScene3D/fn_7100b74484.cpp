// Getters that return a typed value (a string id, reflected by fn_7100009658): built from a
// string member of the holder's object (four string members), or empty.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

// An id built from a string out of line, released like CStrId.
struct UnkBuiltId {
    union {
        CStrId id;
    };

    explicit UnkBuiltId(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltId() { id.~CStrId(); }
};

struct UnkHolder {
    u8 unk_0x0[0x8];
    char* object;
};

template <u32 Offset>
__attribute__((always_inline)) static inline UnkReflValue GetId(const UnkHolder* holder) {
    UnkBuiltId id(*reinterpret_cast<const CRntString*>(holder->object + Offset));
    const CType* type = fn_7100009658();  // refl_GetType<base::global::CStrId>
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, type, &id.id, false);
    return result;
}

UnkReflValue fn_7100b74484(void* self, const UnkHolder* holder) { return GetId<0xe18>(holder); }
UnkReflValue fn_7100b74580(void* self, const UnkHolder* holder) { return GetId<0x2e0>(holder); }
UnkReflValue fn_7100b746b8(void* self, const UnkHolder* holder) { return GetId<0x288>(holder); }
UnkReflValue fn_7100b87318(void* self, const UnkHolder* holder) { return GetId<0x1c0>(holder); }

// An empty id (the pool is created first if there is none yet), released like CStrId.
struct UnkEmptyStrId {
    union {
        CStrId id;
    };

    __attribute__((always_inline)) UnkEmptyStrId() {
        id.data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        id.data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyStrId() { id.~CStrId(); }
};

// Two getters that return an empty string id as a typed value.
__attribute__((always_inline)) static inline UnkReflValue GetEmptyId() {
    UnkEmptyStrId id;
    const CType* type = fn_7100009658();  // refl_GetType<base::global::CStrId>
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, type, &id.id, false);
    return result;
}

UnkReflValue fn_7100b631e8(void* self, const UnkHolder* holder) { return GetEmptyId(); }
UnkReflValue fn_7100b8719c(void* self, const UnkHolder* holder) { return GetEmptyId(); }
