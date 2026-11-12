// CCharClassHydrogigaAIComponent's unit (tools/tu_map.py unit 880): the destructor (fn_71005b98f0; the
// members of fn_71005b981c in reverse, the owned objects at +0x528, then the base destructor) and a hit
// classifier (fn_71005b9a00: 3 when the collision container's id for the hit object is "polyp", else 7).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassBossAIComponent.h"
#include "game/CCharClassHydrogigaAIComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/UnkBossAIComponent.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;

struct UnkGoliathXBase;
void fn_71005ad544(void* member);
void fn_71005ad92c(void* member);
void fn_71005af7e4(void* member);
void fn_71005b23ec(void* member);
void fn_71005b27cc(void* member);

struct UnkCharClassHydrogigaAI_ovb0078 : UnkBossAIComponent {
    UnkOwnedList field_0x528;
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

void fn_71005b98f0(UnkCharClassHydrogigaAI_ovb0078* self) {
    self->vtable = g_unk_7101a50aa0 + 2;
    fn_71005b27cc(self->field_0xb88);
    fn_71005b23ec(self->field_0xa98);
    fn_71005af7e4(self->field_0x920);
    fn_71005ad92c(self->field_0x830);
    fn_71005ad544(self->field_0x738);
    fn_71008c2de0(self->field_0x6c8);
    fn_71008c2de0(self->field_0x658);
    fn_71008c2de0(self->field_0x5e8);
    fn_71008c2de0(self->field_0x578);
    self->field_0x528.~UnkOwnedList();
    fn_710041a230(reinterpret_cast<UnkGoliathXBase*>(self));
}

struct UnkCollisionContainer;

struct UnkHydrogigaOwner_ovb0078 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkHydrogigaHitTarget_ovb0078 {
    u8 pad_0x0[0x60];
    void* field_0x60;
};

static inline bool SameId(const CStrId& a, const CStrId& b) { return a.data == b.data; }

u32 fn_71005b9a00(void*, UnkHydrogigaOwner_ovb0078* owner, void*, UnkHydrogigaHitTarget_ovb0078* target) {
    if (void* object = target->field_0x60) {
        CStrId* id = fn_7100864018(owner->field_0x2a0, object);
        if (SameId(*id, CStrId("polyp", true)))
            return 3;
    }
    return 7;
}
