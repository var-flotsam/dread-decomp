// Appends two type descriptors to the array at +8, as the generated fn_7100ef0840
// (base::refl_PushPair) does: the getter's result, then a descriptor held in data. CStrId's type
// and g_unk_7101d4ec50: fn_7100263ea8; the unsigned integer type and g_unk_7108b6da30:
// fn_710011e050.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/UnkReflAttributes.h"

extern base::reflection::CType g_unk_7101d4ec50;
extern base::reflection::CType g_unk_7108b6da30;

struct UnkIdTypePair;
template <>
struct base::UnkPushPairTraits<UnkIdTypePair> {
    static void* first() { return const_cast<reflection::CType*>(fn_7100009658()); }
    static void* second() { return &g_unk_7101d4ec50; }
};

struct UnkU32TypePair;
template <>
struct base::UnkPushPairTraits<UnkU32TypePair> {
    static void* first() { return const_cast<reflection::CType*>(fn_7100005178()); }
    static void* second() { return &g_unk_7108b6da30; }
};

__attribute__((flatten)) void fn_7100263ea8(char* self) { base::refl_PushPair<UnkIdTypePair>(self); }

__attribute__((flatten)) void fn_710011e050(char* self) { base::refl_PushPair<UnkU32TypePair>(self); }
