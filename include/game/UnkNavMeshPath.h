#pragma once

// navmesh::SNavMeshPath, the large path object (the unit tools/tu_map.py puts fn_71002ad688 in):
// a flag at +0x8, a weak reference at +0x11978, the count of the 16-bit words at +0x11980 and the
// block of links at +0x12988, whose weak reference at its +0xd8 (+0x12a60) is the target the links
// are registered with. fn_71002ad688 (src/game/navmesh/SNavMeshPath/fn_71002ad688.cpp) retargets it; the copy
// assignments (fn_71002ae5f4, fn_71002ae6e8) call it with the target of another path's reference.
// Only the members those functions touch are spelled out; names and layout are placeholders.

#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkLinkList;

struct UnkNavMeshPathLinks {
    UnkLinkList* field_0x0;
    u8 pad_0x8[0x10];
    UnkLinkList* field_0x18;
    u8 pad_0x20[0xb8];
    UnkWeakHandle* field_0xd8;
};

struct UnkNavMeshPath {
    u8 pad_0x0[8];
    u8 field_0x8;
    u8 pad_0x9[0x11978 - 0x9];
    UnkWeakHandle* field_0x11978;
    u8 pad_0x11980[0x12988 - 0x11980];
    UnkNavMeshPathLinks field_0x12988;
};
static_assert(__builtin_offsetof(UnkNavMeshPath, field_0x12988) == 0x12988, "UnkNavMeshPath placeholder layout");

// Points the path at a new target: drops the links of the previous one, registers them with the
// lists of `target` (null for none) and takes its weak handle for the references at +0x12a60 and
// +0x11978. Defined in src/game/navmesh/SNavMeshPath/fn_71002ad688.cpp.
void fn_71002ad688(UnkNavMeshPath* self, UnkWeakTarget* target);

// The vtable of the path's points (the 0x120-byte records of up to 250 from +0xc, reached through
// the GOT): their constructors and destructor (src/game/navmesh/SNavMeshPath.cpp) and the copy of
// the point table (src/game/behaviortree/CEmmySearchingAction/fn_7100518ef4.cpp) store its address point, entry 2.
extern void* g_unk_7101a21120[];
