#pragma once

#include "base/global/CRntVector.h"
#include "game/UnkNavMeshHit.h"
#include "types.h"

// A list of hits with room for a fixed number (not known here) after the count.
// From src/game/navmesh/CNavMesh.cpp (the same definition).
struct UnkNavMeshHits {
    u32 count;

    UnkNavMeshHit* items() { return reinterpret_cast<UnkNavMeshHit*>(&count + 1); }
    void Clear() {
        for (u64 i = 0; i < count; ++i)
            items()[i].~UnkNavMeshHit();
        count = 0;
    }
    // The hit at `i`, constructing the ones up to it first when the list is shorter.
    UnkNavMeshHit& At(u64 i) {
        if (i >= count) {
            for (u32 j = count; j < i + 1; ++j)
                new (&items()[j]) UnkNavMeshHit;
            count = i + 1;
        }
        return items()[i];
    }
    void Push(const UnkNavMeshHit& hit) {
        new (&items()[count]) UnkNavMeshHit(hit);
        ++count;
    }
};
