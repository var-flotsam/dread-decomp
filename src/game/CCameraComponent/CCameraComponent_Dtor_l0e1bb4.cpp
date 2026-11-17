// CCameraComponent's unit (tools/tu_map.py unit 2691): the component's destructor. Releases three
// references, destroys its members in reverse order and runs the base component's destructor.
// Names and layouts are placeholders.
#include "base/UnkKeyedStrIds.h"
#include "base/global/CRntVector.h"
#include "game/UnkComponent.h"
#include "game/UnkTU2732.h"
#include "game/UnkTU39.h"
#include "types.h"

extern char g_unk_7101cb1390[];
extern char g_unk_7101cb1510[];
extern char g_unk_7101cb1530[];

void fn_71010e1d70(UnkKeyedStrIds* self);

struct UnkCameraRefObj_l0e1bb4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkCameraRef_l0e1bb4 {
    UnkCameraRefObj_l0e1bb4* ptr;
    ~UnkCameraRef_l0e1bb4() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkCameraNode_l0e1bb4 {
    u64 field_0x0;
    UnkCameraNode_l0e1bb4* next;
};

template <char* Vtable>
struct UnkCameraNodeList_l0e1bb4 {
    char* vtable;
    u32 count;
    UnkCameraNode_l0e1bb4* head;
    UnkCameraNode_l0e1bb4* tail;
    base::UnkAllocator* allocator;
    u8 unk_0x28[0x8];

    ~UnkCameraNodeList_l0e1bb4() {
        vtable = Vtable + 0x10;
        for (UnkCameraNode_l0e1bb4* n = head; n;) {
            UnkCameraNode_l0e1bb4* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkCameraKeyedIds_l0e1bb4 : UnkKeyedStrIds {
    ~UnkCameraKeyedIds_l0e1bb4() { fn_71010e1d70(this); }
};

struct UnkCameraSlot_l0e1bb4 {
    u8 data[0xb8];
    ~UnkCameraSlot_l0e1bb4() { fn_7100030a50(reinterpret_cast<UnkMoveSlot_0xb0*>(this)); }
};

struct UnkCameraState_l0e1bb4 {
    u8 data[0x9a0];
    ~UnkCameraState_l0e1bb4() { fn_710119ad3c(this); }
};

struct UnkCameraComponent_l0e1bb4 : UnkBaseComponent {
    u8 unk_0x40[0x10];
    UnkCameraNodeList_l0e1bb4<g_unk_7101cb1510> list_0x50;
    UnkCameraKeyedIds_l0e1bb4 ids;  // +0x80
    UnkCameraNodeList_l0e1bb4<g_unk_7101cb1530> list_0xa8;
    u8 unk_0xd8[0x18];
    UnkCameraSlot_l0e1bb4 slot_0xf0;
    UnkCameraSlot_l0e1bb4 slot_0x1a8;
    u8 unk_0x260[0x20];
    UnkCameraState_l0e1bb4 state;  // +0x280
    u8 unk_0xc20[0x10];
    UnkCameraRef_l0e1bb4 field_0xc30;
    UnkCameraRef_l0e1bb4 field_0xc38;
    u8 unk_0xc40[0x8];
    UnkCameraRef_l0e1bb4 field_0xc48;
};
static_assert(__builtin_offsetof(UnkCameraComponent_l0e1bb4, list_0xa8) == 0xa8, "layout");
static_assert(__builtin_offsetof(UnkCameraComponent_l0e1bb4, state) == 0x280, "layout");
static_assert(__builtin_offsetof(UnkCameraComponent_l0e1bb4, field_0xc48) == 0xc48, "layout");

void fn_71010e1bb4(UnkCameraComponent_l0e1bb4* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101cb1390 + 0x10);
    self->field_0xc48.~UnkCameraRef_l0e1bb4();
    self->field_0xc38.~UnkCameraRef_l0e1bb4();
    self->field_0xc30.~UnkCameraRef_l0e1bb4();
    self->state.~UnkCameraState_l0e1bb4();
    self->slot_0x1a8.~UnkCameraSlot_l0e1bb4();
    self->slot_0xf0.~UnkCameraSlot_l0e1bb4();
    self->list_0xa8.~UnkCameraNodeList_l0e1bb4();
    self->ids.~UnkCameraKeyedIds_l0e1bb4();
    self->list_0x50.~UnkCameraNodeList_l0e1bb4();
    fn_71010c4194(self);
}
