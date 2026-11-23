// Copies of fn_7100b87488 (src/game_aca57c/fn_7100ad4b54.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_71005abe98, fn_71005abf48.
// Getters building a value of kind 2 (fn_71000a18d4 fills in the rest from the type the value is
// described by): words of the translation unit's globals, a zero, flags of an object and a vector.
#include "base/UnkAngleSource.h"
#include "base/UnkFlagSource.h"
#include "base/UnkStateObject.h"
#include "base/UnkVector3.h"
#include "base/UnkVectorSource.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/reflection.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBossAIComponent.h"
#include "game/UnkDefIterValue.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1633.h"
#include "game/UnkTU2200.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU437.h"
#include "types.h"
#include "game/sound/CMusicManager.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

__attribute__((always_inline)) static inline UnkReflValue MakeValue(const CType* type, const void* value) {
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, type, value, 0);
    return result;
}

UnkReflValue fn_71005abe98() {
    u32 value = 0;
    return MakeValue(fn_710000abbc(), &value);
}

UnkReflValue fn_71005abf48() {
    u32 value = 0;
    return MakeValue(fn_7100005178(), &value);
}

template <class T>
struct UnkArgument {
    u8 unk_0x0[8];
    T* field_0x8;
};

// Flags of the argument's object (at 0xd63d6b, 0xd641c4 and 0xd641c5).
template <u32 Offset>
__attribute__((always_inline)) static inline UnkReflValue GetFlag(UnkArgument<bool>* argument) {
    bool value = argument->field_0x8[Offset];
    return MakeValue(fn_710000d228(), &value);
}

__attribute__((always_inline)) static inline UnkReflValue GetVirtualFlag(UnkArgument<UnkFlagSource>* argument) {
    bool value = argument->field_0x8->vf_0x58();
    return MakeValue(fn_710000d228(), &value);
}

// Values of kind 2 referring (flag 1) to a cutscene definition held by an iterator value: in place
// at 0x8, or through the pointer there when the state (after the Size-byte value) is 2. The class
// getter is called first, so the class (whose reflected data is at the descriptor) is set up.

template <u32 Size, void* (*GetClass)(), const CType* Descriptor>
__attribute__((always_inline)) static inline UnkReflValue GetDef(UnkArgument<UnkDefIterValue<Size>>* argument) {
    void* def = argument->field_0x8->Get();
    GetClass();
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    fn_71000a18d4(&result, Descriptor, def, 1);
    return result;
}

// Values of kind 2 holding a manager singleton, created on first use (allocated with flags 0x3f or
// 3, and constructed in place).
struct UnkManager;
struct UnkMgr_0xf18;

template <void** Instance, u64 Size, void (*Construct)(void*), const CType* (*GetType)(), u32 Flags = 0x3f>
__attribute__((always_inline)) static inline UnkReflValue GetSingleton() {
    void* instance = *Instance;
    if (!instance) {
        instance = fn_71000839a0(Size, Flags);
        Construct(instance);
        *Instance = instance;
    }
    void* value = instance;
    return MakeValue(GetType(), &value);
}

