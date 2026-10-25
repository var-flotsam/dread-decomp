// Getters building a value of kind 2 (fn_71000a18d4 fills in the rest from the type the value is
// described by): words of the translation unit's globals, a zero, flags of an object and a vector.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"  // the type getters (refl_GetType<T>)
#include "types.h"

struct UnkValue {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
};

void fn_71000a18d4(UnkValue* out, void* type, const void* value, u32 flag);

// The type getters return a const descriptor; fn_71000a18d4 is declared with the non-const pointer
// its other callers use (it only reads the descriptor).
__attribute__((always_inline)) static inline UnkValue MakeValue(const void* type, const void* value) {
    UnkValue result;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    result.field_0x10 = 2;
    fn_71000a18d4(&result, const_cast<void*>(type), value, 0);
    return result;
}

// fn_7100014768 (int), fn_7100005178 (unsigned), fn_710000abbc (float), fn_710000d228 (bool) and
// fn_71000246e4 (CVector3D) are the type getters UnkReflAttributes.h declares.
void* fn_7100010e58();  // an 8-byte type's getter

extern u32 g_unk_7172756b90 __attribute__((visibility("hidden")));
extern u32 g_unk_7172756cbc __attribute__((visibility("hidden")));
extern u64 g_unk_7172756b98 __attribute__((visibility("hidden")));
extern u32 g_unk_7172756ba0 __attribute__((visibility("hidden")));

UnkValue fn_7100ad4b54() {
    u32 value = g_unk_7172756b90;
    return MakeValue(fn_7100014768(), &value);
}
UnkValue fn_7100ad4ba0() {
    u32 value = g_unk_7172756cbc;
    return MakeValue(fn_7100005178(), &value);
}
UnkValue fn_7100ad4bec() {
    u64 value = g_unk_7172756b98;
    return MakeValue(fn_7100010e58(), &value);
}
UnkValue fn_7100ad4c38() {
    u32 value = g_unk_7172756ba0;  // a float's bits, copied as a word
    return MakeValue(fn_710000abbc(), &value);
}

