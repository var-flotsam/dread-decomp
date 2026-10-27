// The translation unit of base::res::CRes (its reflection is generated): the destructor of the
// record a resource type is registered with (UnkResDesc, 0x90 bytes; src/base/UnkResRegistry.cpp
// builds and copies it, src/game_s7/fn_71001b1dd0.cpp fills it in). Only the members it releases
// are typed here: the name, two file-path ids, the list of file-path ids and the last file-path id.

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

typedef base::UnkStrIdMemberOf<g_unk_7101d47200> UnkResDescPathId;

// The list of file-path ids (an UnkVector, flags 9): destroying it frees the array.
struct UnkResDescPathList : base::UnkVector<UnkResDescPathId> {
    ~UnkResDescPathList() { Free(); }
};

struct UnkResDesc {
    u8 unk_0x0[0x10];               // the type and its registry
    base::UnkStrIdMember name;      // 0x10
    UnkResDescPathId field_0x18;
    UnkResDescPathId field_0x20;
    u8 unk_0x28[0x38];              // two callbacks, the factory, the type, 0x58
    UnkResDescPathList paths;       // 0x60
    UnkResDescPathId field_0x88;

    ~UnkResDesc() asm("fn_71000e8dd0");
};
static_assert(sizeof(UnkResDesc) == 0x90, "UnkResDesc layout");

// Clang 9, the matching compiler, ignores the asm label on the destructor and emits its mangled
// symbols, so this alias gives the definition the address's name; a newer Clang (the port's) names
// the definition by its label (as src/base/UnkResRegistry.cpp does for the record's constructor).
#if defined(__clang__) && __clang_major__ <= 9
void fn_71000e8dd0(UnkResDesc*) __attribute__((alias("_ZN10UnkResDescD2Ev")));
#endif

UnkResDesc::~UnkResDesc() {}
