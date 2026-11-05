// Reflected invokers of the CAnimationNavMeshItemComponent (unit 1473, fn_71009b84d0) and CLogicPathNavMeshItemComponent
// (unit 1480, fn_71009bd120) units: take a source descriptor (by pointer) and an argument, look the owner's source up by
// the descriptor's id (fn_71009c0e24) and, when it is the unit's stage class (CAnimationNavMeshItemStage /
// CLogicPathNavMeshItemStage), pass it the argument (fn_71009c5ab8, as fn_71009c28b8 in
// src/game/CNavMeshItemComponent/fn_71009c2d74.cpp). Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

struct UnkSource;
struct UnkSourceArg;
void fn_71009c5ab8(UnkSource* src, UnkSourceArg* arg);

struct UnkStageDesc_A4_b75 {
    u8 pad_0x0[0x30];
    u64 id;
};

UnkReflObject* fn_71009c0e24(void* owner, u64 id);

template <const base::reflection::CClass* (*GetClass)()>
static inline __attribute__((always_inline)) UnkReflValue InvokeStage(char* obj, UnkReflValue* const* values) {
    UnkReflValue* args = *values;
    void* owner = *reinterpret_cast<void**>(obj + 8);
    UnkStageDesc_A4_b75* const* desc =
        args[0].type ? refl_ArgValue<UnkStageDesc_A4_b75*>(&args[0]) : nullptr;
    UnkSourceArg* arg = args[1].type ? const_cast<UnkSourceArg*>(refl_ArgValue<UnkSourceArg>(&args[1])) : nullptr;
    if (UnkReflObject* src = fn_71009c0e24(owner, (*desc)->id)) {
        if (fn_710009d50c(src->vf_0x0(), GetClass()))
            fn_71009c5ab8(reinterpret_cast<UnkSource*>(src), arg);
    }
    return refl_EmptyValue();
}

UnkReflValue fn_71009b84d0(void*, char* obj, UnkReflValue* const* values) {
    return InvokeStage<fn_71009b8664>(obj, values);
}

UnkReflValue fn_71009bd120(void*, char* obj, UnkReflValue* const* values) {
    return InvokeStage<fn_71009bd2b4>(obj, values);
}
