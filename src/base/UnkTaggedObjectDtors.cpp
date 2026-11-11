// Destructors of objects tagged for debugging, from units 116, 125, 127 to 134 and 140 of
// tools/tu_map.py (0x7100140bbc-0x7100149bec): each class starts with the tagged header of
// src/base/UnkTaggedObject137.cpp (vtable, flags, the tag 0x600d600d that becomes 0xdeaddead, a
// counted resource at +0x10 and a self pointer) and destroys its own members first. Classes with
// one base keep a sub-object at +0x20; classes with a second base at +0x20 keep it at +0x28. Each
// comes as the complete destructor and the deleting one (the game's operator delete), and the
// two-base ones also through the second base (the pointer is that base's). The vtables are
// g_unk_<addr> + 0x10 (the second base's further on). Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "game/UnkTU32.h"
#include "types.h"

struct UnkTaggedHeader {
    void* vtable;
    u32 flags;
    u32 tag;                       // 0xc, 0xdeaddead once destroyed
    UnkCountedResource* resource;  // 0x10
    void* self;                    // 0x18
};

static inline void ReleaseTagged(UnkTaggedHeader* self) {
    self->tag = 0xdeaddead;
    if (UnkCountedResource* r = self->resource) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
}

static inline char* At(void* self, u64 offset) { return static_cast<char*>(self) + offset; }
template <class T>
static inline T& Field(void* self, u64 offset) {
    return *reinterpret_cast<T*>(At(self, offset));
}
static inline UnkTaggedHeader* FromSecondBase(void** base2) {
    return reinterpret_cast<UnkTaggedHeader*>(reinterpret_cast<char*>(base2) - 0x20);
}

void fn_710020c63c(void* sub);     // a sub-object's reset (units 125, 128, 129, 130)
void fn_710020c4dc(void* sub);     // its destructor

// ---- one base, sub-object reset then destroyed (units 125, 128, 130) ----

template <char* Vtable>
static inline void DestroyResetSub(UnkTaggedHeader* self) {
    self->vtable = Vtable + 0x10;
    fn_710020c63c(At(self, 0x20));
    fn_710020c4dc(At(self, 0x20));
    ReleaseTagged(self);
}

extern char g_unk_7101a1c030[];
extern char g_unk_7101a1c1b8[];
extern char g_unk_7101a1c4a8[];
void fn_7100142c0c(UnkTaggedHeader* self) { DestroyResetSub<g_unk_7101a1c030>(self); }
void fn_7100142c7c(UnkTaggedHeader* self) {
    DestroyResetSub<g_unk_7101a1c030>(self);
    fn_7100083c00(self);
}
void fn_7100143bbc(UnkTaggedHeader* self) { DestroyResetSub<g_unk_7101a1c1b8>(self); }
void fn_7100143c2c(UnkTaggedHeader* self) {
    DestroyResetSub<g_unk_7101a1c1b8>(self);
    fn_7100083c00(self);
}
void fn_7100144c2c(UnkTaggedHeader* self) { DestroyResetSub<g_unk_7101a1c4a8>(self); }
void fn_7100144c9c(UnkTaggedHeader* self) {
    DestroyResetSub<g_unk_7101a1c4a8>(self);
    fn_7100083c00(self);
}

// ---- one base and a member destroyed before the sub-object (units 132, 140) ----

// Unit 132: the object the pointer at +0x128 holds.
extern char g_unk_7101a1c630[];
void fn_71001bbc88(void* object);
void fn_7100206a44(void* sub);

static inline void DestroyUnit132(UnkTaggedHeader* self) {
    char* sub = At(self, 0x20);
    asm("" : "+r"(sub));
    self->vtable = g_unk_7101a1c630 + 0x10;
    fn_71001bbc88(Field<void*>(self, 0x128));
    fn_7100206a44(sub);
    ReleaseTagged(self);
}

void fn_7100145710(UnkTaggedHeader* self) { DestroyUnit132(self); }
void fn_7100145780(UnkTaggedHeader* self) {
    DestroyUnit132(self);
    fn_7100083c00(self);
}

// Unit 140: the member at +0x138.
extern char g_unk_7101a1cbe8[];
void fn_71001faf68(void* sub);

static inline void DestroyUnit140(UnkTaggedHeader* self) {
    char* sub = At(self, 0x20);
    asm("" : "+r"(sub));
    self->vtable = g_unk_7101a1cbe8 + 0x10;
    fn_710002572c(At(self, 0x138));
    fn_71001faf68(sub);
    ReleaseTagged(self);
}

void fn_710014803c(UnkTaggedHeader* self) { DestroyUnit140(self); }
void fn_71001480ac(UnkTaggedHeader* self) {
    DestroyUnit140(self);
    fn_7100083c00(self);
}

// ---- two bases ----

// Unit 116 (second vtable + 0x1d0): the member at +0x1c0, then the sub-object.
extern char g_unk_7101a1be28[];
void fn_71001f4efc(void* sub);

static inline void DestroyUnit116(UnkTaggedHeader* self) {
    self->vtable = g_unk_7101a1be28 + 0x10;
    Field<void*>(self, 0x20) = g_unk_7101a1be28 + 0x1d0;
    fn_710002572c(At(self, 0x1c0));
    fn_71001f4efc(At(self, 0x28));
    ReleaseTagged(self);
}

