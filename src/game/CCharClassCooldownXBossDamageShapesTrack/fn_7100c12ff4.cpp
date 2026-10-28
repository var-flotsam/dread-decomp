// Copy constructors of records over a base of two words (vtable g_unk_7101a1a138): a string id
// and a vector of string ids, plus a second string id and a word in one kind. Each of the four
// classes (one vtable each) has two identical copies.
#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkStrIdVec;
using base::global::CStrId;

extern char g_unk_7101a1a138[];
extern char g_unk_7101aa2b80[];
extern char g_unk_7101aa7d00[];
extern char g_unk_7101aa3030[];
extern char g_unk_7101aa3440[];

struct UnkStrIdRecordBase {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkStrIdVecRecord : UnkStrIdRecordBase {
    CStrId field_0x18;
    UnkStrIdVec field_0x20;
};

struct UnkStrIdRecord : UnkStrIdVecRecord {
    CStrId field_0x48;
    u32 field_0x50;
};

template <char* Vtable>
__attribute__((always_inline)) static inline void CopyConstruct(UnkStrIdVecRecord* self,
                                                                const UnkStrIdVecRecord* other) {
    self->vtable = g_unk_7101a1a138 + 0x10;
    __builtin_memcpy(&self->field_0x8, &other->field_0x8, 0x10);
    self->vtable = Vtable + 0x10;
    new (&self->field_0x18) CStrId(other->field_0x18);
    new (&self->field_0x20) UnkStrIdVec(other->field_0x20);
}

template <char* Vtable>
__attribute__((always_inline)) static inline void CopyConstruct(UnkStrIdRecord* self, const UnkStrIdRecord* other) {
    CopyConstruct<Vtable>(static_cast<UnkStrIdVecRecord*>(self), other);
    new (&self->field_0x48) CStrId(other->field_0x48);
    self->field_0x50 = other->field_0x50;
}

void fn_7100c12ff4(UnkStrIdRecord* self, const UnkStrIdRecord* other) {
    CopyConstruct<g_unk_7101aa2b80>(self, other);
}

void fn_7100c13178(UnkStrIdRecord* self, const UnkStrIdRecord* other) {
    CopyConstruct<g_unk_7101aa2b80>(self, other);
}

void fn_7100d22488(UnkStrIdRecord* self, const UnkStrIdRecord* other) {
    CopyConstruct<g_unk_7101aa7d00>(self, other);
}

void fn_7100d2260c(UnkStrIdRecord* self, const UnkStrIdRecord* other) {
    CopyConstruct<g_unk_7101aa7d00>(self, other);
}

void fn_7100c1df6c(UnkStrIdVecRecord* self, const UnkStrIdVecRecord* other) {
    CopyConstruct<g_unk_7101aa3030>(self, other);
}

void fn_7100c1e0c4(UnkStrIdVecRecord* self, const UnkStrIdVecRecord* other) {
    CopyConstruct<g_unk_7101aa3030>(self, other);
}

void fn_7100c2791c(UnkStrIdVecRecord* self, const UnkStrIdVecRecord* other) {
    CopyConstruct<g_unk_7101aa3440>(self, other);
}

void fn_7100c27a74(UnkStrIdVecRecord* self, const UnkStrIdVecRecord* other) {
    CopyConstruct<g_unk_7101aa3440>(self, other);
}
