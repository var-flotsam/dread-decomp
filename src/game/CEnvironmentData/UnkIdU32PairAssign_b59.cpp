// CEnvironmentData::SFogTransition's unit (tools/tu_map.py): copy-assignment of the cached (key,
// element) value of an iterator (as src/game/CEnvironmentData/UnkIdValuePairVariant_b42.cpp: owned in
// state 1, referring to the iterator's key and element in state 2), for elements {vtable, string id,
// u32}. Either side may be a reference; an empty source assigns nothing. Seven copies, one per element
// class. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdU32Elem_b59 {
    char* vtable;
    base::global::CStrId id;
    u32 value;
    UnkIdU32Elem_b59& operator=(const UnkIdU32Elem_b59& o) {
        id = o.id;
        value = o.value;
        return *this;
    }
};

struct UnkIdU32Pair_b59 {
    struct Value {
        u32 key;
        UnkIdU32Elem_b59 element;
    };
    struct Ref {
        u32* key;
        UnkIdU32Elem_b59* element;
    };
    union {
        Value value;
        Ref ref;
    };
    u32 state;

    u32& Key() { return state == 2 ? *ref.key : value.key; }
    UnkIdU32Elem_b59& Element() { return state == 2 ? *ref.element : value.element; }
    const u32& Key() const { return state == 2 ? *ref.key : value.key; }
    const UnkIdU32Elem_b59& Element() const { return state == 2 ? *ref.element : value.element; }

    __attribute__((always_inline)) UnkIdU32Pair_b59& operator=(const UnkIdU32Pair_b59& o) {
        switch (o.state) {
        case 1:
            Key() = o.value.key;
            Element() = o.value.element;
            break;
        case 2:
            Key() = *o.ref.key;
            Element() = *o.ref.element;
            break;
        }
        return *this;
    }
};

#define UNK_ID_U32_PAIR_ASSIGN_B59(addr)                                                  \
    UnkIdU32Pair_b59* fn_##addr(UnkIdU32Pair_b59* self, const UnkIdU32Pair_b59* other) { \
        return &(*self = *other);                                                         \
    }

UNK_ID_U32_PAIR_ASSIGN_B59(7100f5f0f0)
UNK_ID_U32_PAIR_ASSIGN_B59(7100f617cc)
UNK_ID_U32_PAIR_ASSIGN_B59(7100f6a894)
UNK_ID_U32_PAIR_ASSIGN_B59(7100f70180)
UNK_ID_U32_PAIR_ASSIGN_B59(7100f722ec)
UNK_ID_U32_PAIR_ASSIGN_B59(7100f74aac)
UNK_ID_U32_PAIR_ASSIGN_B59(7100f76ccc)
