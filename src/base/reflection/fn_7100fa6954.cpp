// base::reflection's unit (tools/tu_map.py, unit 2610): sets an object's two named ids (+0x10, +0x18),
// builds their "subarea_..." file-path id (fn_7100fa6ad0) into +0x20 and resets the value at +0xc8 and
// the two flags after it. Names and layouts are placeholders.
#include "base/UnkPathId_lfa6ad0.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkNamedId_lfa6ad0;

UnkPathId_lfa6ad0 fn_7100fa6ad0(void* self, const UnkNamedId_lfa6ad0* area, const UnkNamedId_lfa6ad0* first,
                                const UnkNamedId_lfa6ad0* second);

// Takes `other`'s id as CStrId's move does (UnkStrIdBase::MoveFrom), on the file-path pool:
// released in two steps, with no empty id stored in between.
inline void TakePath_lfa6954(CFilePathStrId& path, CFilePathStrId& other) {
    base::global::UnkStringPool* pool = g_unk_7101d47200;
    if (pool) {
        if (path.data && path.data != &pool->empty) {
            --path.data->ref_count;
            if (path.data->ref_count == 0) {
                fn_71000803a4(pool, path.data);
                pool = g_unk_7101d47200;
            }
        }
    }
    path.data = other.data;
    other.data = &pool->empty;
}

struct UnkSubAreaRef_lfa6954 {
    u8 pad_0x0[0x10];
    CStrId first;          // 0x10
    CStrId second;         // 0x18
    CFilePathStrId path;   // 0x20
    u8 pad_0x28[0xa0];
    f32 field_0xc8;
    bool field_0xcc;
    bool field_0xcd;
};

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7100fa6954(UnkSubAreaRef_lfa6954* self, const CStrId* area, const CStrId* first, const CStrId* second) {
    self->first = *first;
    self->second = *second;
    UnkPathId_lfa6ad0 built = fn_7100fa6ad0(self, reinterpret_cast<const UnkNamedId_lfa6ad0*>(area),
                                            reinterpret_cast<const UnkNamedId_lfa6ad0*>(first),
                                            reinterpret_cast<const UnkNamedId_lfa6ad0*>(second));
    // The built id is a file-path id: taken, then destroyed.
    CFilePathStrId& path = static_cast<CFilePathStrId&>(built.id);
    TakePath_lfa6954(self->path, path);
    path.~CFilePathStrId();
    self->field_0xc8 = -1.0f;
    self->field_0xcc = false;
    self->field_0xcd = false;
}
