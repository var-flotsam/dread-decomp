// Releases two string ids of an object, in the given order. Three objects.
#include "base/UnkDestructors.h"
#include "types.h"

template <u32 First, u32 Second>
__attribute__((always_inline)) static inline void ReleaseIds(char* self) {
    reinterpret_cast<base::UnkStrIdMember*>(self + First)->~UnkStrIdMemberOf();
    reinterpret_cast<base::UnkStrIdMember*>(self + Second)->~UnkStrIdMemberOf();
}

void fn_7100b2b9d4(char* self) { ReleaseIds<0x780, 0x788>(self); }
void fn_7100c8f7cc(char* self) { ReleaseIds<0x8, 0x0>(self); }
void fn_7100d9bf2c(char* self) { ReleaseIds<0x10, 0x8>(self); }
