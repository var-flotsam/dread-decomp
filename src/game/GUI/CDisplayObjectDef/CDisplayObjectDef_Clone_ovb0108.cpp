// GUI::CDisplayObjectDef's unit (tools/tu_map.py): a copy of a reflected object as a new display
// definition (constructed by fn_7100b064bc): the source object is pushed as the objects to write
// (fn_71000ee314, as the blackboard writer in src/game/CBlackboard/CBlackboard_Sections_ovb0026.cpp
// does), the new definition is set up as the read target (fn_71000f0568, mode 0xc, as the GUI
// loaders in src/game/GUI/CGUIManager.cpp do), and fn_7100b09720 reads one into the other with
// this unit's callbacks. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

struct UnkDisplayObjectDef_ovb0108;
void fn_7100b064bc(UnkDisplayObjectDef_ovb0108* self);

struct UnkWriteStack_b0026;
struct UnkWriteRoot_b0026;
void fn_71000ee314(UnkWriteStack_b0026* self, UnkWriteRoot_b0026* root, u32 count);

// The objects to copy: up to 100 entries of three values each (UnkWriteStack_b0026's layout).
struct UnkCopyEntry_ovb0108 {
    UnkReflValue a;
    u8 pad_0x18[0x18];
    UnkReflValue b;
    UnkReflValue c;
    u8 pad_0x60[0x8];
};
struct UnkCopyStack_ovb0108 {
    u8 pad_0x0[0x14];
    u32 count;
    UnkCopyEntry_ovb0108 entries[100];
    u8 pad_0x28b8[0x8];
};
static_assert(sizeof(UnkCopyStack_ovb0108) == 0x28c0, "UnkCopyStack_ovb0108 layout");

struct UnkCopyRoot_ovb0108 {
    char* type;
    void* object;
};

struct UnkCopyCallbacks_ovb0108 {
    u64 words[6];
};
extern UnkCopyCallbacks_ovb0108 g_unk_717275ed88 __attribute__((visibility("hidden")));

// What fn_7100b09720 is given: the callbacks, the objects to copy and the target.
struct UnkCopyContext_ovb0108 {
    UnkCopyCallbacks_ovb0108 callbacks;
    UnkCopyStack_ovb0108* stack;
    UnkReadTarget* target;
};

struct UnkDisplayReader_ovb0108;
s32 fn_7100b09720(UnkDisplayReader_ovb0108* self, UnkReflObject* obj);

UnkDisplayObjectDef_ovb0108* fn_7100b06708(UnkReflObject* source) {
    auto* def = static_cast<UnkDisplayObjectDef_ovb0108*>(operator new(0x90));
    fn_7100b064bc(def);
    UnkCopyContext_ovb0108 ctx;
    UnkCopyStack_ovb0108 stack;
    {
        UnkCopyRoot_ovb0108 root = {fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(source->vf_0x0())), source),
                                    source};
        fn_71000ee314(reinterpret_cast<UnkWriteStack_b0026*>(&stack), reinterpret_cast<UnkWriteRoot_b0026*>(&root), 2);
    }
    UnkReadTarget target;
    {
        auto* type = reinterpret_cast<const base::reflection::CType*>(reinterpret_cast<UnkReflObject*>(def)->vf_0x0());
        UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, def, true);
        fn_71000f0568(&target, &value, 0xc);
        fn_71000a1c60(&value);
    }
    if (stack.count) {
        ctx.stack = &stack;
        ctx.target = &target;
        const UnkCopyCallbacks_ovb0108* callbacks = &g_unk_717275ed88;
        ctx.callbacks.words[0] = callbacks->words[0];
        ctx.callbacks.words[1] = callbacks->words[1];
        ctx.callbacks.words[2] = callbacks->words[2];
        ctx.callbacks.words[3] = callbacks->words[3];
        ctx.callbacks.words[4] = callbacks->words[4];
        ctx.callbacks.words[5] = callbacks->words[5];
        UnkReadEntry* end = &target.entries[target.count];
        UnkReflObject* obj = target.count ? static_cast<UnkReflObject*>(end[-1].field_0x0->field_0x10) : nullptr;
        fn_7100b09720(reinterpret_cast<UnkDisplayReader_ovb0108*>(&ctx), obj);
    }
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    for (u64 i = 0; i < stack.count; ++i) {
        fn_71000a1c60(&stack.entries[i].c);
        fn_71000a1c60(&stack.entries[i].b);
        fn_71000a1c60(&stack.entries[i].a);
    }
    return def;
}
