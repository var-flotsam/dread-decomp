// Two identical destructors of a reflection class (vtable unk_7101a1b3a0) that free the array at
// 0xf0 (16-byte elements holding a string id first), then call the base destructor fn_710009cc04,
// as base::refl_StrIdVecDtor (the generated fn_7100828f90) does for an array of string ids:
// fn_7100099468, fn_7100099a84. Also the deleting destructor of another class (vtable
// unk_7101a1d8b8) with an array at 0x10 of 16-byte elements holding a string id second, and
// nothing else to destroy: fn_710016a7f8.
#include "UnkClone.h"
#include "base/UnkCloneCalls.h"

extern char unk_7101a1b3a0[];
extern char unk_7101a1d8b8[];
void fn_710009cc04(char*);

struct UnkIdRecordOwner_0xf0;
using UnkIdRecord = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 0, 0x10, UnkCloneStrId<UnkIdRecordOwner_0xf0>>;

static inline void Destroy(char* self) {
    *reinterpret_cast<char**>(self) = unk_7101a1b3a0 + 0x10;
    reinterpret_cast<base::UnkVector<UnkIdRecord>*>(self + 0xf0)->Free();
    fn_710009cc04(self);
}

void fn_7100099468(char* self) { Destroy(self); }

void fn_7100099a84(char* self) { Destroy(self); }

using UnkWordIdRecord = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 8, 0x10, UnkCloneStrId<UnkIdRecordOwner_0xf0>>;

void fn_710016a7f8(char* self) {
    *reinterpret_cast<char**>(self) = unk_7101a1d8b8 + 0x10;
    reinterpret_cast<base::UnkVector<UnkWordIdRecord>*>(self + 0x10)->Free();
    __builtin_operator_delete(self);
}
