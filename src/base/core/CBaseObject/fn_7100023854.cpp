// base::core::CBaseObject's unit (unit 29 of tools/tu_map.py): runs fn_71000e7b5c over each of the
// object's class's 0xb0-byte entries in its array at +0xe8, with the class and the object as the
// context. The callers (constructors that finish with it) name the object UnkNode. Names are
// placeholders.
#include "base/reflection/CClass.h"
#include "types.h"

using base::reflection::CClass;

struct UnkNode;

struct UnkBaseObjectView {
    void** vtable;

    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkBaseObjectView*)>(vtable[0])(this); }
};

struct UnkClassEntryContext {
    const CClass* cls;
    UnkNode* object;
};

struct UnkClassEntry {
    u8 data[0xb0];
};

void fn_71000e7b5c(UnkClassEntry* entry, UnkClassEntryContext* context);

void fn_7100023854(UnkNode* node) {
    auto* self = reinterpret_cast<UnkBaseObjectView*>(node);
    const CClass* cls = self->vf_0x0();
    UnkClassEntryContext context;
    context.cls = self->vf_0x0();
    context.object = node;
    const base::reflection::UnkReflArray& entries = cls->m_array_0xe8;
    if (entries.count != 0) {
        UnkClassEntry* entry = static_cast<UnkClassEntry*>(entries.data);
        for (UnkClassEntry* end = entry + entries.count; entry != end; ++entry)
            fn_71000e7b5c(entry, &context);
    }
}
