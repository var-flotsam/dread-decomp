// Unit 2577 (CSubAreaManager::CTunableSubAreaManager): frees the six id lists of a set (two string-id lists and
// four file-path-id lists), last to first. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkPooledId.h"
#include "types.h"

using UnkId = UnkPooledId<&g_unk_7101d47108>;
using UnkPathId = UnkPooledId<&g_unk_7101d47200>;

struct UnkIdSet {
    base::global::CRntVector<UnkId> field_0x0;
    base::global::CRntVector<UnkId> field_0x28;
    base::global::CRntVector<UnkPathId> field_0x50;
    base::global::CRntVector<UnkPathId> field_0x78;
    base::global::CRntVector<UnkPathId> field_0xa0;
    base::global::CRntVector<UnkPathId> field_0xc8;
};

void fn_7100ef2390(UnkIdSet* self) {
    self->field_0xc8.Free();
    self->field_0xa0.Free();
    self->field_0x78.Free();
    self->field_0x50.Free();
    self->field_0x28.Free();
    self->field_0x0.Free();
}
