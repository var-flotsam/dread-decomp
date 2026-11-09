// CTriggerComponent unit (tools/tu_map.py): owning pointers to the unit's 0x60-byte records
// (fn_7100a632e8 destroys one, fn_7100083c00 frees it; src/game/CTriggerComponent/CTriggerComponent_Record_ovb0067.cpp).
// fn_7100a63f40 frees an array of them; fn_7100a6371c move-assigns a (key, record) pair into a
// two-state variant that holds either the pair itself (state 1) or pointers to a pair kept elsewhere
// (state 2), the variant of src/game/CTriggerComponent/CTriggerComponent_OwnedIterSync_b56.cpp. Names
// and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkTriggerOwned_b56;
void fn_7100a632e8(UnkTriggerOwned_b56* self);

static inline void DestroyOwned(UnkTriggerOwned_b56* p) {
    if (p) {
        fn_7100a632e8(p);
        fn_7100083c00(p);
    }
}

struct UnkTriggerOwnedPtr_ovb0067 {
    // A union member: the array is reloaded after every element, as in the original.
    union {
        UnkTriggerOwned_b56* ptr;
    };

    ~UnkTriggerOwnedPtr_ovb0067() {
        UnkTriggerOwned_b56* p = ptr;
        ptr = nullptr;
        DestroyOwned(p);
    }
};

void fn_7100a63f40(base::UnkVector<UnkTriggerOwnedPtr_ovb0067>* self) {
    self->Free();
}

struct UnkTriggerKeyed_ovb0067 {
    u32 key;
    UnkTriggerOwned_b56* owned;
};

struct UnkTriggerKeyedVariant_ovb0067 {
    union {
        UnkTriggerKeyed_ovb0067 object;
        struct {
            u32* first;
            UnkTriggerOwned_b56** second;
        } pair;
    };
    s32 state;

    u32& Key() { return state == 2 ? *pair.first : object.key; }
    UnkTriggerOwned_b56*& Owned() { return state == 2 ? *pair.second : object.owned; }

    // Takes the key, then the other's record (`slot` gives the other's slot; it is looked up only
    // after the key is stored), destroying the record held before.
    template <class Slot>
    __attribute__((always_inline)) void Assign(u32 key, Slot slot) {
        Key() = key;
        UnkTriggerOwned_b56*& owned = slot();
        UnkTriggerOwned_b56*& dst = Owned();
        UnkTriggerOwned_b56* p = owned;
        owned = nullptr;
        UnkTriggerOwned_b56* old = dst;
        dst = p;
        DestroyOwned(old);
    }
};

UnkTriggerKeyedVariant_ovb0067* fn_7100a6371c(UnkTriggerKeyedVariant_ovb0067* self,
                                              UnkTriggerKeyedVariant_ovb0067* other) {
    switch (other->state) {
    case 1:
        self->Assign(other->object.key, [other]() -> UnkTriggerOwned_b56*& { return other->object.owned; });
        break;
    case 2:
        self->Assign(*other->pair.first, [other]() -> UnkTriggerOwned_b56*& { return *other->pair.second; });
        break;
    }
    return self;
}
