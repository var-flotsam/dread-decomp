// The move constructor of the cached (key, element) value of the iterators over arrays of 0x30-byte
// polymorphic {vtable, string id, 24-byte value} elements (base::UnkIdValueElem,
// include/base/UnkCloneHelpers.h), 0x7101cab338's class in CEnvironmentData::SFogTransition's unit:
// empty (0), owned (1) or referring to the iterator's key and element (2); the new value is left
// owned either way. Same shape as src/game/CEnvironmentData/UnkIdValuePairVariant_b42.cpp (16-byte
// values). Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "types.h"

extern char g_unk_7101cab338[];

template <char* Vtable>
struct UnkIdValuePairTag_A6_b74;

template <char* Vtable>
struct base::UnkIdValueTraits<UnkIdValuePairTag_A6_b74<Vtable>> {
    static char* vtable() { return Vtable; }
};

template <char* Vtable>
struct UnkIdValuePair_A6_b74 {
    using Tag = UnkIdValuePairTag_A6_b74<Vtable>;
    using Elem = base::UnkIdValueElem<Tag, UnkCloneElem<Tag, 24>>;
    struct Value {
        u32 key;
        Elem element;
    };
    struct Ref {
        const u32* key;
        const Elem* element;
    };
    union {
        Value value;
        Ref ref;
    };
    u32 state;
};

template <char* Vtable>
static inline void MoveConstruct_A6_b74(UnkIdValuePair_A6_b74<Vtable>* self, const UnkIdValuePair_A6_b74<Vtable>* other) {
    using Elem = typename UnkIdValuePair_A6_b74<Vtable>::Elem;
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        self->value.key = other->value.key;
        new (&self->value.element) Elem(other->value.element);
    } else if (other->state == 2) {
        self->state = 1;
        typename UnkIdValuePair_A6_b74<Vtable>::Ref ref = other->ref;
        self->value.key = *ref.key;
        new (&self->value.element) Elem(*ref.element);
    }
}

#define UNK_ID_VALUE_PAIR_MOVE_A6_B74(addr, vtable)                                     \
    void fn_##addr(void* dst, const void* src) {                                         \
        MoveConstruct_A6_b74(static_cast<UnkIdValuePair_A6_b74<vtable>*>(dst),           \
                             static_cast<const UnkIdValuePair_A6_b74<vtable>*>(src));    \
    }

// Signatures as src/reflection/ declares them.
UNK_ID_VALUE_PAIR_MOVE_A6_B74(7100f6d66c, g_unk_7101cab338)
UNK_ID_VALUE_PAIR_MOVE_A6_B74(7100f6dbc0, g_unk_7101cab338)
