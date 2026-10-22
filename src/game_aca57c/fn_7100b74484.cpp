// Getters that return a typed value (a string id, reflected by fn_7100009658): built from a
// string member of the holder's object (four string members), or empty.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkValue_0x18 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    u32 kind = 2;
};

struct UnkString;
void fn_7100000850(CStrId* out, const UnkString* str, bool flag);  // id from a string

// An id built from a string out of line, released like CStrId.
struct UnkBuiltId {
    union {
        CStrId id;
    };

    explicit UnkBuiltId(const UnkString* str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltId() { id.~CStrId(); }
};

struct UnkType;
const UnkType* fn_7100009658();  // refl_GetType<base::global::CStrId>
void fn_71000a18d4(UnkValue_0x18* out, const UnkType* type, void* data, bool flag);

struct UnkHolder {
    u8 unk_0x0[0x8];
    char* object;
};

template <u32 Offset>
__attribute__((always_inline)) static inline UnkValue_0x18 GetId(const UnkHolder* holder) {
    UnkBuiltId id(reinterpret_cast<const UnkString*>(holder->object + Offset));
    const UnkType* type = fn_7100009658();
    UnkValue_0x18 result;
    fn_71000a18d4(&result, type, &id.id, false);
    return result;
}

UnkValue_0x18 fn_7100b74484(void* self, const UnkHolder* holder) { return GetId<0xe18>(holder); }
UnkValue_0x18 fn_7100b74580(void* self, const UnkHolder* holder) { return GetId<0x2e0>(holder); }
UnkValue_0x18 fn_7100b746b8(void* self, const UnkHolder* holder) { return GetId<0x288>(holder); }
UnkValue_0x18 fn_7100b87318(void* self, const UnkHolder* holder) { return GetId<0x1c0>(holder); }

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
__attribute__((always_inline)) static inline UnkValue_0x18 GetEmptyId() {
    UnkEmptyStrId id;
    const UnkType* type = fn_7100009658();
    UnkValue_0x18 result;
    fn_71000a18d4(&result, type, &id.id, false);
    return result;
}

UnkValue_0x18 fn_7100b631e8(void* self, const UnkHolder* holder) { return GetEmptyId(); }
UnkValue_0x18 fn_7100b8719c(void* self, const UnkHolder* holder) { return GetEmptyId(); }
