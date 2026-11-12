// base::reflection::CFlagsetType's unit (tools/tu_map.py unit 78): reads a flag set's bits from a
// file (4 or 8 bytes, as the type's size says) and sets the field with them, typed as unsigned or as
// the 8-byte integer type. Names are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntFile.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntFile;
using base::reflection::CType;

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;

struct UnkFlagsetType_b0068 {
    u8 pad_0x0[0x18];
    u64 size;  // 0x18
};

// A value's type and where it is.
struct UnkFlagsetValue_b0068 {
    const CType* type;
    void* value;
};

static inline bool Set(UnkReflField* field, const UnkReflFieldObject* obj, const CType* type, void* bits) {
    UnkFlagsetValue_b0068 value = {type, bits};
    return fn_71000e7a80(field, obj, reinterpret_cast<UnkReflValueArg*>(&value));
}

bool fn_710009a430(UnkFlagsetType_b0068* self, const UnkReflFieldObject* obj, UnkReflField* field, CRntFile* file) {
    switch (self->size) {
    case 4: {
        u32 bits;
        fn_710007c714(file, &bits, 4, 1);
        return Set(field, obj, fn_7100005178(), &bits);
    }
    case 8: {
        u64 bits;
        fn_710007c714(file, &bits, 8, 1);
        return Set(field, obj, fn_7100010e58(), &bits);
    }
    default:
        return false;
    }
}
