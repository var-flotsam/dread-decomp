// Unit 2640 of tools/tu_map.py: move constructors of reflected iterator values
// (base::refl_IterValueMove, base/UnkCollectionIter.h) whose elements hold growable arrays copied by
// unit 2601's copy constructors (src/game/UnkUnit2601/UnkUnit2601_VecCopy_b56.h), which have no move
// constructor. fn_7100ffcdf8: a u32 index and the 0x50-byte record (as
// src/base/UnkUnit2640/UnkRecordIterAssign.cpp's); fn_7100fff810: a string id key and a {records,
// string id} element. Also the 0x50-byte record's implicit copy constructor. Names and layouts are
// placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "../../game/UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "types.h"

struct UnkRecordsIdElem_b56 {
    UnkRecord50VecMember_b56 records;
    base::global::CStrId id;
};

void fn_7100ffcdf8(void* dst, const void* src) { base::refl_IterValueMove<UnkRecord50b_b56, u32>(dst, src); }

void fn_7100fff810(void* dst, const void* src) {
    base::refl_IterValueMove<UnkRecordsIdElem_b56, base::UnkStrIdMember>(dst, src);
}

void fn_7100ffc4f4(UnkRecord50b_b56* self, const UnkRecord50b_b56* other) { new (self) UnkRecord50b_b56(*other); }
