// CMapAcquisitionComponent unit (tools/tu_map.py): fn_7100a862b0 handles message 1: it hands the map menu a
// callback (fn_7100a88628), sets its acquired flag (+0x539), stores it under the unit's property in its
// blackboard section, and drops its timeline listener (+0x820). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CTimelineComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkMapMenuController.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

extern UnkPropertyEx<> g_unk_7172750dd0;  // the unit's property

struct UnkMapAcquisition_la862b0;
void fn_7100a88628(UnkMapAcquisition_la862b0* self);

// A callback: the object and the function to call with it.
struct UnkMapCallback_la862b0 {
    UnkMapAcquisition_la862b0* object;
    void (*fn)(UnkMapAcquisition_la862b0*);
};
void fn_7100b433b8(UnkMapMenuController* menu, UnkMapCallback_la862b0* callback);

struct UnkMapMenuRef_la862b0 {
    u8 pad_0x0[0x10];
    UnkMapMenuController* target;
};

struct UnkMapWorld_la862b0 {
    u8 pad_0x0[0xd63f40];
    UnkMapMenuRef_la862b0* menu;  // 0xd63f40
};

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkMapLink_la862b0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

struct UnkMapListener_la862b0 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkMapLink_la862b0* link;
};

struct UnkMapMessage_la862b0 {
    s32 type;
};

struct UnkMapAcquisition_la862b0 {
    void** vtable;
    u8 pad_0x8[0x531];
    bool acquired;  // 0x539
    u8 pad_0x53a[0x820 - 0x53a];
    UnkMapListener_la862b0 listener;  // 0x820

    // The blackboard section the component stores its state in.
    CStrId SectionId() {
        return reinterpret_cast<CStrId (*)(UnkMapAcquisition_la862b0*)>(vtable[0x2d8 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkMapAcquisition_la862b0, listener) == 0x820, "layout");

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Stores `value` under `name` in the section `section` names (unless the blackboard is locked).
static inline void SetBool(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                           const bool& value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = const_cast<bool*>(&value);
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

bool fn_7100a862b0(UnkMapAcquisition_la862b0* self, const UnkMapMessage_la862b0* message) {
    if (message->type != 1)
        return true;
    UnkMapMenuRef_la862b0* ref = static_cast<UnkMapWorld_la862b0*>(g_unk_7101d47310->field_0x2228)->menu;
    UnkMapMenuController* menu = ref ? ref->target : nullptr;
    UnkMapCallback_la862b0 callback = {self, fn_7100a88628};
    fn_7100b433b8(menu, &callback);
    self->acquired = true;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section = self->SectionId();
        SetBool(blackboard, section, GetQualifiedId(g_unk_7172750dd0), self->acquired);
    }
    // Unregister from the timeline the listener is linked to and let go of the link.
    UnkMapLink_la862b0* link = self->listener.link;
    if (!link)
        return false;
    if (void* owner = link->owner) {
        fn_7100a2d4d0(owner, &self->listener);
        link = self->listener.link;
        if (!link)
            return false;
    }
    if (--link->ref_count == 0)
        link->Release();
    self->listener.link = nullptr;
    return false;
}
