// Copy-assignment (four identical copies) and advance (two) of a callback slot holding its
// functor inline. The assignment copies the id and the target, drops what the slot held (kind
// 1: the inline functor, which detaches itself from its object; kind 2: a plain binding), then
// binds the slot to the new target (kind 2) if there is one.
#include "types.h"

extern void* g_unk_7101cac988[];

struct UnkBoundObj {
    u8 unk_0x0[0xc8];
    void* field_0xc8;  // the functor attached to it
};

void fn_7100232124(UnkBoundObj* obj);

struct UnkInlineFunctor {
    void** vtable;
    void* field_0x8;
    void* field_0x10;
    // A union member (no type-based alias info): it is reloaded after the object's field is
    // cleared.
    union {
        UnkBoundObj* obj;
    };
    u8 unk_0x20[0x10];

    __attribute__((always_inline)) ~UnkInlineFunctor() {
        vtable = &g_unk_7101cac988[2];
        if (obj->field_0xc8 == this)
            obj->field_0xc8 = nullptr;
        fn_7100232124(obj);
    }
};

struct UnkCallbackSlot {
    u32 id;
    void* target;
    void* field_0x10;
    union {
        UnkInlineFunctor fn;  // kind 1
        void* bound;          // kind 2
    };
    u32 kind;

    void Reset() {
        if (kind == 1) {
            fn.~UnkInlineFunctor();
        } else if (kind != 2) {
            return;
        }
        kind = 0;
    }

    // The same, written as a switch (kept apart from Reset() in the original).
    void Clear() {
        switch (kind) {
        case 1:
            fn.~UnkInlineFunctor();
            kind = 0;
            break;
        case 2:
            kind = 0;
            break;
        }
    }

    // Steps to the next 0x30-byte target and binds the slot to it.
    void Advance() {
        ++id;
        void* t = static_cast<char*>(target) + 0x30;
        target = t;
        Reset();
        kind = 2;
        field_0x10 = this;
        bound = t;
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

UnkCallbackSlot& fn_710106d7f8(UnkCallbackSlot* self, const UnkCallbackSlot* other) {
    return self->Assign(*other);
}

UnkCallbackSlot& fn_710106d8d8(UnkCallbackSlot* self, const UnkCallbackSlot* other) {
    return self->Assign(*other);
}

UnkCallbackSlot& fn_710106dce0(UnkCallbackSlot* self, const UnkCallbackSlot* other) {
    return self->Assign(*other);
}

UnkCallbackSlot& fn_710106ddc0(UnkCallbackSlot* self, const UnkCallbackSlot* other) {
    return self->Assign(*other);
}

void fn_710106d67c(UnkCallbackSlot* self) {
    self->Advance();
}

void fn_710106db64(UnkCallbackSlot* self) {
    self->Advance();
}
