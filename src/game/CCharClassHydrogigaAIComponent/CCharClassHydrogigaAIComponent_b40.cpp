// CCharClassHydrogigaAIComponent unit (tools/tu_map.py unit 880): the constructor (fn_71005b981c). The
// boss AI component's base, the vtable, an array of owned objects at +0x528 (allocator flags 4), four
// tuning floats, the members from +0x578 (each by its own constructor), and the flags at +0x409,
// +0x40b, +0x4dc and +0x4fc. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CDamageSourceFactor.h"
#include "game/UnkBossAIComponent.h"
#include "types.h"

struct UnkOwned_hg {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();  // 0x10
};

// An owning pointer: destroying it deletes the object through its vtable.
struct UnkOwnedPtr_hg {
    UnkOwned_hg* ptr;
    ~UnkOwnedPtr_hg() {
        UnkOwned_hg* p = ptr;
        ptr = nullptr;
        if (p)
            p->Delete();
    }
};

extern void* g_unk_7101a50aa0[];
void fn_71005ad514(void* member);
void fn_71005ad8fc(void* member);
void fn_71005af734(void* member);
void fn_71005b23bc(void* member);
void fn_71005b279c(void* member);

struct UnkCharClassHydrogigaAIComponent : UnkBossAIComponent {
    base::UnkVector<UnkOwnedPtr_hg> field_0x528;
    f32 field_0x550[3];
    u8 pad_0x55c[4];
    f32 field_0x560;
    u8 pad_0x564[0x14];
    u8 field_0x578[0x70];
    u8 field_0x5e8[0x70];
    u8 field_0x658[0x70];
    u8 field_0x6c8[0x70];
    u8 field_0x738[0xf8];
    u8 field_0x830[0xf0];
    u8 field_0x920[0x178];
    u8 field_0xa98[0xf0];
    u8 field_0xb88[0x8];
};

void fn_71005b981c(UnkCharClassHydrogigaAIComponent* self) {
    fn_710041a198(self);
    self->vtable = g_unk_7101a50aa0 + 2;
    new (&self->field_0x528) base::UnkVector<UnkOwnedPtr_hg>(4);
    self->field_0x550[0] = 5.0f;
    self->field_0x550[1] = 5.0f;
    self->field_0x550[2] = 5.0f;
    self->field_0x560 = 10.0f;
    fn_71008c2da8(self->field_0x578);
    fn_71008c2da8(self->field_0x5e8);
    fn_71008c2da8(self->field_0x658);
    fn_71008c2da8(self->field_0x6c8);
    fn_71005ad514(self->field_0x738);
    fn_71005ad8fc(self->field_0x830);
    fn_71005af734(self->field_0x920);
    fn_71005b23bc(self->field_0xa98);
    fn_71005b279c(self->field_0xb88);
    self->field_0x409 = 0;
    self->field_0x40b = 1;
    self->field_0x4fc = 0x101;
    reinterpret_cast<u8*>(self)[0x4dc] = 1;
}
