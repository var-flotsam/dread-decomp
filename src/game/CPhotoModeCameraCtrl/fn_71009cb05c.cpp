// CPhotoModeCameraCtrl unit (tools/tu_map.py unit 1490): the controller's activation. Sets a flag in
// the game blackboard, runs the base step, takes its tuning's values, resets its state and registers
// itself with the item list singleton. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CCameraCtrl.h"
#include "game/CInputComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerDef.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2732.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

static inline const UnkQualifiedId& Qid(UnkGameManagerDef& def) {
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}

template <class T>
static inline void SetValue(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                            const base::reflection::CType* (*type)(), const T& value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = type();
            v.value = const_cast<T*>(&value);
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

struct UnkListedItem;
struct UnkItemListTag;
using UnkItemList = base::UnkSList<UnkItemListTag>;
const void* fn_71010e53ac(void* holder);

struct UnkPhotoTuning_l9cb05c {
    u8 pad_0x0[0xe4];
    u32 field_0xe4;
};

struct UnkPhotoHolder_l9cb05c {
    u8 pad_0x0[0x10];
    UnkPhotoTuning_l9cb05c* tuning;
};

struct UnkVec3_l9cb05c {
    u32 x, y, z;
};

struct UnkPhotoModeCameraCtrl_l9cb05c {
    u8 pad_0x0[0x74];
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    u8 pad_0x80[0x1c];
    s32 field_0x9c;
    u8 pad_0xa0[0x10];
    UnkPhotoHolder_l9cb05c* holder;  // 0xb0
    u8 pad_0xb8[0x158];
    u8 field_0x210[0x9b8];
    UnkVec3_l9cb05c field_0xbc8;
    UnkVec3_l9cb05c field_0xbd4;
    u8 field_0xbe0;
    bool field_0xbe1;
    u8 pad_0xbe2[2];
    u32 field_0xbe4;

    void* Tuning() { return holder ? holder->tuning : nullptr; }
};
static_assert(__builtin_offsetof(UnkPhotoModeCameraCtrl_l9cb05c, field_0xbc8) == 0xbc8, "");
static_assert(__builtin_offsetof(UnkPhotoModeCameraCtrl_l9cb05c, field_0xbe4) == 0xbe4, "");

void fn_71009cb05c(UnkPhotoModeCameraCtrl_l9cb05c* self, UnkArg_fc* arg) {
    self->field_0xbe1 = false;
    UnkGameManager* manager = g_unk_7101d47310;
    SetValue(&manager->field_0x968, GameManagerIdTable().field_0x1148,
             Qid(reinterpret_cast<UnkGameManagerDef&>(g_unk_7172758df0)), fn_710000d228, true);
    fn_71010ed98c(reinterpret_cast<UnkFixedCamera_fc*>(self), arg, 1);
    self->field_0xbe4 = self->holder->tuning->field_0xe4;
    if (!fn_7100decc5c(g_unk_7101d47310))
        fn_7100decc74(g_unk_7101d47310);
    self->field_0x7c = -1.0f;
    self->field_0x74 = 0.0f;
    self->field_0x78 = 0.0f;
    const UnkVec3_l9cb05c* a = static_cast<const UnkVec3_l9cb05c*>(fn_71010e537c(self->Tuning()));
    self->field_0xbc8.x = a->x;
    self->field_0xbc8.y = a->y;
    self->field_0xbc8.z = a->z;
    const UnkVec3_l9cb05c* b = static_cast<const UnkVec3_l9cb05c*>(fn_71010e53ac(self->Tuning()));
    self->field_0xbd4.x = b->x;
    self->field_0xbd4.y = b->y;
    self->field_0xbd4.z = b->z;
    if (!g_unk_7172729910) {
        void* p = fn_71000839a0(0x2208, 0x3f);
        fn_710119aee8(p);
        g_unk_7172729910 = p;
    }
    fn_710119ba7c(static_cast<UnkItemList*>(g_unk_7172729910),
                  reinterpret_cast<UnkListedItem*>(self->field_0x210));
    self->field_0x9c = 2;
}
