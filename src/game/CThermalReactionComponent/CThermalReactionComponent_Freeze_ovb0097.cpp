// CThermalReactionComponent's unit (tools/tu_map.py unit 1540): freezing or thawing the owner's components
// (fn_7100a2918c): four of them get the flag through their own setters; the last two are disabled while frozen,
// their enabled flag kept at +0x58 and restored on thawing. Always returns false. Names and layouts are
// placeholders.
#include "types.h"

struct UnkWeakTarget;
struct UnkDoorLock;
struct UnkStruct_710024fa0c;

void fn_710024fa0c(UnkStruct_710024fa0c*, bool);
UnkDoorLock* fn_71002af860(UnkWeakTarget*);

struct UnkThermalComponent_ovb0097 {
    u8 pad_0x0[0x20];
    bool enabled;  // 0x20
};

UnkThermalComponent_ovb0097* fn_7100a29260(UnkWeakTarget* entity);
UnkThermalComponent_ovb0097* fn_7100a2933c(UnkWeakTarget* entity);
UnkThermalComponent_ovb0097* fn_71007ea250(UnkWeakTarget* entity);
UnkThermalComponent_ovb0097* fn_710063337c(UnkWeakTarget* entity);
UnkThermalComponent_ovb0097* fn_7100a290a8(UnkWeakTarget* entity);
void fn_7100a928ec(UnkThermalComponent_ovb0097* component, bool frozen);
void fn_7100a8f448(UnkThermalComponent_ovb0097* component, bool frozen);
void fn_71008d6320(UnkDoorLock* component, bool frozen);
void fn_71007b6b38(UnkThermalComponent_ovb0097* component, bool frozen);

struct UnkThermalReaction_ovb0097 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* owner;  // 0x30
    u8 pad_0x38[0x20];
    u8 was_enabled;  // 0x58
};

static inline void Freeze(UnkThermalReaction_ovb0097* self, UnkThermalComponent_ovb0097* component, bool frozen) {
    if (!component)
        return;
    bool enable;
    if (frozen) {
        self->was_enabled = component->enabled;
        enable = false;
    } else {
        enable = self->was_enabled != 0;
    }
    fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(component), enable);
}

bool fn_7100a2918c(UnkThermalReaction_ovb0097* self, bool frozen) {
    if (UnkThermalComponent_ovb0097* c = fn_7100a29260(self->owner))
        fn_7100a928ec(c, frozen);
    if (UnkThermalComponent_ovb0097* c = fn_7100a2933c(self->owner))
        fn_7100a8f448(c, frozen);
    if (UnkDoorLock* c = fn_71002af860(self->owner))
        fn_71008d6320(c, frozen);
    if (UnkThermalComponent_ovb0097* c = fn_71007ea250(self->owner))
        fn_71007b6b38(c, frozen);
    Freeze(self, fn_710063337c(self->owner), frozen);
    Freeze(self, fn_7100a290a8(self->owner), frozen);
    return false;
}
