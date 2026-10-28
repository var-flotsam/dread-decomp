// Constructs a record holding an empty file path id (base::global::CFilePathStrId's
// default-constructed state: the file-path pool's empty string, creating the pool first if
// needed) and three counters (0, -1, 1).
#include "base/global/CFilePathStrId.h"

struct UnkPathRecord {
    base::global::UnkStringData* path;  // a CFilePathStrId
    u32 field_0x8;
    s32 field_0xc;
    u32 field_0x10;
};

void fn_7100456ac0(UnkPathRecord* self) {
    self->path = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->path = &g_unk_7101d47200->empty;
    self->field_0x8 = 0;
    self->field_0xc = -1;
    self->field_0x10 = 1;
}
