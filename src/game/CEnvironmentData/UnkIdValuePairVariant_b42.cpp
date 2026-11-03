// The cached (key, element) value of the iterators over arrays of 0x28-byte polymorphic {vtable,
// string id, 16-byte value} elements (base::UnkIdValueElem, include/base/UnkCloneHelpers.h): empty
// (0), owned (1) or referring to the iterator's key and element (2). Its move constructor leaves the
// new value owned either way, the element copied by its copy constructor. Four copies, one per
// element class: two in CEnvironmentData::SFogTransition's unit and two in unit 2601 (tools/tu_map.py).
// Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "types.h"

extern char g_unk_7101cab220[];
extern char g_unk_7101cab400[];

template <char* Vtable>
struct UnkIdValuePairTag_b42a3;

template <char* Vtable>
struct base::UnkIdValueTraits<UnkIdValuePairTag_b42a3<Vtable>> {
    static char* vtable() { return Vtable; }
};

template <char* Vtable>
struct UnkIdValuePair_b42a3 {
    using Tag = UnkIdValuePairTag_b42a3<Vtable>;
    using Elem = base::UnkIdValueElem<Tag, UnkCloneElem<Tag, 16>>;
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
static inline void MoveConstruct_b42a3(UnkIdValuePair_b42a3<Vtable>* self, const UnkIdValuePair_b42a3<Vtable>* other) {
    using Elem = typename UnkIdValuePair_b42a3<Vtable>::Elem;
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        self->value.key = other->value.key;
        new (&self->value.element) Elem(other->value.element);
    } else if (other->state == 2) {
        self->state = 1;
        typename UnkIdValuePair_b42a3<Vtable>::Ref ref = other->ref;
        self->value.key = *ref.key;
        new (&self->value.element) Elem(*ref.element);
    }
}

#define UNK_ID_VALUE_PAIR_MOVE_B42A3(addr, vtable)                                      \
    void fn_##addr(void* dst, const void* src) {                                         \
        MoveConstruct_b42a3(static_cast<UnkIdValuePair_b42a3<vtable>*>(dst),             \
                            static_cast<const UnkIdValuePair_b42a3<vtable>*>(src));      \
    }

// Signatures as src/reflection/ declares them.
UNK_ID_VALUE_PAIR_MOVE_B42A3(7100f5ca7c, g_unk_7101cab220)
UNK_ID_VALUE_PAIR_MOVE_B42A3(7100f5cfb0, g_unk_7101cab220)
UNK_ID_VALUE_PAIR_MOVE_B42A3(7100f83c44, g_unk_7101cab400)
UNK_ID_VALUE_PAIR_MOVE_B42A3(7100f84150, g_unk_7101cab400)
