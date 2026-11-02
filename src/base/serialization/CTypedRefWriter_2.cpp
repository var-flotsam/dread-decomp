// From base::serialization::CTypedRefWriter's unit (tools/tu_map.py unit 94; the writer and its
// contexts are laid out in src/base/serialization/CTypedRefWriter.cpp, which keeps its types to
// itself, so this file reads them through its own views): two identical steps of the writer (two
// overrides, one per context kind) that resolve the innermost context's value (fn_71000f0648) when
// it has none yet and its reader is at the context's position; both report success. Names are
// placeholders.
#include "types.h"

struct UnkTypedRefWriter;
void fn_71000f0648(UnkTypedRefWriter* self);

struct UnkTypedRefReaderPosView {
    u8 pad_0x0[0x10];
    u32 position;
};

struct UnkTypedRefContextPosView {
    u8 pad_0x0[0x10];
    const void* value_type;  // +0x10: the context's value is empty while null
    u8 pad_0x18[0x28];
    UnkTypedRefReaderPosView* reader;  // +0x40
    u8 pad_0x48[0x8];
    u32 position;  // +0x50
};

struct UnkTypedRefEntryPosView {
    UnkTypedRefContextPosView* context;
    u8 storage[0xd0];
};
static_assert(sizeof(UnkTypedRefEntryPosView) == 0xd8, "UnkTypedRefEntryPosView layout");

// The context stack (+0x4): a count and raw storage for the entries, 4-byte aligned.
struct UnkTypedRefStackPosView {
    u32 count;
    u8 storage[sizeof(UnkTypedRefEntryPosView)];

    UnkTypedRefEntryPosView& Top() {
        return *reinterpret_cast<UnkTypedRefEntryPosView*>(storage + (count - 1) * sizeof(UnkTypedRefEntryPosView));
    }
};

struct UnkTypedRefWriterPosView {
    u32 field_0x0;
    UnkTypedRefStackPosView stack;  // +0x4
};

static inline bool ResolveIfAtReader(UnkTypedRefWriterPosView* self) {
    UnkTypedRefContextPosView* context = self->stack.Top().context;
    if (!context->value_type && context->reader && context->reader->position == context->position)
        fn_71000f0648(reinterpret_cast<UnkTypedRefWriter*>(self));
    return true;
}

bool fn_71000f05ec(UnkTypedRefWriterPosView* self) {
    return ResolveIfAtReader(self);
}

bool fn_71000f2a80(UnkTypedRefWriterPosView* self) {
    return ResolveIfAtReader(self);
}
