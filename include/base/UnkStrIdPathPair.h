#pragma once

// A pair of a string id (default pool) at 0 and a file-path id at 8, released by its destructor.
// fn_710001cbd8, fn_710001cfb4 and fn_710008d8e8 destroy one (src/game_s1/fn_710001cbd8.cpp).

#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkStrIdPathPair {
    base::UnkStrIdMember id;
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
};

void fn_710008d8e8(UnkStrIdPathPair* self);

// The pair with the resource the path resolves to (unit 32 of tools/tu_map.py, like the pair's
// own functions).
struct UnkStrIdPathResource {
    UnkStrIdPathPair ids;
    UnkCountedResource* resource;
};
static_assert(sizeof(UnkStrIdPathResource) == 0x18, "UnkStrIdPathResource placeholder layout");

// Assigns both ids from `other` (a string id and a file-path id: only the pair's two words are
// read) and, when both are set, resolves the resource again (fn_710002038c). Not decompiled yet.
void fn_71000267f8(UnkStrIdPathResource* self, const UnkStrIdPathPair& other);

// Releases the ids and drops the resource. Not decompiled yet.
void fn_7100027044(UnkStrIdPathResource* self);
