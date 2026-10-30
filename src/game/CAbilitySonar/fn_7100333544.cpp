// Destructors of a pair of a string id (default pool) at 0 and a file-path id at 8 (as in
// fn_710008d8e8, src/game_s1/fn_710001cbd8.cpp): fn_7100333544; the same pair at 0x18 in
// fn_710034e0b8.

#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkIdPathPair_7100333544 {
    base::UnkStrIdMember id;
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
};

void fn_7100333544(UnkIdPathPair_7100333544* self) { self->~UnkIdPathPair_7100333544(); }

struct UnkIdPathPairAt18_710034e0b8 {
    u8 field_0x0[0x18];
    UnkIdPathPair_7100333544 pair;
};

void fn_710034e0b8(UnkIdPathPairAt18_710034e0b8* self) { self->~UnkIdPathPairAt18_710034e0b8(); }

// The pair the other way round (the path id first), at 8 in another unit.
struct UnkPathIdPairAt8_7100d8ef0c {
    u8 field_0x0[0x8];
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    base::UnkStrIdMember id;
};

void fn_7100d8ef0c(UnkPathIdPairAt8_7100d8ef0c* self) { self->~UnkPathIdPairAt8_7100d8ef0c(); }
