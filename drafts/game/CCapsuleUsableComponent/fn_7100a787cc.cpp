// CCapsuleUsableComponent unit (tools/tu_map.py, unit 1584): once usable and not yet done (+0x650), resolves
// the first lazy reference (+0x5b0), sends its object's +0x220 mode 0, hooks the listener at +0x628 to it, then
// resolves the second (+0x5f0) and sends that one mode 2; without a first object it calls vf_0x2b8(1).
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflStatics.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;
extern UnkDesc_SpitSend g_unk_7172769740;

void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_7100a78e44(void* self);

struct UnkCapsuleHolder_la787cc {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
};

struct UnkCapsuleCounted_la787cc : base::UnkRefCounted {
    UnkCapsuleHolder_la787cc* holder;  // 0x10
};

struct UnkRef {
    UnkCapsuleCounted_la787cc* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

// A reference resolved from its source on first use (state 1 -> 2).
struct UnkLazyRef_la787cc {
    u8 source[0x28];
    u32 state;
    u32 pad_0x2c;
    base::UnkRefHandle handle;

    UnkCapsuleCounted_la787cc* Get() {
        if (state == 1) {
            {
                UnkRef fresh = fn_7100353be8(source);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        return static_cast<UnkCapsuleCounted_la787cc*>(handle.ptr);
    }
};

struct UnkCapsuleHash_la787cc {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkCapsuleConfig_la787cc {
    u8 pad_0x0[0x1d8];
    UnkCapsuleHash_la787cc* field_0x1d8;
    u8 pad_0x1e0[0x8];
    UnkCapsuleHash_la787cc* field_0x1e8;
};

struct UnkCapsule_la787cc;

struct UnkCapsuleVtable_la787cc {
    u8 pad_0x0[0x2b8];
    void (*vf_0x2b8)(UnkCapsule_la787cc* self, bool value);
    u8 pad_0x2c0[0x28];
    bool (*vf_0x2e8)(UnkCapsule_la787cc* self);
};

struct UnkCapsuleListener_la787cc {
    void* field_0x0;
    void* owner;
    void (*callback)(void*);
};

struct UnkCapsule_la787cc {
    UnkCapsuleVtable_la787cc* vtable;
    u8 pad_0x8[0x5b0 - 0x8];
    UnkLazyRef_la787cc first;   // 0x5b0
    u8 pad_0x5e8[0x8];
    UnkLazyRef_la787cc second;  // 0x5f0
    UnkCapsuleListener_la787cc listener;  // 0x628
    UnkWeakHandle* handle;      // 0x640
    UnkCapsuleConfig_la787cc* config;     // 0x648
    bool done;                  // 0x650
};
static_assert(__builtin_offsetof(UnkCapsule_la787cc, done) == 0x650, "layout");

void fn_7100a787cc(UnkCapsule_la787cc* self) {
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
    if (!self->vtable->vf_0x2e8(self))
        return;
    if (self->done)
        return;
    self->done = true;
    UnkCapsuleCounted_la787cc* c = self->first.Get();
    UnkWeakTarget* target;
    if (!c || !c->holder || !(target = c->holder->field_0x220)) {
        self->vtable->vf_0x2b8(self, true);
        return;
    }
    u64 hash = self->config->field_0x1d8->hash;
    mode.id = 0;
    mask.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    self->listener.owner = self;
    self->listener.callback = fn_7100a78e44;
    UnkCapsuleListener_la787cc* link = &self->listener;
    UnkWeakTarget* track = self->first.Get()->holder->field_0x220;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* t = handle->target)
            fn_710117b3f0(t, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    asm("" : "+r"(self));
    if (track) {
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    UnkCapsuleCounted_la787cc* c2 = self->second.Get();
    if (!c2 || !c2->holder)
        return;
    UnkWeakTarget* target2 = c2->holder->field_0x220;
    if (!target2)
        return;
    u64 hash2 = self->config->field_0x1e8->hash;
    mode2.id = 2;
    mask2.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
    fn_7101176690(target2, hash2, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958, &g_unk_7172769740, &mask2);
}
