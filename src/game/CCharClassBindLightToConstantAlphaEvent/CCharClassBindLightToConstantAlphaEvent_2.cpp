// Copies of the event default constructors of
// src/game/CCharClassBindLightToConstantAlphaEvent/CCharClassBindLightToConstantAlphaEvent.cpp
// (fn_7100cb94fc, fn_7100cc30ac, fn_7100be6a34, fn_7100cae724), one per event unit, each with its
// own vtable: on the event base (fn_7100067ea4), the vtable, a string id (or file-path id) at +0x10,
// then the class's own fields. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;


template <class Id, class Tail>
struct UnkIdEventB31 {
    void** vtable;
    u8 unk_0x8[0x8];
    Id id;
    Tail tail;
};

template <char* Vtable, class Id, class Tail>
__attribute__((always_inline)) static inline void ConstructIdEventB31(UnkIdEventB31<Id, Tail>* self,
                                                              const char* str = "") {
    fn_7100067ea4(self);
    self->vtable = reinterpret_cast<void**>(Vtable) + 2;
    new (&self->id) Id(str, true);
}

struct UnkTwoWordsTailB31 {
    u32 field_0x18;
    u32 field_0x1c;
};

struct UnkFlagsTailB31 {
    bool field_0x18;
    bool field_0x19;
};

struct UnkFloatsTailB31 {
    f32 field_0x18;
    f32 field_0x1c;
};


// A set flag after the id (as fn_7100cb94fc): CCharClassSetComponentEnabledEvent; a set and a
// cleared flag (fn_7100bede78).
extern char g_unk_7101aa6078[];
extern char g_unk_7101aa0a48[];

void fn_7100cd2854(UnkIdEventB31<CStrId, bool>* self) {
    ConstructIdEventB31<g_unk_7101aa6078>(self);
    self->tail = true;
}

void fn_7100bede78(UnkIdEventB31<CStrId, UnkFlagsTailB31>* self) {
    ConstructIdEventB31<g_unk_7101aa0a48>(self);
    self->tail.field_0x18 = true;
    self->tail.field_0x19 = false;
}

// A cleared flag (as fn_7100cc30ac): CCharClassShowCutsceneActorEvent.
extern char g_unk_7101aa62f0[];

void fn_7100cda574(UnkIdEventB31<CStrId, bool>* self) {
    ConstructIdEventB31<g_unk_7101aa62f0>(self);
    self->tail = false;
}

// Two floats, 0 and -1 or -1000 (shaped as fn_7100be6a34): CCharClassStopLoopSoundEvent (a
// file-path id built from
// " ") and CCharClassThrowSuccubusShitEvent.
extern char g_unk_7101aa6560[];
extern char g_unk_7101aa6620[];

void fn_7100ce3a34(UnkIdEventB31<CFilePathStrId, UnkFloatsTailB31>* self) {
    ConstructIdEventB31<g_unk_7101aa6560>(self, " ");
    self->tail.field_0x18 = 0.0f;
    self->tail.field_0x1c = -1.0f;
}

void fn_7100ce6420(UnkIdEventB31<CStrId, UnkFloatsTailB31>* self) {
    ConstructIdEventB31<g_unk_7101aa6620>(self);
    self->tail.field_0x18 = 0.0f;
    self->tail.field_0x1c = -1000.0f;
}

// Two set flags: CCharClassHideScenarioItemByNameEvent.
extern char g_unk_7101aa5c50[];

void fn_7100cc43f8(UnkIdEventB31<CStrId, UnkFlagsTailB31>* self) {
    ConstructIdEventB31<g_unk_7101aa5c50>(self);
    self->tail.field_0x18 = true;
    self->tail.field_0x19 = true;
}
