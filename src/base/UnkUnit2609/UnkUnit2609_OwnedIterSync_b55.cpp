// Unit 2609 of tools/tu_map.py (base::reflection names): Sync of two iterators whose current element
// is kept in a two-state variant (the shape of src/game/CLightManager/UnkListIterVariantNext_b43.cpp):
// with a current entry (+8) the variant becomes the pair {iterator, entry}, else it is reset. State 1
// owns an object at +0x18 (fn_7100ef1e1c destroys it, fn_7100083c00 frees it). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkNamedIds;  // src/game/CSubAreaManager.cpp
void fn_7100ef1e1c(UnkNamedIds* self);

struct UnkOwnedObject_b55 {
    u64 field_0x0;
    UnkNamedIds* owned;

    __attribute__((always_inline)) void Destroy() {
        UnkNamedIds* p = owned;
        owned = nullptr;
        if (p) {
            fn_7100ef1e1c(p);
            fn_7100083c00(p);
        }
    }
};

struct UnkOwnedVariant_b55 {
    union {
        UnkOwnedObject_b55 object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            object.Destroy();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    __attribute__((always_inline)) void SetPair(void* first, void* second) {
        s32* p = &state;
        switch (*p) {
        case 1:
            object.Destroy();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = 2;
        pair.first = first;
        pair.second = second;
    }
};

template <int Tag>
struct UnkOwnedIter_b55 {
    void* field_0x0;
    void* current;
    UnkOwnedVariant_b55 variant;

    __attribute__((always_inline)) void Sync() {
        void* c = current;
        if (c)
            variant.SetPair(this, c);
        else
            variant.Reset();
    }
};

void fn_7100f9f5d0(UnkOwnedIter_b55<0>* self) { self->Sync(); }
void fn_7100f9f988(UnkOwnedIter_b55<1>* self) { self->Sync(); }
