// Destructors of a pair of a string id (default pool) at 0 and a file-path id at 8, which release
// both ids and reset them to the empty string: fn_710001cbd8, fn_710001cfb4 and fn_710008d8e8.

#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkIdPathPair_710001cbd8 {
    base::UnkStrIdMember id;
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
};

void fn_710001cbd8(UnkIdPathPair_710001cbd8* self) { self->~UnkIdPathPair_710001cbd8(); }

void fn_710001cfb4(UnkIdPathPair_710001cbd8* self) { self->~UnkIdPathPair_710001cbd8(); }

void fn_710008d8e8(UnkIdPathPair_710001cbd8* self) { self->~UnkIdPathPair_710001cbd8(); }
