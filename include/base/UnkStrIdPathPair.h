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
// own functions): fn_71000267f8 assigns one, copying both ids and resolving the resource again
// (fn_710002038c) when both are set; fn_7100027044 releases the ids and drops the resource.
struct UnkStrIdPathResource {
    UnkStrIdPathPair ids;
    UnkCountedResource* resource;
};
static_assert(sizeof(UnkStrIdPathResource) == 0x18, "UnkStrIdPathResource placeholder layout");
void fn_71000267f8(UnkStrIdPathResource* self, const UnkStrIdPathResource& other);
void fn_7100027044(UnkStrIdPathResource* self);
