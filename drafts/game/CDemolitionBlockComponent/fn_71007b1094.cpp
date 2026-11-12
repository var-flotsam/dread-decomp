// CDemolitionBlockComponent's unit (tools/tu_map.py unit 1211): stores a value at +0x7c, makes sure the component
// is set up (slot 0x88 when the flag at +0x78 is clear), sends the owner's +0x220 object a "move" request,
// hooks the callback fn_71007b1a24 up to it (link at +0xc0, handle at +0xd8) and switches the owner's +0x2a0
// component off. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
u32 fn_71007b1a24(char* p0);
__attribute__((visibility("hidden"))) extern const char g_unk_71015c6b1a[];  // "move"

// The all-bits mask of the request, built once behind two guards (its flag value, then the mask).
extern u32 g_unk_7101d57cb8;
extern u64 g_unk_7101d57cc0;
extern u8 g_unk_7101d57cc8;
extern u64 g_unk_7101d57cd0;
extern "C" int __cxa_guard_acquire(u64*);
extern "C" void __cxa_guard_release(u64*);

static inline u32 MoveMask() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_7101d57cc0), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_7101d57cc0)) {
            if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_7101d57cd0), __ATOMIC_ACQUIRE) & 1), 0)) {
                if (__cxa_guard_acquire(&g_unk_7101d57cd0)) {
                    *reinterpret_cast<u32*>(&g_unk_7101d57cc8) = 0;
                    __cxa_guard_release(&g_unk_7101d57cd0);
                }
            }
            g_unk_7101d57cb8 = (u8)~g_unk_7101d57cc8;
            __cxa_guard_release(&g_unk_7101d57cc0);
        }
    }
    return g_unk_7101d57cb8;
}

struct UnkDemolitionOwner {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
    u8 pad_0x228[0x78];
    UnkStruct_710024fa0c* field_0x2a0;
};

struct UnkDemolitionLink {
    u8 pad_0x0[0x8];
    void* context;
    u32 (*callback)(char*);
};

struct UnkDemolitionBlock {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkDemolitionOwner* owner;  // 0x30
    u8 pad_0x38[0x40];
    bool field_0x78;
    u8 pad_0x79[3];
    f32 field_0x7c;
    u8 pad_0x80[0x40];
    UnkDemolitionLink link;  // 0xc0
    UnkWeakHandle* handle;   // 0xd8
};

void fn_71007b1094(UnkDemolitionBlock* self, f32 value) {
    self->field_0x7c = value;
    if (!self->field_0x78)
        (reinterpret_cast<void (*)(UnkDemolitionBlock*)>((*reinterpret_cast<void***>(self))[0x88 / 8]))(self);
    if (UnkWeakTarget* target = self->owner->field_0x220) {
        u64 hash = fn_7100001570(g_unk_71015c6b1a, 4);
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = MoveMask();
        fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        UnkDemolitionLink* link = &self->link;
        link->context = self;
        link->callback = fn_71007b1a24;
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* t = handle->target)
                fn_710117b3f0(t, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        // keeps self in its own register across the handle reset
        asm("" : "+r"(self));
        fn_7100316018(target, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    if (UnkStruct_710024fa0c* comp = self->owner->field_0x2a0)
        fn_710024fa0c(comp, false);
}
