// The translation unit of base::res::CRes (its reflection is generated): the destructor of the
// record a resource type is registered with (UnkResDesc, base/UnkResDesc.h). Its members release
// themselves: the last file-path id, the list of file-path ids, the two file-path ids and the name.

#include "base/UnkResDesc.h"
#include "types.h"

// Clang 9, the matching compiler, ignores the asm label on the destructor and emits its mangled
// symbols, so this alias gives the definition the address's name; a newer Clang (the port's) names
// the definition by its label (as src/base/UnkResRegistry.cpp does for the record's constructor).
#if defined(__clang__) && __clang_major__ <= 9
void fn_71000e8dd0(UnkResDesc*) __attribute__((alias("_ZN10UnkResDescD2Ev")));
#endif

UnkResDesc::~UnkResDesc() {}
