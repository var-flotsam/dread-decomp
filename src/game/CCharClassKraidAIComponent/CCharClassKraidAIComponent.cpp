// Constructor of a component in the CCharClassKraidAIComponent unit (the shape of fn_710044e048):
// the boss base constructor, the vtable, then eleven members constructed in place (the launch
// patterns of src/game/CAcidBlobsLaunchPattern among them) and three flags. The fields are
// addressed by offset; names are placeholders.
#include "game/UnkBossAIComponent.h"
#include "types.h"

extern void* g_unk_7101a542f0[];

struct UnkKraidAIComponent : UnkBossAIComponent {};

struct UnkLaunchPattern_71005d69b8;
struct UnkLaunchPattern_71005dfcc4;
void fn_71008c2da8(void* member);
void fn_71005eaafc(void* member);
void fn_71005dc924(void* member);
void fn_71005e56bc(void* member);
void fn_71005d69b8(UnkLaunchPattern_71005d69b8* self);
void fn_71005e662c(void* member);
void fn_71005f0cd0(void* member);
void fn_71005dfcc4(UnkLaunchPattern_71005dfcc4* self);
void fn_71005e718c(void* member);

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

template <class T>
static inline T* Member(void* base, u64 offset) {
    return reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_71005fa760(UnkKraidAIComponent* self) {
    fn_710041a198(self);
    At<void**>(self, 0) = g_unk_7101a542f0 + 2;
    fn_71008c2da8(Member<char>(self, 0x550));
    fn_71008c2da8(Member<char>(self, 0x5c0));
    fn_71005eaafc(Member<char>(self, 0x630));
    fn_71005dc924(Member<char>(self, 0x770));
    fn_71005e56bc(Member<char>(self, 0x860));
    fn_71005d69b8(Member<UnkLaunchPattern_71005d69b8>(self, 0x950));
    fn_71005e662c(Member<char>(self, 0xa88));
    fn_71005f0cd0(Member<char>(self, 0xb90));
    fn_71005dfcc4(Member<UnkLaunchPattern_71005dfcc4>(self, 0xc98));
    fn_71005e718c(Member<char>(self, 0xdb8));
    At<u8>(self, 0x409) = 0;
    At<u8>(self, 0x4fc) = 1;
    At<u8>(self, 0x4fd) = 1;
    At<u8>(self, 0x4dc) = 1;
}
