// Unit 2559: passes the names collected from an object (fn_7100e19b40) to the loading screen, at most
// five of them, unless the screen (+0x1b0) is already busy. With the flag set each name gets a
// weight of 1000; without it, a name takes its own weight (+0x14) once there are at least two.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkLoadingScreen;
extern UnkLoadingScreen* g_unk_7101d47380;
void fn_7100e0f89c(UnkLoadingScreen* self);

// Creates the object on first use: storage from the default allocator, built by its constructor
// function (as src/engine/HostArguments.cpp's).
template <class T, u64 Size, void (*Ctor)(T*)>
__attribute__((always_inline)) static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

struct UnkLoadingScreenBusy_ovb0262m {
    u8 pad_0x0[0x1b0];
    u32 busy;  // 0x1b0
};

struct UnkNameSource_ovb0262m {
    u8 pad_0x0[8];
    u32 count;  // 0x8
};

struct UnkLoadName_ovb0262m {
    base::global::UnkStringData* id;  // 0x0 (a string id)
    u8 pad_0x8[0xc];
    f32 weight;  // 0x14
};

void fn_7100e19b40(UnkNameSource_ovb0262m* self, base::UnkVector<UnkLoadName_ovb0262m*>* out);
void fn_7100e13784(UnkLoadingScreen* self, const base::global::CRntString* name, f32 weight);

// The text of the name's pooled data, its use count raised first (a CRntString at +0x10).
static inline const base::global::CRntString* NameText(UnkLoadName_ovb0262m* name) {
    // The count is written untyped: the original reads the name again afterwards.
    u8* data = reinterpret_cast<u8*>(name->id);
    u32 uses;
    __builtin_memcpy(&uses, data + 0x44, sizeof(uses));
    ++uses;
    __builtin_memcpy(data + 0x44, &uses, sizeof(uses));
    return reinterpret_cast<const base::global::CRntString*>(reinterpret_cast<u8*>(name->id) + 0x10);
}

struct UnkLoadCheckPointSingleton_ovb0118;

void fn_7100e19954(UnkLoadCheckPointSingleton_ovb0118* self_, bool flag) {
    UnkNameSource_ovb0262m* self = reinterpret_cast<UnkNameSource_ovb0262m*>(self_);
    if (reinterpret_cast<UnkLoadingScreenBusy_ovb0262m*>(
            Singleton<UnkLoadingScreen, 0x220, fn_7100e0f89c>(g_unk_7101d47380))->busy)
        return;
    base::UnkVector<UnkLoadName_ovb0262m*> names(0xf);
    names.Reserve(self->count);
    fn_7100e19b40(self, &names);
    for (u64 i = 0; i < names.count && i < 5; ++i) {
        UnkLoadName_ovb0262m* name = names.data[i];
        UnkLoadingScreen* screen = Singleton<UnkLoadingScreen, 0x220, fn_7100e0f89c>(g_unk_7101d47380);
        fn_7100e13784(screen, NameText(name),
                      flag ? 1000.0f : (names.count < 2 ? 1000.0f : name->weight));
    }
    names.Free();
}