UnkValue fn_7100b87488() {
    u32 value = 0;
    return MakeValue(fn_7100014768(), &value);
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

UnkValue fn_7100d2c880(void* unused, UnkArgument<UnkStateObject>* argument) {
    bool value = argument->field_0x8->field_0x24 == 2;
    return MakeValue(fn_710000d228(), &value);
}

// Flags of the argument's object (at 0xd63d6b, 0xd641c4 and 0xd641c5).
template <u32 Offset>
__attribute__((always_inline)) static inline UnkValue GetFlag(UnkArgument<bool>* argument) {
    bool value = argument->field_0x8[Offset];
    return MakeValue(fn_710000d228(), &value);
}

UnkValue fn_7100eaa1c4(void* unused, UnkArgument<bool>* argument) { return GetFlag<0xd63d6b>(argument); }
UnkValue fn_7100eaa53c(void* unused, UnkArgument<bool>* argument) { return GetFlag<0xd641c4>(argument); }
UnkValue fn_7100eaa5f8(void* unused, UnkArgument<bool>* argument) { return GetFlag<0xd641c5>(argument); }

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

UnkValue fn_7100d6e0d4(void* unused, UnkArgument<UnkVectorSource>* argument) {
    UnkVector3 value = argument->field_0x8->vf_0x68();
    return MakeValue(fn_71000246e4(), &value);
}

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

__attribute__((always_inline)) static inline UnkValue GetVirtualFlag(UnkArgument<UnkFlagSource>* argument) {
    bool value = argument->field_0x8->vf_0x58();
    return MakeValue(fn_710000d228(), &value);
}

UnkValue fn_7100d5ee80(void* unused, UnkArgument<UnkFlagSource>* argument) { return GetVirtualFlag(argument); }
UnkValue fn_7100d60ae0(void* unused, UnkArgument<UnkFlagSource>* argument) { return GetVirtualFlag(argument); }
UnkValue fn_7100d61c30(void* unused, UnkArgument<UnkFlagSource>* argument) { return GetVirtualFlag(argument); }
UnkValue fn_7100d691e0(void* unused, UnkArgument<UnkFlagSource>* argument) { return GetVirtualFlag(argument); }

// Values of kind 2 referring (flag 1) to a cutscene definition held by an iterator value: in place
// at 0x8, or through the pointer there when the state (after the Size-byte value) is 2. The class
// getter is called first, so the class (whose reflected data is at the descriptor) is set up.
extern u8 g_unk_7172794c08[] __attribute__((visibility("hidden")));
extern u8 g_unk_7172794d40[] __attribute__((visibility("hidden")));
extern u8 g_unk_7172794e78[] __attribute__((visibility("hidden")));

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

template <u32 Size, const base::reflection::CClass* (*GetClass)(), u8* Descriptor>
__attribute__((always_inline)) static inline UnkValue GetDef(UnkArgument<UnkDefIterValue<Size>>* argument) {
    void* def = argument->field_0x8->Get();
    GetClass();
    UnkValue result;
    result.field_0x0 = 0;
    result.field_0x8 = 0;
    result.field_0x10 = 2;
    fn_71000a18d4(&result, Descriptor, def, 1);
    return result;
}

UnkValue fn_7100c8e85c(void* unused, UnkArgument<UnkDefIterValue<0xa0>>* argument) {
    return GetDef<0xa0, fn_7100c828a4, g_unk_7172794c08>(argument);
}
UnkValue fn_7100c8e9c4(void* unused, UnkArgument<UnkDefIterValue<0xa0>>* argument) {
    return GetDef<0xa0, fn_7100c828a4, g_unk_7172794c08>(argument);
}
UnkValue fn_7100c9022c(void* unused, UnkArgument<UnkDefIterValue<0x18>>* argument) {
    return GetDef<0x18, fn_7100c84768, g_unk_7172794d40>(argument);
}
UnkValue fn_7100c90448(void* unused, UnkArgument<UnkDefIterValue<0x18>>* argument) {
    return GetDef<0x18, fn_7100c84768, g_unk_7172794d40>(argument);
}
UnkValue fn_7100c91b80(void* unused, UnkArgument<UnkDefIterValue<0x40>>* argument) {
    return GetDef<0x40, fn_7100c85d3c, g_unk_7172794e78>(argument);
}
UnkValue fn_7100c91e70(void* unused, UnkArgument<UnkDefIterValue<0x40>>* argument) {
    return GetDef<0x40, fn_7100c85d3c, g_unk_7172794e78>(argument);
}

// Values of kind 2 holding a manager singleton, created on first use (allocated with flags 0x3f or
// 3, and constructed in place).
void fn_7100ee1f34(void* self);
void fn_7100d8ced8(void* self);
void fn_7100359118(void* self);
void fn_7100fd3804(void* self);
void* fn_7100ad61f8();  // base::reflection::refl_GetType<CSubAreaManager*>
void* fn_7100ad62d4();  // base::reflection::refl_GetType<CEnvironmentManager*>
void* fn_7100ad6568();  // base::reflection::refl_GetType<CLightManager*>
void* fn_7100ad6644();  // base::reflection::refl_GetType<sound::CMusicManager*>
extern void* g_unk_7101d47360;
extern void* g_unk_7101d47368;
extern void* g_unk_71726b1688;
extern void* g_unk_71726bc560;

template <void** Instance, u64 Size, void (*Construct)(void*), void* (*GetType)(), u32 Flags = 0x3f>
__attribute__((always_inline)) static inline UnkValue GetSingleton() {
    void* instance = *Instance;
    if (!instance) {
        instance = fn_71000839a0(Size, Flags);
        Construct(instance);
        *Instance = instance;
    }
    void* value = instance;
    return MakeValue(GetType(), &value);
}

UnkValue fn_7100adaa30() { return GetSingleton<&g_unk_7101d47360, 0xae8, fn_7100ee1f34, fn_7100ad61f8>(); }
UnkValue fn_7100adaaa4() { return GetSingleton<&g_unk_7101d47368, 0x4c8, fn_7100d8ced8, fn_7100ad62d4>(); }
UnkValue fn_7100adabb8() { return GetSingleton<&g_unk_71726b1688, 0x198, fn_7100359118, fn_7100ad6568, 3>(); }
UnkValue fn_7100adac2c() { return GetSingleton<&g_unk_71726bc560, 0xf18, fn_7100fd3804, fn_7100ad6644>(); }

// The sum of the two angles at 0xc8 of the argument's object, in degrees.
struct UnkAngleSource {
    u8 unk_0x0[0xc8];
    f32 field_0xc8;
    f32 field_0xcc;
};

UnkValue fn_7100d690c4(void* unused, UnkArgument<UnkAngleSource>* argument) {
    UnkAngleSource* source = argument->field_0x8;
    f32 value = (source->field_0xcc + source->field_0xc8) * 180.0f / 3.141592653589793;
    return MakeValue(fn_710000abbc(), &value);
}
