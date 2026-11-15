// sound::CMusicManager::CTunableMusicManager unit: the copy constructor of a growable array of 0x18-byte
// {file path id, 3-float vector} records. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "../../../../../src/game/UnkUnit2601/UnkUnit2601_VecCopy_b56.h"

struct UnkPathEntry_lfddf18 {
    base::global::CFilePathStrId id;
    UnkVec3f field_0x8;
};

using UnkPathEntryVec_lfddf18 = UnkCopyVec_b56<UnkPathEntry_lfddf18>;
void fn_7100456980(UnkPathEntryVec_lfddf18* self, u32 n);
template <>
inline void UnkPathEntryVec_lfddf18::Reserve(u32 n) {
    fn_7100456980(this, n);
}

void fn_7100fddf18(UnkPathEntryVec_lfddf18* self, const UnkPathEntryVec_lfddf18* other) { self->CopyFrom(*other); }
