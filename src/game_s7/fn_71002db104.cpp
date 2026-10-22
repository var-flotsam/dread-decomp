// Copies of fn_7100c8f7cc (src/game_aca57c/fn_7100b2b9d4.cpp), which the binary
// repeats at other addresses: fn_71002db104, fn_710058a2b8.
// Releases two string ids of an object, in the given order. Three objects.
#include "base/UnkDestructors.h"
#include "types.h"

template <u32 First, u32 Second>
__attribute__((always_inline)) static inline void ReleaseIds(char* self) {
    reinterpret_cast<base::UnkStrIdMember*>(self + First)->~UnkStrIdMemberOf();
    reinterpret_cast<base::UnkStrIdMember*>(self + Second)->~UnkStrIdMemberOf();
}

void fn_71002db104(char* self) { ReleaseIds<0x8, 0x0>(self); }

void fn_710058a2b8(char* self) { ReleaseIds<0x8, 0x0>(self); }

