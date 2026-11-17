// base::reflection's unit (tools/tu_map.py, unit 2610): the copy-assignment of a record of string ids,
// a nine-id record, a file-path list and a few flags (fn_7100fa579c), which does nothing when assigned to
// itself. Names and layouts are placeholders.
#include "base/UnkResPathVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkStrIdRecord_lfa4b64;

// Each part below is assigned through its own out-of-line copy-assignment.
struct UnkIdRecord_lfa579c {
    u8 raw[0x48];
    UnkIdRecord_lfa579c& operator=(const UnkIdRecord_lfa579c& other) {
        fn_7100fa4b64(reinterpret_cast<UnkStrIdRecord_lfa4b64*>(this),
                      reinterpret_cast<const UnkStrIdRecord_lfa4b64*>(&other));
        return *this;
    }
};

struct UnkTable_lfa579c {
    u8 raw[0x28];
    UnkTable_lfa579c& operator=(const UnkTable_lfa579c& other) {
        fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(this), reinterpret_cast<const UnkStruct_0x28*>(&other));
        return *this;
    }
};

struct UnkPathList_lfa579c {
    u8 raw[0x28];  // an UnkResPathVec (as a typed member, the two argument addresses come out swapped)
    UnkPathList_lfa579c& operator=(const UnkPathList_lfa579c& other) {
        fn_71000d0958(reinterpret_cast<UnkResPathVec*>(this), reinterpret_cast<const UnkResPathVec*>(&other));
        return *this;
    }
};

struct UnkRecord_lfa579c {
    u8 unk_0x0[0x10];
    CStrId field_0x10;
    CStrId field_0x18;
    CFilePathStrId field_0x20;
    CStrId field_0x28;
    UnkIdRecord_lfa579c field_0x30;  // nine string ids
    UnkTable_lfa579c field_0x78;
    UnkPathList_lfa579c field_0xa0;
    u32 field_0xc8;
    bool field_0xcc;
    bool field_0xcd;
};
static_assert(__builtin_offsetof(UnkRecord_lfa579c, field_0xa0) == 0xa0, "");
static_assert(__builtin_offsetof(UnkRecord_lfa579c, field_0xc8) == 0xc8, "");

UnkRecord_lfa579c* fn_7100fa579c(UnkRecord_lfa579c* self, const UnkRecord_lfa579c* other) {
    if (self != other) {
        self->field_0x10 = other->field_0x10;
        self->field_0x18 = other->field_0x18;
        self->field_0x30 = other->field_0x30;
        self->field_0x78 = other->field_0x78;
        self->field_0x28 = other->field_0x28;
        self->field_0xa0 = other->field_0xa0;
        self->field_0x20 = other->field_0x20;
        self->field_0xc8 = other->field_0xc8;
        self->field_0xcc = other->field_0xcc;
        self->field_0xcd = other->field_0xcd;
    }
    return self;
}
