// CSamusGunComponent unit (unit 1315 of tools/tu_map.py): on an item change, a change of the current
// missile runs fn_7100859938, and each of eleven item ids makes the gun refresh one of two states
// (its virtual 0x170 or 0x178). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CSamusGunComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkSamusGun_l85a498;

struct UnkSamusGunVtable_l85a498 {
    void* slots[0x2e];
    void (*vf_0x170)(UnkSamusGun_l85a498* self);
    void (*vf_0x178)(UnkSamusGun_l85a498* self);
};

struct UnkSamusGun_l85a498 {
    UnkSamusGunVtable_l85a498* vtable;
};

static inline UnkQualifiedId& Qid_l85a498(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

bool fn_710085a498(UnkSamusGun_l85a498* self, const base::global::CStrId* item) {
    if (*item == Qid_l85a498(g_unk_717275a1d8).id)
        fn_7100859938(reinterpret_cast<UnkSamusGun_b58*>(self));

    if (item->data->Key() == 0x800de7325c041b95)
        self->vtable->vf_0x170(self);
    if (item->data->Key() == 0x4304c637f199bfe9)
        self->vtable->vf_0x170(self);
    if (item->data->Key() == 0x859110f82eedbcd6)
        self->vtable->vf_0x170(self);
    if (item->data->Key() == 0xee0f2cbc2e1fb2a1)
        self->vtable->vf_0x170(self);
    if (item->data->Key() == 0xacde0331796b9622)
        self->vtable->vf_0x170(self);
    if (item->data->Key() == 0x02917b0733cc2cae)
        self->vtable->vf_0x170(self);
    if (item->data->Key() == 0x5c6d9dd64ce576cb)
        self->vtable->vf_0x170(self);
    if (item->data->Key() == 0xd303ff501f26c721)
        self->vtable->vf_0x170(self);

    if (item->data->Key() == 0xf65d40ede5a88588)
        self->vtable->vf_0x178(self);
    if (item->data->Key() == 0x55cc1ae7e7d67f67)
        self->vtable->vf_0x178(self);
    if (item->data->Key() == 0x9409e1193b571675)
        self->vtable->vf_0x178(self);
    return false;
}
