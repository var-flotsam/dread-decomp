// Unit 2535 of tools/tu_map.py (no name hint): reflected invokers over an array of type-erased values built from
// the object at +8 of the call's object (fn_7100ad2fac): one returns it as a value of the list type of the class
// attribute, the other joins it into a string id. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU2535.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::global::CRntString;
using base::global::CStrId;

struct UnkReflType;
char* fn_7100ad2fac(char* self, char* source);

struct UnkValue_a5368;

// Function table of a type-erased value; only the destroy slot is named.
struct UnkValueOps_a5368 {
    void* field_0x0[8];
    void (*destroy)(UnkValue_a5368* self);
};

struct UnkValue_a5368 {
    void* storage;
    union {
        UnkValueOps_a5368* ops;
    };

    ~UnkValue_a5368() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};

// The values the callee builds in place.
struct UnkValueList_a5368 {
    union {
        base::UnkVector<UnkValue_a5368> values;
    };
    UnkValueList_a5368() {}
    ~UnkValueList_a5368() { values.Free(); }
};

UnkReflValue fn_7100da5368(void*, char* obj) {
    UnkValueList_a5368 list;
    fn_7100ad2fac(reinterpret_cast<char*>(&list), *reinterpret_cast<char**>(obj + 8) + 8);
    const CType* type = reinterpret_cast<const CType*>(fn_710025d654());
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &list, 0);
    return r;
}


CRntString fn_7100ad3614(void* source);
struct UnkValue_a5184;

// Function table of a type-erased value; only the destroy slot is named.
struct UnkValueOps_a5184 {
    void* field_0x0[8];
    void (*destroy)(UnkValue_a5184* self);
};

struct UnkValue_a5184 {
    void* storage;
    union {
        UnkValueOps_a5184* ops;
    };

    ~UnkValue_a5184() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};

// The values the callee builds in place.
struct UnkValueList_a5184 {
    union {
        base::UnkVector<UnkValue_a5184> values;
    };
    UnkValueList_a5184() {}
    ~UnkValueList_a5184() { values.Free(); }
};

// Room for a string id that the out-of-line constructor (fn_7100000850) builds; it is left
// without a destructor, since its string is always handed on.
union UnkStrIdSlot_a5184 {
    CStrId id;
    UnkStrIdSlot_a5184() {}
    ~UnkStrIdSlot_a5184() {}
};

union UnkStrHolder_a5184 {
    CRntString str;
    UnkStrHolder_a5184() {}
    ~UnkStrHolder_a5184() {}
};

CStrId* fn_7100da5184(CStrId* self, char* source) {
    UnkStrIdSlot_a5184 slot;
    UnkStrHolder_a5184 holder;
    UnkValueList_a5184 list;
    fn_7100ad2fac(reinterpret_cast<char*>(&list), source + 8);
    new (&holder.str) CRntString(fn_7100ad3614(&list));
    fn_7100000850(&slot.id, holder.str, true);
    self->ReleaseAndTake(slot.id);
    holder.str.~CRntString();
    return self;
}
