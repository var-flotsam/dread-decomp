// Reads a reflected object from a file held in memory (tools/tu_map.py unit 91, fn_71000edda0 to
// fn_71000edfc0): the file is copied, the object's value is set up as a read target and the
// parser fills it through the caller's callbacks (the reader types are in
// base/reflection/UnkReflObjectRead.h). Names and layouts are placeholders.
// Holds fn_71000edda0.
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

// The file being read and two words the reader keeps (+0x38 is zero after a good read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

// Copy-constructs a file from another one (its contents copied into a block of its own).
void fn_710007beac(base::global::CRntFile* self, const base::global::CRntFile* other, u32 flags,
                   base::UnkAllocator* allocator);

// An object and its type.
struct UnkTypedObject {
    const CType* type;
    void* object;
};
// Builds a reflected value for an object (by reference when by_ref is 1).
void fn_71000a1828(UnkReflValue* out, const UnkTypedObject* object, u32 by_ref, u32 kind);

// Reads `object` from a copy of `source` (from its start, the reader's +0x3c set to `flags`) into
// a target set up with `mode`, through `callbacks`. Returns whether the parse succeeded.
bool fn_71000edda0(const base::global::CRntFile* source, const UnkTypedObject* object, u32 mode, u32 flags,
                   const UnkParseCallbacks* callbacks) {
    UnkFileReader reader;
    fn_710007beac(&reader.file, source, 0x16, nullptr);
    reader.field_0x3c = flags;
    fn_710007c00c(&reader.file, 0, 0);
    reader.field_0x38 = 0;
    UnkReadTarget target;
    {
        UnkReflValue value;
        fn_71000a1828(&value, object, 1, 2);
        fn_71000f0568(&target, &value, mode);
        fn_71000a1c60(&value);
    }
    UnkParseContext ctx(*callbacks, &reader, &target);
    bool ok = fn_710000f850(&ctx, object->type, nullptr) != 0;
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
    return ok;
}
