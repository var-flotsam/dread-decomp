// CSensorDoorComponent's destructors (complete and deleting) (tools/tu_map.py, the name from reflection): the vtable, then the
// two value holders at +0x108 and +0x178 (the layout of base::UnkValueHolder: their four cleared pairs,
// their vtable and the reflected value, fn_71000a1c60) and the zeroed member at +0x58 (vtable
// unk_7101a70cb0), then the component base destructor (fn_71010c4194). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkComponent.h"
#include "types.h"


extern char g_unk_7101a70a88[];  // the component's vtable
extern char g_unk_7101a70c58[];  // the value holders' vtable
extern char g_unk_7101a70cb0[];  // the member's vtable

struct UnkSensorPair {
    u64 a, b;
    ~UnkSensorPair() {
        a = 0;
        b = 0;
    }
};

struct UnkSensorValueHolderBase {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    ~UnkSensorValueHolderBase() {
        vtable = g_unk_7101a70c58 + 0x10;
        fn_71000a1c60(&value);
    }
};

struct UnkSensorValueHolder : UnkSensorValueHolderBase {
    UnkSensorPair pairs[4];
};
static_assert(sizeof(UnkSensorValueHolder) == 0x70, "value holder layout");

struct UnkSensorMember {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    ~UnkSensorMember() {
        vtable = g_unk_7101a70cb0 + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkSensorDoorBase : UnkBaseComponent {
    ~UnkSensorDoorBase() { fn_71010c4194(this); }
};


struct UnkSensorDoorComponent : UnkSensorDoorBase {
    u8 pad_0x40[0x18];
    UnkSensorMember field_0x58;
    u8 pad_0x80[0x108 - 0x80];
    UnkSensorValueHolder field_0x108;
    UnkSensorValueHolder field_0x178;
    ~UnkSensorDoorComponent() { vtable = reinterpret_cast<void**>(g_unk_7101a70a88 + 0x10); }
};
static_assert(__builtin_offsetof(UnkSensorDoorComponent, field_0x178) == 0x178, "layout");

void fn_71007b7c88(UnkSensorDoorComponent* self) {
    self->~UnkSensorDoorComponent();
}

// The deleting destructor.
void fn_71007b7d5c(UnkSensorDoorComponent* self) {
    self->~UnkSensorDoorComponent();
    fn_7100083c00(self);
}