void fn_7100140c60(UnkTaggedHeader* self) { DestroyUnit116(self); }
void fn_7100140cd4(void** base2) { DestroyUnit116(FromSecondBase(base2)); }
void fn_7100140d48(UnkTaggedHeader* self) {
    DestroyUnit116(self);
    fn_7100083c00(self);
}
void fn_7100140dc8(void** base2) {
    UnkTaggedHeader* self = FromSecondBase(base2);
    DestroyUnit116(self);
    fn_7100083c00(self);
}

// Unit 133 (second vtable + 0x1d0): the object the pointer at +0x130 holds, the member at +0x168,
// then the sub-object.
extern char g_unk_7101a1c7d8[];
void fn_71001c08dc(void* object);
void fn_7100207258(void* sub);

// The complete destructor computes the sub-object's address before the first call (Pin).
template <bool Pin>
static inline void DestroyUnit133(UnkTaggedHeader* self) {
    self->vtable = g_unk_7101a1c7d8 + 0x10;
    Field<void*>(self, 0x20) = g_unk_7101a1c7d8 + 0x1d0;
    char* sub = At(self, 0x28);
    if (Pin)
        asm("" : "+r"(sub));
    fn_71001c08dc(Field<void*>(self, 0x130));
    fn_710002572c(At(self, 0x168));
    fn_7100207258(Pin ? sub : At(self, 0x28));
    ReleaseTagged(self);
}

void fn_7100145c80(UnkTaggedHeader* self) { DestroyUnit133<true>(self); }
void fn_7100145d00(void** base2) { DestroyUnit133<false>(FromSecondBase(base2)); }
void fn_7100145d7c(UnkTaggedHeader* self) {
    DestroyUnit133<true>(self);
    fn_7100083c00(self);
}
void fn_7100145e08(void** base2) {
    UnkTaggedHeader* self = FromSecondBase(base2);
    DestroyUnit133<false>(self);
    fn_7100083c00(self);
}

// ---- a sub-object prepared (Pre), a member at MemberOffset destroyed, then the sub-object (Post);
// with a second base when SecondVtable (its vtable's offset from the first) is not 0 ----

template <char* Vtable, u32 SecondVtable, void (*Pre)(void*), u32 MemberOffset, void (*Post)(void*)>
struct UnkTaggedClass {
    static constexpr u64 kSub = SecondVtable ? 0x28 : 0x20;

    static inline void Destroy(UnkTaggedHeader* self) {
        self->vtable = Vtable + 0x10;
        if (SecondVtable)
            Field<void*>(self, 0x20) = Vtable + SecondVtable;
        Pre(At(self, kSub));
        fn_710002572c(At(self, MemberOffset));
        Post(At(self, kSub));
        ReleaseTagged(self);
    }
    static inline void Delete(UnkTaggedHeader* self) {
        Destroy(self);
        fn_7100083c00(self);
    }
};

// Unit 127.
extern char g_unk_7101a1c170[];
void fn_710020d334(void* sub);
void fn_710020d1bc(void* sub);
using UnkTagged127 = UnkTaggedClass<g_unk_7101a1c170, 0, fn_710020d334, 0x140, fn_710020d1bc>;
void fn_71001433bc(UnkTaggedHeader* self) { UnkTagged127::Destroy(self); }
void fn_7100143434(UnkTaggedHeader* self) { UnkTagged127::Delete(self); }

// Unit 129.
extern char g_unk_7101a1c2f8[];
using UnkTagged129 = UnkTaggedClass<g_unk_7101a1c2f8, 0x178, fn_710020c63c, 0x148, fn_710020c4dc>;
void fn_7100144244(UnkTaggedHeader* self) { UnkTagged129::Destroy(self); }
void fn_71001442c4(void** base2) { UnkTagged129::Destroy(FromSecondBase(base2)); }
void fn_7100144344(UnkTaggedHeader* self) { UnkTagged129::Delete(self); }
void fn_71001443d0(void** base2) { UnkTagged129::Delete(FromSecondBase(base2)); }

// Unit 131.
extern char g_unk_7101a1c5e8[];
void fn_7100205d98(void* sub);
void fn_7100205ca0(void* sub);
using UnkTagged131 = UnkTaggedClass<g_unk_7101a1c5e8, 0, fn_7100205d98, 0x138, fn_7100205ca0>;
void fn_710014528c(UnkTaggedHeader* self) { UnkTagged131::Destroy(self); }
void fn_7100145304(UnkTaggedHeader* self) { UnkTagged131::Delete(self); }

// Unit 134.
extern char g_unk_7101a1c9e0[];
void fn_71002084ac(void* sub);
void fn_7100208338(void* sub);
using UnkTagged134 = UnkTaggedClass<g_unk_7101a1c9e0, 0x1d0, fn_71002084ac, 0x168, fn_7100208338>;
void fn_7100146a3c(UnkTaggedHeader* self) { UnkTagged134::Destroy(self); }
void fn_7100146abc(void** base2) { UnkTagged134::Destroy(FromSecondBase(base2)); }
void fn_7100146b3c(UnkTaggedHeader* self) { UnkTagged134::Delete(self); }
void fn_7100146bc8(void** base2) { UnkTagged134::Delete(FromSecondBase(base2)); }
