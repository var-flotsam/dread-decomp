// CAbilityShinespark unit: the shinespark's destructor. Clears its state machine and five states, releases
// its counted reference, string id and two buffers, unregisters its two listeners, then destroys CAbility's
// members and runs the base destructor fn_7101043934. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityShinespark.h"
#include "game/CGameObject.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern char g_unk_7101a232a0[];  // CAbilityShinespark's vtable
extern char g_unk_7101a23550[];  // its state machine's vtable

struct UnkCameraCtrl_ovb0049;

static inline void Release(UnkRefObject* object) {
    if (object && --object->ref_count == 0)
        object->vf_0x10();
}

struct UnkShinesparkPair_l32804c {
    u64 a, b;

    void Destroy() {
        a = 0;
        b = 0;
    }
};

// A value holder: a reflected value and pairs cleared on destruction (the states bind their callbacks in
// three of them).
template <int N, int Size>
struct UnkShinesparkHolder_l32804c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkShinesparkPair_l32804c pairs[N];
    u8 pad[Size - 0x30 - N * 0x10];

    void Destroy(void** holder_vtable) {
        for (int i = N - 1; i >= 0; --i)
            pairs[i].Destroy();
        vtable = holder_vtable;
        fn_71000a1c60(&value);
    }
};
using UnkShinesparkState_l32804c = UnkShinesparkHolder_l32804c<3, 0x70>;

struct UnkShinesparkMember_l32804c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    void Destroy() {
        vtable = &g_unk_7101a22650[2];
        field_0x20 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
    }
};

struct UnkShinesparkMachine_l32804c {
    void** vtable;
    void* owner;
    void* current;
    void* previous;
    u32 count;
    u32 states[0x20];

    void Destroy() {
        vtable = reinterpret_cast<void**>(g_unk_7101a23550) + 2;
        current = nullptr;
        previous = nullptr;
        count = 0;
    }
};

// A listener registered on a target through a weak handle.
struct UnkShinesparkListener_l32804c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    UnkWeakHandle* handle;

    void Destroy() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkWeakHandle* h = handle) {
            if (h->target)
                fn_710117b3f0(h->target, this);
            Release(handle);
        }
    }
};

struct UnkShinesparkBuffers_l32804c {
    void* keys;
    void* values;
    u32 count;
    u8 pad_0x14[0x20 - 0x14];
    base::IAllocator* allocator;

    void Destroy() {
        count = 0;
        if (values)
            allocator->vf_0x48(values);
        if (keys)
            allocator->vf_0x48(keys);
    }
};

struct UnkAbilityShinespark_l32804c {
    void** vtable;
    u64 field_0x8;
    UnkShinesparkMember_l32804c member;  // 0x10
    u8 pad_0x38[0xe8 - 0x38];
    UnkShinesparkHolder_l32804c<4, 0x70> holders[6];  // 0xe8
    u8 pad_0x388[0x3b8 - 0x388];
    UnkShinesparkListener_l32804c listener_0x3b8;  // 0x3b8
    UnkShinesparkListener_l32804c listener_0x3d8;  // 0x3d8
    u8 pad_0x3f8[0x400 - 0x3f8];
    UnkShinesparkBuffers_l32804c buffers;  // 0x400
    u8 pad_0x428[0x448 - 0x428];
    base::global::CStrId name;  // 0x448
    u8 pad_0x450[0x468 - 0x450];
    UnkRefObject* ref;  // 0x468
    UnkShinesparkState_l32804c states[5];  // 0x470
    UnkShinesparkMachine_l32804c machine;  // 0x6a0
};
static_assert(__builtin_offsetof(UnkAbilityShinespark_l32804c, holders) == 0xe8, "layout");
static_assert(__builtin_offsetof(UnkAbilityShinespark_l32804c, listener_0x3b8) == 0x3b8, "layout");
static_assert(__builtin_offsetof(UnkAbilityShinespark_l32804c, buffers) == 0x400, "layout");
static_assert(__builtin_offsetof(UnkAbilityShinespark_l32804c, name) == 0x448, "layout");
static_assert(__builtin_offsetof(UnkAbilityShinespark_l32804c, states) == 0x470, "layout");
static_assert(__builtin_offsetof(UnkAbilityShinespark_l32804c, machine) == 0x6a0, "layout");

void fn_710032804c(UnkAbilityShinespark_l32804c* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a232a0) + 2;
    self->machine.Destroy();
    for (int i = 4; i >= 0; --i)
        self->states[i].Destroy(reinterpret_cast<void**>(g_unk_7101a234f8) + 2);
    Release(self->ref);
    self->name.~CStrId();
    self->buffers.Destroy();
    self->listener_0x3d8.Destroy();
    self->listener_0x3b8.Destroy();

    // CAbility's members, then its base.
    self->vtable = &g_unk_7101a223f8[2];
    for (int i = 5; i >= 0; --i)
        self->holders[i].Destroy(&g_unk_7101a225f8[2]);
    self->member.Destroy();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
