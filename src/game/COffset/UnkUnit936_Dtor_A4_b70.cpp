// Unit 936's (tools/tu_map.py; hint COffset) destructor of a class with vtable 0x7101a561c8: two arrays of
// owned objects (game_s2/UnkOwnedList.h's UnkOwnedPtr) at +0x798 and +0x770, the members at +0x638
// (fn_71006241c4) and +0x510 (fn_710060a5c8), then the base's destructor fn_7100416e80. Names are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

extern void* g_unk_7101a561c8[];
char* fn_71006241c4(char* p0);  // as src/lifted/ defines it
void fn_710060a5c8(char* self);  // a base::refl_Dtor instance in src/clones/clones_02.cpp
struct UnkStruct_7100416e80;

struct UnkUnit936Dtor_A4_b70;

template <>
struct base::UnkDtorTraits<UnkUnit936Dtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a561c8); }
    static void base(char* self) { fn_7100416e80(reinterpret_cast<UnkStruct_7100416e80*>(self)); }
};

template <u32 Offset>
struct UnkUnit936Owned_A4_b70 {
    static void Release(char* self) { reinterpret_cast<base::UnkVector<UnkOwnedPtr>*>(self + Offset)->Free(); }
};

struct UnkUnit936Members_A4_b70 {
    static void Release(char* self) {
        fn_71006241c4(self + 0x638);
        fn_710060a5c8(self + 0x510);
    }
};

__attribute__((flatten)) void fn_7100614f0c(char* self) {
    base::refl_Dtor<UnkUnit936Dtor_A4_b70, UnkUnit936Owned_A4_b70<0x798>, UnkUnit936Owned_A4_b70<0x770>,
                    UnkUnit936Members_A4_b70>(self);
}
