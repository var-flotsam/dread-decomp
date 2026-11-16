// CGrabComponent unit (tools/tu_map.py unit 2709): the component's destructor. Takes the component
// out of the registry of grabbing components, unregisters the object at +0x1e0, then destroys the
// members (the grab id, the grab description at +0xf8, four counted links, three arrays whose first
// one's entries point to slots that are cleared) and the component base. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkSList.h"
#include "base/global/CRntVector.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CChozoCommanderKiGrabAttack.h"
#include "game/CGrabComponent.h"
#include "game/UnkComponent.h"
#include "types.h"


using UnkGrabNode_l13b174 = UnkGrabList_ovb0057::Node;

static inline s32 IndexOf(UnkGrabList_ovb0057* list, u64 value) {
    u32 i = 0;
    for (UnkGrabNode_l13b174* n = list->head; n; n = n->next, ++i) {
        if (n->value == value)
            return static_cast<s32>(i);
    }
    return -1;
}

static inline UnkGrabNode_l13b174* Find(UnkGrabList_ovb0057* list, u64 value) {
    for (UnkGrabNode_l13b174* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkGrabList_ovb0057* list, UnkGrabNode_l13b174* node) {
    UnkGrabNode_l13b174* prev = nullptr;
    UnkGrabNode_l13b174* it = list->head;
    while (it) {
        UnkGrabNode_l13b174* cur = it;
        UnkGrabNode_l13b174* next = it->next;
        it = next;
        if (cur != node) {
            prev = cur;
            continue;
        }
        if (prev) {
            prev->next = next;
            if (list->tail == node)
                list->tail = prev;
        } else {
            list->head = next;
        }
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        list->allocator->vf_0x48(node);
        return;
    }
}

// A counted link to a grabbed or grabbing actor; dropping the last reference releases it.
struct UnkGrabLink_l13b174 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
};

struct UnkGrabLinkRef_l13b174 {
    UnkGrabLink_l13b174* link;

    ~UnkGrabLinkRef_l13b174() {
        if (UnkGrabLink_l13b174* l = link) {
            if (--l->ref_count == 0)
                l->Release();
        }
    }
};

struct UnkGrabComponent_l13b174 {
    void** vtable;
    u8 pad_0x8[0x38];
    base::global::CRntVector<void**> lists[3];  // 0x40
    u8 pad_0xb8[0x10];
    UnkGrabLinkRef_l13b174 field_0xc8;
    UnkGrabLinkRef_l13b174 field_0xd0;
    UnkGrabLinkRef_l13b174 field_0xd8;
    u8 pad_0xe0[0x10];
    UnkGrabLinkRef_l13b174 grabbed;  // 0xf0
    u8 info[0xd8];      // 0xf8: the grab description (fn_7100431cec destroys it)
    base::UnkStrIdMember field_0x1d0;
    u8 pad_0x1d8[8];
    void* field_0x1e0;
    u64 field_0x1e8;
    u64 field_0x1f0;
};
static_assert(__builtin_offsetof(UnkGrabComponent_l13b174, grabbed) == 0xf0, "layout");
static_assert(__builtin_offsetof(UnkGrabComponent_l13b174, field_0x1d0) == 0x1d0, "layout");
static_assert(__builtin_offsetof(UnkGrabComponent_l13b174, field_0x1f0) == 0x1f0, "layout");

void fn_710113b174(UnkGrabComponent_l13b174* self) {
    self->vtable = g_unk_7101cb3230 + 2;

    UnkGrabList_ovb0057* registry = &g_unk_7172a090b8;
    u64 key = reinterpret_cast<u64>(self);
    if (IndexOf(registry, key) >= 0) {
        if (UnkGrabNode_l13b174* node = Find(registry, key))
            Erase(registry, node);
    }

    if (self->field_0x1e0) {
        fn_71004567dc(self->field_0x1e0, &self->field_0x1e0);
        self->field_0x1e0 = nullptr;
    }
    self->field_0x1e8 = 0;
    self->field_0x1f0 = 0;

    using base::UnkStrIdMember;
    self->field_0x1d0.~UnkStrIdMember();
    fn_7100431cec(self->info);
    self->grabbed.~UnkGrabLinkRef_l13b174();
    self->field_0xd8.~UnkGrabLinkRef_l13b174();
    self->field_0xd0.~UnkGrabLinkRef_l13b174();
    self->field_0xc8.~UnkGrabLinkRef_l13b174();
    base::global::CRntVector<void**>& links = self->lists[0];
    for (void*** it = links.data, ***end = links.data + links.count; it != end; ++it)
        **it = nullptr;
    for (base::global::CRntVector<void**>& list : self->lists)
        list.Clear();
    self->lists[2].Free();
    self->lists[1].Free();
    self->lists[0].Free();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
