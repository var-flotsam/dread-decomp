// CCharClassBaseTriggerComponent's unit (tools/tu_map.py unit 1562): an actor enters the trigger. When it
// is the player its name id is kept (+0x9b0); the actor's weak handle and the entry info are appended to
// the list of entries (+0xa8) and the trigger is told through its virtual 0x190. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

#include <new>

using base::global::CStrId;

struct UnkTriggerActor_la4734c {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkTriggerInfo_la4734c {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;

    UnkTriggerInfo_la4734c(const UnkTriggerInfo_la4734c& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8) {}
};

struct UnkTriggerArg_la4734c {
    UnkTriggerActor_la4734c* actor;
    UnkTriggerInfo_la4734c info;  // 0x8
};

// A counted reference to a weak handle.
struct UnkHandleRef_la4734c {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_la4734c(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    UnkHandleRef_la4734c(const UnkHandleRef_la4734c& other) : handle(nullptr) {
        AssignRef(&handle, other.handle);
    }
    ~UnkHandleRef_la4734c() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

struct UnkTriggerEntry_la4734c {
    UnkHandleRef_la4734c ref;
    UnkTriggerInfo_la4734c info;

    // The info comes by value: it is read before the reference is taken.
    UnkTriggerEntry_la4734c(const UnkHandleRef_la4734c& r, UnkTriggerInfo_la4734c i) : ref(r), info(i) {}
};

struct UnkTriggerComponent_la4734c;

struct UnkTriggerComponentVtbl_la4734c {
    void* pad_0x0[0x190 / 8];
    void (*vf_0x190)(UnkTriggerComponent_la4734c* self, UnkTriggerActor_la4734c* actor,
                     UnkTriggerInfo_la4734c* info);
};

struct UnkTriggerComponent_la4734c {
    UnkTriggerComponentVtbl_la4734c* vtable;
    u8 pad_0x8[0xa0];
    u32 count;  // 0xa8
    u8 storage[32 * sizeof(UnkTriggerEntry_la4734c)];  // 0xac
    u8 pad_0x3ac[0x9b0 - 0x3ac];
    CStrId player_name;  // 0x9b0

    UnkTriggerEntry_la4734c* items() { return reinterpret_cast<UnkTriggerEntry_la4734c*>(storage); }
    void Add(const UnkHandleRef_la4734c& ref, UnkTriggerInfo_la4734c info) {
        new (&items()[count]) UnkTriggerEntry_la4734c(ref, info);
        ++count;
    }
    void vf_0x190(UnkTriggerActor_la4734c* actor, UnkTriggerInfo_la4734c* info) {
        vtable->vf_0x190(this, actor, info);
    }
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la4734c, player_name) == 0x9b0, "layout");

void fn_7100a4734c(UnkTriggerComponent_la4734c* self, UnkTriggerArg_la4734c* arg) {
    if (reinterpret_cast<UnkPlayer*>(arg->actor) == fn_7100dec788(g_unk_7101d47310))
        self->player_name = arg->actor->name;
    UnkHandleRef_la4734c ref(fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(arg->actor)));
    self->Add(ref, arg->info);
    self->vf_0x190(arg->actor, &arg->info);
}
