#pragma once

// A pair of a string id (default pool) at 0 and a file-path id at 8, released by its destructor.
// fn_710001cbd8, fn_710001cfb4 and fn_710008d8e8 destroy one (src/game_s1/fn_710001cbd8.cpp).

#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkStrIdPathPair {
    base::UnkStrIdMember id;
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
};

void fn_710008d8e8(UnkStrIdPathPair* self);
