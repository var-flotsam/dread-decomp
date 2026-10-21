// Copy-assignment of a callback slot (four identical copies): copies the id and the target,
// drops what the slot held (kind 1 owns an object, deleted through vtable slot 1; kind 2 is a
// plain binding), then binds the slot to the new target (kind 2) if there is one.
#include "types.h"

struct UnkOwnedCallable {
    virtual void vf_0x0();
    virtual void vf_0x8();  // deletes the object
};

struct UnkCallbackSlot {
    u32 id;
    void* target;
    void* field_0x10;
    union {
        UnkOwnedCallable* owned;  // kind 1
        void* bound;              // kind 2
    };
    u32 kind;

    void Reset() {
        if (kind == 1) {
            UnkOwnedCallable* p = owned;
            owned = nullptr;
            if (p)
                p->vf_0x8();
        } else if (kind != 2) {
            return;
        }
        kind = 0;
    }

    // The same, written as a switch (kept apart from Reset() in the original).
    void Clear() {
        switch (kind) {
        case 1: {
            UnkOwnedCallable* p = owned;
            owned = nullptr;
            if (p)
                p->vf_0x8();
            kind = 0;
            break;
        }
        case 2:
            kind = 0;
            break;
        }
    }

    UnkCallbackSlot& Assign(const UnkCallbackSlot& other) {
        if (this != &other) {
            id = other.id;
            void* t = other.target;
            target = t;
            if (t) {
                Reset();
                kind = 2;
                field_0x10 = this;
                bound = t;
            } else {
                Clear();
            }
        }
        return *this;
    }
};

UnkCallbackSlot& fn_7100fb5cac(UnkCallbackSlot* self, const UnkCallbackSlot* other) {
    return self->Assign(*other);
}

UnkCallbackSlot& fn_7100fb5d5c(UnkCallbackSlot* self, const UnkCallbackSlot* other) {
    return self->Assign(*other);
}

UnkCallbackSlot& fn_7100fb6104(UnkCallbackSlot* self, const UnkCallbackSlot* other) {
    return self->Assign(*other);
}

UnkCallbackSlot& fn_7100fb61b4(UnkCallbackSlot* self, const UnkCallbackSlot* other) {
    return self->Assign(*other);
}
