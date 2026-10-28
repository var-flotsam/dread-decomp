// Constructor of a large navigation component: constructs its members in order, sets up its
// containers and fixed-size arrays empty, and names its state-machine pool
// "NavComponentStateMachinePool".
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkMember_0x10 {
    UnkMember_0x10(u32 a, u32 b, u32 c, void* d);  // fn_71003fa9fc
    alignas(8) u8 unk[0x70];
};
struct UnkMember_0x80 {
    UnkMember_0x80();  // fn_7100639cf8
    alignas(8) u8 unk[0xb8];
};
struct UnkMember_0x138 {
    UnkMember_0x138();  // fn_710059c5d8
    alignas(8) u8 unk[0xf0];
};
struct UnkMember_0x228 {
    UnkMember_0x228();  // fn_71004751f8
    alignas(8) u8 unk[0x90];
};
struct UnkMember_0x2b8 {
    UnkMember_0x2b8();  // fn_71005ac6e0
    alignas(8) u8 unk[0x78];
};
struct UnkMember_0x330 {
    UnkMember_0x330();  // fn_71003f8b30
    alignas(8) u8 unk[0x38];
};
struct UnkMember_0x368 {
    UnkMember_0x368();  // fn_71006de790
    alignas(8) u8 unk[0x88];
};
struct UnkMember_0x3f0 {
    UnkMember_0x3f0();  // fn_71006bcf84
    alignas(8) u8 unk[0x80];
};
struct UnkMember_0x4f0 {
    UnkMember_0x4f0();  // fn_710057a010
    alignas(8) u8 unk[0x78];
};
struct UnkMember_0x568 {
    UnkMember_0x568();  // fn_71005cce80
    alignas(8) u8 unk[0x78];
};
struct UnkMember_0x5e0 {
    UnkMember_0x5e0();  // fn_71006a1cc0
    alignas(8) u8 unk[0x348];
};
struct UnkMember_0x928 {
    UnkMember_0x928();  // fn_71003dbf20
    alignas(8) u8 unk[0x260];
};
struct UnkStateMachinePool {
    UnkStateMachinePool();                // fn_7100f11ce8
    void SetName(const CStrId& name);     // fn_7100f11e48
    alignas(8) u8 unk[0x40];
};
struct UnkMember_0x2528 {
    UnkMember_0x2528(u32 a, u32 b, u32 c, void* d);  // fn_7100da809c
    alignas(8) u8 unk[0x70];
};
struct UnkMember_0x25a0 {
    UnkMember_0x25a0(u32 a, u32 b, u32 c, void* d);  // fn_7100da81b4
    alignas(8) u8 unk[0x70];
};

struct UnkList_0x30 {
    void* field_0x0 = nullptr;
    void* field_0x8 = nullptr;
    void* field_0x10 = nullptr;
    u32 step = 10;
    u32 flags = 0xe;
    base::UnkAllocator* allocator = fn_7100083990();
    void* field_0x28 = nullptr;
};

struct UnkFixedArray {
    u32 count = 0;
    u8 storage[0x400];
};

struct UnkNavComponent {
    void* field_0x0 = nullptr;
    bool field_0x8 = false;
    UnkMember_0x10 field_0x10{0x32, 0, 0x13, nullptr};
    UnkMember_0x80 field_0x80;
    UnkMember_0x138 field_0x138;
    UnkMember_0x228 field_0x228;
    UnkMember_0x2b8 field_0x2b8;
    UnkMember_0x330 field_0x330;
    UnkMember_0x368 field_0x368;
    UnkMember_0x3f0 field_0x3f0;
    UnkMember_0x3f0 field_0x470;
    UnkMember_0x4f0 field_0x4f0;
    UnkMember_0x568 field_0x568;
    UnkMember_0x5e0 field_0x5e0;
    UnkMember_0x928 field_0x928;
    UnkStateMachinePool stateMachinePool;  // 0xb88
    UnkList_0x30 field_0xbc8;
    UnkFixedArray field_0xbf8;
    UnkFixedArray field_0xffc;
    UnkFixedArray field_0x1400;
    UnkFixedArray field_0x1804;
    UnkFixedArray field_0x1c08;
    UnkFixedArray field_0x200c;
    void* field_0x2410 = nullptr;
    u32 field_0x2418 = 0;
    bool field_0x241c = false;
    u32 field_0x2420 = 0;
    base::UnkVector<void*> field_0x2428{0xf};
    base::UnkVector<void*> field_0x2450{0xf};
    base::UnkVector<void*> field_0x2478{0xf};
    bool field_0x24a0 = false;
    base::UnkVector<void*> field_0x24a8{0xf};
    base::UnkVector<void*> field_0x24d0{0xf};
    base::UnkVector<void*> field_0x24f8{0xf};
    bool field_0x2520 = false;
    UnkMember_0x2528 field_0x2528{0x20, 0, 4, nullptr};
    bool field_0x2598 = false;
    UnkMember_0x25a0 field_0x25a0{0x40, 0, 4, nullptr};

    __attribute__((always_inline)) UnkNavComponent() {
        stateMachinePool.SetName(CStrId("NavComponentStateMachinePool", true));
    }
};

static_assert(__builtin_offsetof(UnkNavComponent, stateMachinePool) == 0xb88, "layout");
static_assert(__builtin_offsetof(UnkNavComponent, field_0x2428) == 0x2428, "layout");
static_assert(__builtin_offsetof(UnkNavComponent, field_0x25a0) == 0x25a0, "layout");

void fn_7100da7e2c(UnkNavComponent* self) {
    new (self) UnkNavComponent();
}
