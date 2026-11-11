// CCameraComponent's unit (tools/tu_map.py): applies a camera description to the controller its id
// names (offsets, the values at +0xe4/+0x258) and, when asked, makes that controller the current one
// unless it already is and is active. Names and layouts are placeholders.
#include "base/UnkParallelFind.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkCamVec3_l0e5da4 {
    f32 x, y, z;
};

// The description fn_7100df2404 of the game manager finds by name.
struct UnkCamCtrl_ovb5 {
    u8 pad_0x0[0x18];
    CStrId id;                      // 0x18
    bool hasOffsets;                // 0x20
    UnkCamVec3_l0e5da4 field_0x24;  // 0x24
    UnkCamVec3_l0e5da4 field_0x30;  // 0x30
    u32 field_0x3c;
    u8 pad_0x40[0x8];
    f32 field_0x48;
};
static_assert(__builtin_offsetof(UnkCamCtrl_ovb5, field_0x48) == 0x48, "layout");

// A camera controller of the component.
struct UnkCamObject_l0e5da4 {
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
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0(const UnkCamVec3_l0e5da4* value);
    virtual void vf_0xa8(const UnkCamVec3_l0e5da4* value);
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100(UnkCamCtrl_ovb5* ctrl);

    u8 pad_0x8[0x8];
    bool active;  // 0x10
    u8 pad_0x11[0xf];
    CStrId id;  // 0x20
};

// A reference to a controller through its slot: no slot, or a slot that may be empty.
struct UnkCamRef_l0e5da4 {
    UnkCamObject_l0e5da4** slot;

    UnkCamObject_l0e5da4* Get() const { return slot ? *slot : nullptr; }
    bool IsNull() const { return Get() == nullptr; }
};

struct UnkCamComp6_ovb5 {
    u8 pad_0x0[0x60];
    UnkCamRef_l0e5da4 current;  // 0x60
    u8 pad_0x68[0x18];
    UnkCamObject_l0e5da4** values;  // 0x80
    CStrId* ids;                    // 0x88
    u32 count;                      // 0x90
    u8 pad_0x94[0x50];
    u32 field_0xe4;
    u8 pad_0xe8[0x258 - 0xe8];
    f32 field_0x258;

    // The id of the current controller, the empty id without one.
    const CStrId& CurrentId() const {
        if (current.slot == nullptr)
            return g_unk_7101d470e0;
        UnkCamObject_l0e5da4* object = *current.slot;
        return object == nullptr ? g_unk_7101d470e0 : object->id;
    }
};
static_assert(__builtin_offsetof(UnkCamComp6_ovb5, field_0x258) == 0x258, "layout");

// The id table (fn_7100031894 builds it; as src/game/CAbilityEnergyWave/CAbilityEnergyWave_Effect_ovb0022.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x7c8];
    CStrId field_0x7c8;
    u8 pad_0x7d0[0x2578 - 0x7d0];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

static inline bool SameId(const CStrId& a, const CStrId& b) {
    return a.data == b.data;
}

struct UnkCamComp2_ovb5;
struct UnkCamEntry_ovb5;
void fn_71010e41e0(UnkCamComp2_ovb5* self, UnkCamEntry_ovb5* entry, bool flag);

void fn_71010e5da4(UnkCamComp6_ovb5* self, UnkCamCtrl_ovb5* ctrl, bool flag, bool a, bool b) {
    UnkCamObject_l0e5da4* object = UnkParallelFind(self, ctrl->id);
    if (ctrl->hasOffsets && object) {
        object->vf_0xa0(&ctrl->field_0x24);
        object->vf_0xa8(&ctrl->field_0x30);
    }
    self->field_0xe4 = ctrl->field_0x3c;
    if (a)
        self->field_0x258 = ctrl->field_0x48;
    if (object)
        object->vf_0x100(ctrl);

    const CStrId& currentId = self->CurrentId();
    UnkCamObject_l0e5da4* current = self->current.Get();
    bool noCurrent = self->current.IsNull();
    if (!b)
        return;
    // Already current: nothing to do while it is active, except for the table's id at +0x7c8.
    if (!noCurrent && SameId(currentId, ctrl->id)) {
        if (!SameId(currentId, GameManagerIdTable().field_0x7c8) && current->active)
            return;
    }
    UnkCamObject_l0e5da4** slot = UnkParallelFindSlot(self, ctrl->id);
    if (slot != &self->values[self->count] && (*slot)->active)
        fn_71010e41e0(reinterpret_cast<UnkCamComp2_ovb5*>(self), reinterpret_cast<UnkCamEntry_ovb5*>(*slot), flag);
}
