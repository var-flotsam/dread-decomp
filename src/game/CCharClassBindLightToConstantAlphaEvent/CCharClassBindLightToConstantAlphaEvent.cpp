// Default constructors of event classes on the event base (fn_7100067ea4, the base constructor):
// the vtable, a string id (or file-path id) built from "" at +0x10, then the class's own fields.
// One per unit: CCharClassBindLightToConstantAlphaEvent's (a second id and a cleared word) and
// those of the events named at each. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

void fn_7100067ea4(void* self);  // the base constructor

template <class Id, class Tail>
struct UnkIdEvent {
    void** vtable;
    u8 unk_0x8[0x8];
    Id id;
    Tail tail;
};

template <char* Vtable, class Id, class Tail>
__attribute__((always_inline)) static inline void ConstructIdEvent(UnkIdEvent<Id, Tail>* self) {
    fn_7100067ea4(self);
    self->vtable = reinterpret_cast<void**>(Vtable) + 2;
    new (&self->id) Id("", true);
}

// CCharClassBindLightToConstantAlphaEvent: a second id, then a cleared word.
struct UnkIdWordTail {
    CStrId id;
    u32 field_0x20;
};
extern char g_unk_7101aa5560[];

void fn_7100cad998(UnkIdEvent<CStrId, UnkIdWordTail>* self) {
    ConstructIdEvent<g_unk_7101aa5560>(self);
    new (&self->tail.id) CStrId("", true);
    self->tail.field_0x20 = 0;
}

// A cleared word after the id: CCharClassStopEnvironmentStreamEvent (a file-path id) and
// ICharClassFXDeleteEvent.
extern char g_unk_7101aa0418[];
extern char g_unk_7101aa0928[];

void fn_7100b96270(UnkIdEvent<CFilePathStrId, u32>* self) {
    ConstructIdEvent<g_unk_7101aa0418>(self);
    self->tail = 0;
}

void fn_7100bec670(UnkIdEvent<CStrId, u32>* self) {
    ConstructIdEvent<g_unk_7101aa0928>(self);
    self->tail = 0;
}

// A flag after the id: cleared in CCharClassSetSceneGroupEnabledEvent and
// CCharClassHideCutsceneActorEvent, set in CCharClassEnableScenarioItemByNameEvent.
extern char g_unk_7101aa1cc8[];
extern char g_unk_7101aa5bf0[];
extern char g_unk_7101aa58c0[];

void fn_7100bdebc8(UnkIdEvent<CStrId, bool>* self) {
    ConstructIdEvent<g_unk_7101aa1cc8>(self);
    self->tail = false;
}

void fn_7100cc30ac(UnkIdEvent<CStrId, bool>* self) {
    ConstructIdEvent<g_unk_7101aa5bf0>(self);
    self->tail = false;
}

void fn_7100cb94fc(UnkIdEvent<CStrId, bool>* self) {
    ConstructIdEvent<g_unk_7101aa58c0>(self);
    self->tail = true;
}

// CCharClassStopMaterialTransitionEvent: two words, 0 and 1.
struct UnkTwoWordsTail {
    u32 field_0x18;
    u32 field_0x1c;
};
extern char g_unk_7101aa1f38[];

void fn_7100be6a34(UnkIdEvent<CStrId, UnkTwoWordsTail>* self) {
    ConstructIdEvent<g_unk_7101aa1f38>(self);
    self->tail.field_0x18 = 0;
    self->tail.field_0x1c = 1;
}

// CCharClassCameraFXPresetEvent: a cleared 8-byte field and word.
struct UnkWordsTail {
    u64 field_0x18;
    u32 field_0x20;
};
extern char g_unk_7101aa5590[];

void fn_7100cae724(UnkIdEvent<CStrId, UnkWordsTail>* self) {
    ConstructIdEvent<g_unk_7101aa5590>(self);
    self->tail.field_0x18 = 0;
    self->tail.field_0x20 = 0;
}

// CCharClassSetCameraFarEvent: a float of 60000 at +0xc, in the base's words, before the id.
struct UnkCameraFarEvent {
    void** vtable;
    u8 unk_0x8[0x4];
    f32 far_0xc;
    CStrId id;
};
extern char g_unk_7101aa6018[];

void fn_7100cd0ed0(UnkCameraFarEvent* self) {
    fn_7100067ea4(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101aa6018) + 2;
    self->far_0xc = 60000.0f;
    new (&self->id) CStrId("", true);
}
