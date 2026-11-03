// base::core::CBaseObject's unit (unit 29 of tools/tu_map.py): the constructor of a class
// (vtable g_unk_7101a196f0) that makes sure base::core::CBaseObject's class exists
// (fn_7100023464), then runs fn_71000e7b5c over each 0xb0-byte entry of that class's array at
// +0xe8 (g_unk_7101d4ea78, inside the getter's static) with the object's dynamic class and the
// object as the context, as fn_7100023854 does. Names are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "types.h"

using base::reflection::CClass;

extern void* g_unk_7101a196f0[];
extern base::reflection::UnkReflArray g_unk_7101d4ea78 __attribute__((visibility("hidden")));

// The entry walk's types as src/base/core/CBaseObject/fn_7100023854.cpp declares them.
struct UnkNode;

struct UnkClassEntryContext {
    const CClass* cls;
    UnkNode* object;
};

struct UnkClassEntry {
    u8 data[0xb0];
};

void fn_71000e7b5c(UnkClassEntry* entry, UnkClassEntryContext* context);

struct UnkBaseObject_7100024e38 {
    void** vtable;

    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkBaseObject_7100024e38*)>(vtable[0])(this); }
};

void fn_7100024e38(UnkBaseObject_7100024e38* self) {
    self->vtable = g_unk_7101a196f0 + 2;
    fn_7100023464();
    const CClass* cls = self->vf_0x0();
    base::reflection::UnkReflArray* entries = &g_unk_7101d4ea78;
    asm("" : "+r"(entries));
    u32 count = entries->count;
    UnkClassEntryContext context;
    context.cls = cls;
    context.object = reinterpret_cast<UnkNode*>(self);
    if (count != 0) {
        auto* entry = static_cast<UnkClassEntry*>(entries->data);
        for (auto* end = entry + count; entry != end; ++entry)
            fn_71000e7b5c(entry, &context);
    }
}
