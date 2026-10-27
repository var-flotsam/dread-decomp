// base::animation::CBlendSpaceResource's unit (translation unit 14 of data/tu_map.csv), whose
// helpers also serve the reflected-object file readers (src/game/UnkObjectFileRead.cpp and its
// relatives): fn_710000ccd8 checks a file's version header against the registered one, and
// fn_7100010c60 parses a value of a given type into a scratch target and drops it (skipping it;
// the reader types are in base/reflection/UnkReflObjectRead.h). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

// The file read into memory and two words the opener sets (as in src/game/UnkObjectFileRead.cpp);
// +0x38 is the version check's result: 0 the registered version, 1 an older one, 2 a newer one.
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

// The registry of file versions, by the 64-bit id a file starts with.
struct UnkVersionRegistry;
struct UnkVersionEntry;
extern UnkVersionRegistry* g_unk_7101d4c8d0;
UnkVersionEntry* fn_71000a5118(UnkVersionRegistry* self, const u64* id);
// The entry's version (major, minor); false when it has none.
bool fn_71000e6ce0(UnkVersionEntry* self, u16* major, u16* minor);

// Reads the file's id and, when it is registered, its version; sets field_0x38 from the comparison
// (a file without an id is taken as current). True when the version is the registered one.
// NON_MATCHING(minor): on the early exit the store of field_0x38 and the setting of the return value
// are scheduled the other way round.
bool fn_710000ccd8(UnkFileReader* self) {
    u64 id = (u64)-1;
    fn_710007c714(&self->file, &id, 8, 1);
    if (id == (u64)-1) {
        self->field_0x38 = 0;
        return true;
    }
    UnkVersionEntry* entry = fn_71000a5118(g_unk_7101d4c8d0, &id);
    u16 major = 0;
    u16 minor = 0;
    if (entry && fn_71000e6ce0(entry, &major, &minor)) {
        u16 file_major = 0;
        u16 file_minor = 0;
        fn_710007c714(&self->file, &file_major, 2, 1);
        fn_710007c714(&self->file, &file_minor, 2, 1);
        u32 status;
        if (file_major == major && file_minor == minor) {
            status = 0;
        } else if (file_major > major) {
            status = 2;
        } else {
            status = 1;
            if (file_major >= major && file_minor >= minor)
                status = 2;
        }
        self->field_0x38 = status;
    }
    return self->field_0x38 == 0;
}

// The void type (src/base/reflection/UnkReflFunction.cpp).
extern base::reflection::CType g_unk_7101d5af48;

void fn_71000a2014(UnkReflValue* self);

// Parses a value of `type` (nothing for void) with the context's callbacks and file into a target of
// its own, and destroys what was read.
void fn_7100010c60(const UnkParseContext* ctx, const CType* type) {
    if (type == &g_unk_7101d5af48)
        return;
    UnkReflValue value;
    value.field_0x0 = nullptr;
    value.type = nullptr;
    value.kind = 2;
    value.field_0x14 = 1;
    fn_71000a2014(&value);
    UnkReadTarget target;
    fn_71000f0568(&target, &value, 5);
    target.field_0x5468 = 0;
    UnkParseContext sub(*ctx, &target);
    fn_710000f850(&sub, type, nullptr);
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_71000a1c60(&value);
}
