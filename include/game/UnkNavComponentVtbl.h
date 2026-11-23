#pragma once

#include "types.h"

struct UnkNavQuery;
struct UnkNavComponent;
struct UnkNavMeshHits;

struct UnkNavComponentVtbl {
    u8 pad_0x0[0x548];
    void (*vf_0x548)(UnkNavComponent* self, u16 cell, UnkNavQuery* query, UnkNavMeshHits* hits);
    void (*vf_0x550)(UnkNavComponent* self, u16 cell, UnkNavQuery* query, UnkNavMeshHits* hits);
};
