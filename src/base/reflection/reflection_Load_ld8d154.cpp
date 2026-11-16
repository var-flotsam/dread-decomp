// Unit 2525 (base::reflection): resets the record, makes its three owned objects (two 11-bucket maps
// and a pair of them) and loads each from the path with its extension (".brev", ".bres", ".brem"),
// then marks the record loaded (fn_7100d8d154). Names and layouts are placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

// As src/base/reflection/reflection_Reset_ld8d45c.cpp and the loaders' units declare them.
struct UnkRecord_ld8d45c;
void fn_7100d8d45c(UnkRecord_ld8d45c* self);
struct UnkMapPair_ovb56;
void fn_7100d8d670(UnkMapPair_ovb56* self);
struct UnkEntry_0x30;
void fn_7100f9bf8c(void* self, const UnkEntry_0x30* path);
void fn_7100f947b0(void* self, const UnkEntry_0x30* path);
void fn_7100f89450(void*, const UnkEntry_0x30* path);

// As src/base/reflection/reflection_environment_ovb0056.cpp defines it: a chained map started with 11
// buckets.
struct UnkMap11_ovb56 : UnkChainedMap {
    __attribute__((always_inline)) UnkMap11_ovb56() { ResizeBuckets(11); }
};

// An owned object made of one such map (0x48 bytes).
struct UnkOwnedMap_ld8d154 {
    UnkMap11_ovb56 map;
};
static_assert(sizeof(UnkOwnedMap_ld8d154) == 0x48, "UnkOwnedMap_ld8d154 layout");

// The storage of a pair of such maps (0x90 bytes), constructed by fn_7100d8d670.
struct UnkOwnedPair_ld8d154 {
    u8 data[0x90];
};

struct UnkRecord_ld8d154 {
    UnkOwnedMap_ld8d154* a;
    UnkOwnedMap_ld8d154* b;
    UnkOwnedPair_ld8d154* c;
    u8 pad_0x18[0x4a9];
    bool loaded;  // 0x4c1
};
static_assert(__builtin_offsetof(UnkRecord_ld8d154, loaded) == 0x4c1, "UnkRecord_ld8d154 layout");

// A string as the loaders take it.
inline const UnkEntry_0x30* Entry_ld8d154(const CRntString& str) {
    return reinterpret_cast<const UnkEntry_0x30*>(&str);
}

void fn_7100d8d154(UnkRecord_ld8d154* self, const CRntString* path) {
    fn_7100d8d45c(reinterpret_cast<UnkRecord_ld8d45c*>(self));
    self->a = new (fn_71000839a0(sizeof(UnkOwnedMap_ld8d154), 5)) UnkOwnedMap_ld8d154();
    self->b = new (fn_71000839a0(sizeof(UnkOwnedMap_ld8d154), 5)) UnkOwnedMap_ld8d154();
    UnkOwnedPair_ld8d154* c = new (fn_71000839a0(sizeof(UnkOwnedPair_ld8d154), 5)) UnkOwnedPair_ld8d154();
    fn_7100d8d670(reinterpret_cast<UnkMapPair_ovb56*>(c));
    self->c = c;
    fn_7100f9bf8c(self->a, Entry_ld8d154(fn_710007ef90(path, ".brev")));
    fn_7100f947b0(self->b, Entry_ld8d154(fn_710007ef90(path, ".bres")));
    fn_7100f89450(self->c, Entry_ld8d154(fn_710007ef90(path, ".brem")));
    self->loaded = true;
}
