// Two identical destructors of a reflection class (vtable unk_7101a1b3a0) that free the array at
// 0xf0 (16-byte elements holding a string id first), then call the base destructor fn_710009cc04,
// as base::refl_StrIdVecDtor (the generated fn_7100828f90) does for an array of string ids:
// fn_7100099468, fn_7100099a84, and the deleting destructor fn_7100099550, which then frees the
// object (fn_7100083c00). Also the deleting destructor of another class (vtable
// unk_7101a1d8b8) with an array at 0x10 of 16-byte elements holding a string id second, and
// nothing else to destroy: fn_710016a7f8, and its complete destructor fn_710016a714. Also the Free
// of arrays of bare string ids, from CFilePathStrId's pool (fn_71001b15b4, fn_71000d076c) or the
// default one (fn_71000cdfb0), and Begin over such arrays, as base::refl_VtBegin (the generated
// fn_7100f75f8c) without the element vtable: the iterator syncs out of line, then the temporary's
// id is released (fn_71000d0f1c, CFilePathStrId's pool; fn_71000ce8b8, the default one).
#include "UnkClone.h"
#include "base/UnkCloneCalls.h"
#include "base/UnkVector.h"

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

void fn_7100099550(char* self) {
    Destroy(self);
    fn_7100083c00(self);
}

using UnkWordIdRecord = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 8, 0x10, UnkCloneStrId<UnkIdRecordOwner_0xf0>>;

void fn_710016a714(char* self) {
    *reinterpret_cast<char**>(self) = unk_7101a1d8b8 + 0x10;
    reinterpret_cast<base::UnkVector<UnkWordIdRecord>*>(self + 0x10)->Free();
}

void fn_710016a7f8(char* self) {
    *reinterpret_cast<char**>(self) = unk_7101a1d8b8 + 0x10;
    reinterpret_cast<base::UnkVector<UnkWordIdRecord>*>(self + 0x10)->Free();
    __builtin_operator_delete(self);
}

using UnkFilePathIdElem = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 0, 8, UnkCloneStrIdB<UnkIdRecordOwner_0xf0>>;
using UnkIdElem = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 0, 8, UnkCloneStrId<UnkIdRecordOwner_0xf0>>;

void fn_71001b15b4(base::UnkVector<UnkFilePathIdElem>* self) { self->Free(); }

void fn_71000d076c(base::UnkVector<UnkFilePathIdElem>* self) { self->Free(); }

void fn_71000cdfb0(base::UnkVector<UnkIdElem>* self) { self->Free(); }

template <class Elem, void (*Sync)(void*)>
__attribute__((always_inline)) static inline void Begin(const base::UnkVector<Elem>* v,
                                                        base::UnkCollectionIter<Elem>* out) {
    base::UnkCollectionIter<Elem> it(0, v->data);
    if (out != &it) {
        out->index = it.index;
        out->element = it.element;
        Sync(out);
    }
}

void fn_71000be6c0(void*);
void fn_71000bce50(void*);

void fn_71000d0f1c(const base::UnkVector<UnkFilePathIdElem>* v, base::UnkCollectionIter<UnkFilePathIdElem>* out) {
    Begin<UnkFilePathIdElem, fn_71000be6c0>(v, out);
}

void fn_71000ce8b8(const base::UnkVector<UnkIdElem>* v, base::UnkCollectionIter<UnkIdElem>* out) {
    Begin<UnkIdElem, fn_71000bce50>(v, out);
}
