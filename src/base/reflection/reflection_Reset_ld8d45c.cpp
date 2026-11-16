// Unit 2525 (base::reflection): stops the sound of kind 5, frees the three owned objects and clears
// the record's ids and values back to their defaults. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/CRntVector.h"
#include "base/reflection.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU23.h"
#include "types.h"

using base::global::CStrId;

struct UnkOwnedA_ld8d45c;
struct UnkOwnedB_ld8d45c;
struct UnkOwnedC_ld8d45c;
void fn_7100f9c184(UnkOwnedA_ld8d45c* self);
void fn_7100d8f8f8(UnkOwnedA_ld8d45c* self);
void fn_7100f949a8(UnkOwnedB_ld8d45c* self);
void fn_7100d90040(UnkOwnedB_ld8d45c* self);
struct UnkMusicPresets;
void fn_7100f89678(UnkMusicPresets* self);

struct UnkOwnedA_ld8d45c {
    ~UnkOwnedA_ld8d45c() {
        fn_7100f9c184(this);
        fn_7100d8f8f8(this);
    }
    static void operator delete(void* ptr) { fn_7100083c00(ptr); }
};

struct UnkOwnedB_ld8d45c {
    ~UnkOwnedB_ld8d45c() {
        fn_7100f949a8(this);
        fn_7100d90040(this);
    }
    static void operator delete(void* ptr) { fn_7100083c00(ptr); }
};

struct UnkOwnedC_ld8d45c {
    u8 pad_0x0[0x48];
    u8 field_0x48[8];

    ~UnkOwnedC_ld8d45c() {
        fn_7100f89678(reinterpret_cast<UnkMusicPresets*>(this));
        fn_7100d9012c(field_0x48);
        fn_7100d90218(this);
    }
    static void operator delete(void* ptr) { fn_7100083c00(ptr); }
};

// A named value.
struct UnkNamedValue_ld8d45c {
    CStrId name;
    u64 value;

    void Reset() {
        name = g_unk_7101d470e0;
        value = 0;
    }
};

// Three pointers cleared together.
struct UnkSpan_ld8d45c {
    void* begin;
    void* end;
    void* capacity;

    void Clear() {
        begin = nullptr;
        end = nullptr;
        capacity = nullptr;
    }
};

struct UnkRecord_ld8d45c {
    UnkOwnedA_ld8d45c* a;
    UnkOwnedB_ld8d45c* b;
    UnkOwnedC_ld8d45c* c;
    UnkNamedValue_ld8d45c first;  // 0x18
    u8 pad_0x28[0x450];
    UnkNamedValue_ld8d45c second;  // 0x478
    UnkNamedValue_ld8d45c third;   // 0x488
    u8 pad_0x498[0xc];
    u8 field_0x4a4[3];
    UnkSpan_ld8d45c span;  // 0x4a8
    u8 field_0x4c0;
    u8 field_0x4c1;
};
static_assert(__builtin_offsetof(UnkRecord_ld8d45c, second) == 0x478, "UnkRecord_ld8d45c layout");
static_assert(__builtin_offsetof(UnkRecord_ld8d45c, field_0x4a4) == 0x4a4, "UnkRecord_ld8d45c layout");
static_assert(__builtin_offsetof(UnkRecord_ld8d45c, field_0x4c1) == 0x4c1, "UnkRecord_ld8d45c layout");

template <typename T>
inline void DeleteOwned(T*& ptr) {
    if (ptr) {
        delete ptr;
        ptr = nullptr;
    }
}

void fn_7100d8d45c(UnkRecord_ld8d45c* self) {
    fn_7100108ae8(g_unk_7101d4d820, 5, 0.0f, 0.0f, false);
    DeleteOwned(self->a);
    DeleteOwned(self->b);
    DeleteOwned(self->c);
    self->first.Reset();
    self->second.Reset();
    self->third.Reset();
    self->field_0x4a4[0] = 0;
    self->field_0x4a4[1] = 0;
    self->field_0x4a4[2] = 0;
    self->span.Clear();
    self->field_0x4c1 = 0;
}
