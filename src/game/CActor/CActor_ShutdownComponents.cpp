// CActor's unit (unit 341): an actor's shutdown: unregistered from the game manager's object at
// +0x608 when it has a registration (+0x1a8), each component (array +0xa0, count +0xb0) is told
// (vf_0x68), then each component's shared holder (vf_0x28) is emptied, clearing its object's
// back pointer and dropping the reference; then the actor's own vf_0x88. Names and layouts are
// placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

void fn_71011a2a24(UnkGameManagerSub_0x608* registry, void* actor);

struct UnkCounted341 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 count;  // 0x8
    UnkCounted341* owner;  // 0x10
};

struct UnkHolder341 {
    UnkCounted341* object;
};

struct UnkComponent341 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkHolder341* vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
};

struct UnkActor341 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    u8 pad_0x8[0x98];
    UnkComponent341** components;  // 0xa0
    u8 pad_0xa8[0x8];
    u32 component_count;  // 0xb0
    u8 pad_0xb4[0xf4];
    void* registration;  // 0x1a8
};
static_assert(__builtin_offsetof(UnkActor341, registration) == 0x1a8, "UnkActor341 layout");

void fn_7100232a48(UnkActor341* self) {
    if (self->registration)
        fn_71011a2a24(g_unk_7101d47310->field_0x608, self);
    for (UnkComponent341** it = self->components, **end = it + self->component_count; it != end; ++it)
        (*it)->vf_0x68();
    for (UnkComponent341** it = self->components, **end = it + self->component_count; it != end; ++it) {
        UnkHolder341* holder = (*it)->vf_0x28();
        if (holder && holder->object) {
            // The original reloads the holder's pointer after this store.
            typedef UnkCounted341* __attribute__((may_alias)) AliasPtr;
            *reinterpret_cast<AliasPtr*>(&holder->object->owner) = nullptr;
            if (UnkCounted341* object = holder->object) {
                if (--object->count == 0)
                    object->vf_0x10();
                holder->object = nullptr;
            }
        }
    }
    self->vf_0x88();
}
