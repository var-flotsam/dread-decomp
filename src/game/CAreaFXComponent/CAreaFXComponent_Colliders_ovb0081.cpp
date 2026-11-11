// CAreaFXComponent unit (tools/tu_map.py unit 1263): the colliders in the area. fn_71007fee9c collects
// them from the owner's collision container (+0x2a0, fn_710086493c with flags 0x180) into the list at
// +0x438 and hands each CCollider / CColliderGroup to its owner; fn_7100801fa4 hands them to the
// scenario's tracker (+0x820) and drops the list's references. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkStruct_7100861148_Collider;

void fn_7100d5abfc(void* owner, UnkStruct_7100861148_Collider* collider);
struct UnkTracker_71008618cc;
void fn_7100d59994(UnkTracker_71008618cc*, UnkReflObject*);
void fn_7100d5a304(UnkTracker_71008618cc*, UnkReflObject*);

struct UnkAreaFXCollider_ovb0081 : UnkReflObject {
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(u32 state);

    u8 pad_0x8[0x28];
    void* field_0x30;
};

struct UnkAreaFXEntry_ovb0081 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 ref_count;
    UnkAreaFXCollider_ovb0081* collider;  // 0x10
};

struct UnkAreaFXEntries_ovb0081 {
    UnkAreaFXEntry_ovb0081** data;
    u32 count;
};

struct UnkAreaFXOwner_ovb0081 {
    u8 pad_0x0[0x2a0];
    void* field_0x2a0;
};

struct UnkAreaFXColl_ovb0081 {
    u8 pad_0x0[0x30];
    UnkAreaFXOwner_ovb0081* owner;
    u8 pad_0x38[0x400];
    UnkAreaFXEntries_ovb0081 entries;  // 0x438
};

void fn_710086493c(void* container, u32 flags, UnkAreaFXEntries_ovb0081* out);

void fn_71007fee9c(UnkAreaFXColl_ovb0081* self) {
    void* container = self->owner->field_0x2a0;
    if (!container)
        return;
    fn_710086493c(container, 0x180, &self->entries);
    for (u64 i = 0; i < self->entries.count; ++i) {
        UnkAreaFXEntry_ovb0081* entry = self->entries.data[i];
        if (!entry)
            continue;
        UnkAreaFXCollider_ovb0081* c = entry->collider;
        if (!c)
            continue;
        void* target = c->field_0x30;
        bool is_collider = fn_710009d50c(c->vf_0x0(), fn_7100d361c0());
        UnkAreaFXCollider_ovb0081* collider = is_collider ? c : nullptr;
        if (target && is_collider) {
            fn_7100d5abfc(target, reinterpret_cast<UnkStruct_7100861148_Collider*>(collider));
            collider->vf_0x40(4);
            continue;
        }
        bool is_group = fn_710009d50c(c->vf_0x0(), fn_7100d454fc());
        if (target && is_group)
            fn_7100d5ac50(target, reinterpret_cast<UnkStruct_7100861148_Collider*>(c));
    }
}

void fn_7100801fa4(UnkAreaFXColl_ovb0081* self) {
    if (UnkGameManagerSub_0x608* scenario = g_unk_7101d47310->field_0x608) {
        if (UnkTracker_71008618cc* tracker = static_cast<UnkTracker_71008618cc*>(scenario->field_0x820)) {
            for (u64 i = 0; i < self->entries.count; ++i) {
                UnkAreaFXEntry_ovb0081* entry = self->entries.data[i];
                if (!entry)
                    continue;
                UnkAreaFXCollider_ovb0081* c = entry->collider;
                if (!c)
                    continue;
                if (fn_710009d50c(c->vf_0x0(), fn_7100d361c0()))
                    fn_7100d59994(tracker, c);
                else if (fn_710009d50c(c->vf_0x0(), fn_7100d454fc()))
                    fn_7100d5a304(tracker, c);
            }
        }
    }
    for (u64 i = 0; i < self->entries.count; ++i) {
        if (UnkAreaFXEntry_ovb0081* entry = self->entries.data[i]) {
            u32* count = &entry->ref_count;
            if (--*count == 0)
                entry->vf_0x10();
        }
    }
    self->entries.count = 0;
}
