// Copies of fn_7100b87488 (src/game_aca57c/fn_7100ad4b54.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_71005abe98, fn_71005abf48.
// Getters building a value of kind 2 (fn_71000a18d4 fills in the rest from the type the value is
// described by): words of the translation unit's globals, a zero, flags of an object and a vector.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

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

const CType* fn_7100010e58();  // an 8-byte type's getter

extern u32 g_unk_7172756b90 __attribute__((visibility("hidden")));
extern u32 g_unk_7172756cbc __attribute__((visibility("hidden")));
extern u64 g_unk_7172756b98 __attribute__((visibility("hidden")));
extern u32 g_unk_7172756ba0 __attribute__((visibility("hidden")));

UnkReflValue fn_71005abe98() {
    u32 value = 0;
    return MakeValue(fn_710000abbc(), &value);
}

UnkReflValue fn_71005abf48() {
    u32 value = 0;
    return MakeValue(fn_7100005178(), &value);
}

// Whether the word at 0x24 of the argument's object is 2.
struct UnkStateObject {
    u8 unk_0x0[0x24];
    s32 field_0x24;
};

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

// The vector the argument's object returns from its virtual function at 0x68.
// (Returned in memory: its copy constructor is user-provided.)
struct UnkVector3 {
    f32 x, y, z;

    __attribute__((always_inline)) UnkVector3(const UnkVector3& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkVectorSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual UnkVector3 vf_0x68();
};

// The flag the argument's object returns from its virtual function at 0x58.
struct UnkFlagSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual bool vf_0x58();
};

__attribute__((always_inline)) static inline UnkReflValue GetVirtualFlag(UnkArgument<UnkFlagSource>* argument) {
    bool value = argument->field_0x8->vf_0x58();
    return MakeValue(fn_710000d228(), &value);
}

// Values of kind 2 referring (flag 1) to a cutscene definition held by an iterator value: in place
// at 0x8, or through the pointer there when the state (after the Size-byte value) is 2. The class
// getter is called first, so the class (whose reflected data is at the descriptor) is set up.
extern const CType g_unk_7172794c08 __attribute__((visibility("hidden")));
extern const CType g_unk_7172794d40 __attribute__((visibility("hidden")));
extern const CType g_unk_7172794e78 __attribute__((visibility("hidden")));

template <u32 Size>
struct UnkDefIterValue {
    u64 unk_0x0;
    union {
        u8 value[Size];
        void* ref;
    };
    s32 state;

    __attribute__((always_inline)) void* Get() { return state == 2 ? ref : value; }
};

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
void fn_7100ee1f34(void* self);
void fn_7100d8ced8(void* self);
void fn_7100359118(void* self);
void fn_7100fd3804(void* self);
const CType* fn_7100ad61f8();  // base::reflection::refl_GetType<CSubAreaManager*>
const CType* fn_7100ad62d4();  // base::reflection::refl_GetType<CEnvironmentManager*>
const CType* fn_7100ad6568();  // base::reflection::refl_GetType<CLightManager*>
const CType* fn_7100ad6644();  // base::reflection::refl_GetType<sound::CMusicManager*>
extern void* g_unk_7101d47360;
extern void* g_unk_7101d47368;
extern void* g_unk_71726b1688;
extern void* g_unk_71726bc560;

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

// The sum of the two angles at 0xc8 of the argument's object, in degrees.
struct UnkAngleSource {
    u8 unk_0x0[0xc8];
    f32 field_0xc8;
    f32 field_0xcc;
};


