// CPerceptionComponent unit (tools/tu_map.py unit 1486): two destructors, copies of fn_71007cebf0 and fn_71007cec94
// (CXParasiteDropComponent_Dtor_b49a3.cpp) with their own vtable and base. The vtable, the list of
// owned objects at +0x58 (the layout of game_s2/UnkOwnedList.h, but each element destroyed
// through its vtable slot at +0x8) destroyed, then the base destructor fn_71011acd74;
// fn_71009c7d24 also frees the object. Names and layouts are placeholders.
#include "game/UnkComponent.h"
#include "base/UnkVector.h"
#include "types.h"

// Signature as src/game/CCharClassCollisionComponent/CCharClassCollisionComponent.cpp declares it.
struct UnkIdObject;
void fn_71011acd74(UnkIdObject* self);

extern void* g_unk_7101a88c60[];

struct UnkPerceptionOwned_b52 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkPerceptionOwnedPtr_b52 {
    // A union member, as UnkOwnedPtr: the count is reloaded after every element.
    union {
        UnkPerceptionOwned_b52* ptr;
    };
    ~UnkPerceptionOwnedPtr_b52() {
        UnkPerceptionOwned_b52* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x8();
    }
};

struct UnkPerceptionOwnedList_b52 {
    union {
        UnkPerceptionOwnedPtr_b52* data;
    };
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkPerceptionOwnedList_b52() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~UnkPerceptionOwnedPtr_b52();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkCPerceptionComponent_b52 : UnkBaseComponent {
    u8 pad_0x40[0x18];
    UnkPerceptionOwnedList_b52 list;  // 0x58
};

__attribute__((always_inline)) static inline void Destroy_b52(UnkCPerceptionComponent_b52* self) {
    self->vtable = g_unk_7101a88c60 + 2;
    self->list.~UnkPerceptionOwnedList_b52();
    fn_71011acd74(reinterpret_cast<UnkIdObject*>(self));
}

void fn_71009c7c80(UnkCPerceptionComponent_b52* self) {
    Destroy_b52(self);
}

void fn_71009c7d24(UnkCPerceptionComponent_b52* self) {
    Destroy_b52(self);
    operator delete(self);
}
