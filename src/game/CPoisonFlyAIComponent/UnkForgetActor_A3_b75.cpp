// fn_710063a748: the poison-fly manager part forgets an actor: finds the 0x30-byte entry whose
// ref-counted handle (+0x20) points at the actor, releases the handle (vf 0x10 when the count reaches
// zero) and clears it. Names are placeholders.
#include "game/UnkActor.h"
#include "game/UnkTU958.h"
#include "types.h"

struct UnkPoisonFlyHandle_A3_b75 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 refcount;
    UnkActor* actor;
};

struct UnkPoisonFlyEntry_A3_b75 {
    u8 pad_0x0[0x20];
    UnkPoisonFlyHandle_A3_b75* handle;
    u8 pad_0x28[8];
};

struct UnkPoisonFlyPart_A3_b75 {
    void* field_0x0;
    UnkPoisonFlyEntry_A3_b75* data;
    u32 count;
};

static inline __attribute__((always_inline)) UnkPoisonFlyEntry_A3_b75* UnkPoisonFlyFind_A3_b75(
    UnkPoisonFlyPart_A3_b75* self, UnkActor* actor) {
    for (UnkPoisonFlyEntry_A3_b75 *e = self->data, *end = e + self->count; e != end; ++e) {
        UnkPoisonFlyHandle_A3_b75* h = e->handle;
        if ((h ? h->actor : nullptr) == actor)
            return e;
    }
    return nullptr;
}

void fn_710063a748(u8* part, UnkActor* actor) {
    UnkPoisonFlyEntry_A3_b75* e = UnkPoisonFlyFind_A3_b75(reinterpret_cast<UnkPoisonFlyPart_A3_b75*>(part), actor);
    if (!e)
        return;
    UnkPoisonFlyHandle_A3_b75* h = e->handle;
    if (!h)
        return;
    if (--h->refcount == 0)
        h->vf_0x10();
    e->handle = nullptr;
}
