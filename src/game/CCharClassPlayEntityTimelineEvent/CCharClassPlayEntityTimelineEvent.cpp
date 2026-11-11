// Copy constructors of event classes directly over a base copied out of line (fn_7100067ebc),
// two identical copies per class. Here: the class's vtable, then two string ids from 0x10 with
// their references (the shape of fn_7100ce0aa4 in
// src/game/CCharClassCollisionMaterialFXSetActiveEvent/fn_7100ba22f0.cpp), in
// CCharClassPlayEntityTimelineEvent's unit and those of CCharClassShowTutorialMessageEvent,
// CCharClassLogicSoundEvent and CCharClassStaticLUACallback1Event; further down, two more shapes.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "game/CCharClassLogicSoundEvent.h"
#include "game/CCharClassPlayEntityTimelineEvent.h"
#include "game/CCharClassSetMaterialPropertyEvent.h"
#include "game/CCharClassShowTutorialMessageEvent.h"
#include "game/CCharClassStaticLUACallback1Event.h"
#include "types.h"

using base::global::CStrId;

struct UnkTwoIdsEvent {
    char* vtable;
    u8 unk_0x8[0x8];
    CStrId ids[2];
};

template <char* Vtable>
__attribute__((always_inline)) static inline void CopyTwoIds(UnkTwoIdsEvent* self, const UnkTwoIdsEvent* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = Vtable + 0x10;
    for (u32 i = 0; i < 2; ++i)
        new (&self->ids[i]) CStrId(other->ids[i]);
}

#define COPY_TWO_IDS(addr, vtable) \
    void fn_##addr(UnkTwoIdsEvent* self, const UnkTwoIdsEvent* other) { CopyTwoIds<vtable>(self, other); }

COPY_TWO_IDS(7100bc58f8, g_unk_7101aa1648)
COPY_TWO_IDS(7100bc5974, g_unk_7101aa1648)
COPY_TWO_IDS(7100be1148, g_unk_7101aa1d88)
COPY_TWO_IDS(7100be11c4, g_unk_7101aa1d88)
COPY_TWO_IDS(7100cc649c, g_unk_7101aa5d10)
COPY_TWO_IDS(7100cc6518, g_unk_7101aa5d10)
COPY_TWO_IDS(7100cdfd00, g_unk_7101aa6410)
COPY_TWO_IDS(7100cdfd7c, g_unk_7101aa6410)

// A class on a middle class (vtable 0x7101aa06b8) of 0x14 plain bytes from 0xc, then the
// class's vtable: CCharClassUsableFadeInEvent's and CCharClassFadeInEvent's units.
extern char g_unk_7101aa06b8[];
extern char g_unk_7101aa2058[];
extern char g_unk_7101aa5920[];

struct UnkFadeInEvent {
    char* vtable;
    u8 unk_0x8[0x4];
    u8 data[0x14];
};

template <char* Vtable>
__attribute__((always_inline)) static inline void CopyFadeIn(UnkFadeInEvent* self, const UnkFadeInEvent* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = g_unk_7101aa06b8 + 0x10;
    __builtin_memcpy(self->data, other->data, sizeof(self->data));
    self->vtable = Vtable + 0x10;
}

void fn_7100be9944(UnkFadeInEvent* self, const UnkFadeInEvent* other) { CopyFadeIn<g_unk_7101aa2058>(self, other); }
void fn_7100be99a0(UnkFadeInEvent* self, const UnkFadeInEvent* other) { CopyFadeIn<g_unk_7101aa2058>(self, other); }
void fn_7100cba158(UnkFadeInEvent* self, const UnkFadeInEvent* other) { CopyFadeIn<g_unk_7101aa5920>(self, other); }
void fn_7100cba1b4(UnkFadeInEvent* self, const UnkFadeInEvent* other) { CopyFadeIn<g_unk_7101aa5920>(self, other); }

// A string id at 0x10, then 0x1c plain bytes: CCharClassSetMaterialPropertyEvent's unit.

struct UnkIdBytesEvent {
    char* vtable;
    u8 unk_0x8[0x8];
    CStrId id;
    u8 data[0x1c];
};

__attribute__((always_inline)) static inline void CopyIdBytes(UnkIdBytesEvent* self, const UnkIdBytesEvent* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = g_unk_7101aa6108 + 0x10;
    new (&self->id) CStrId(other->id);
    __builtin_memcpy(self->data, other->data, sizeof(self->data));
}

void fn_7100cd49fc(UnkIdBytesEvent* self, const UnkIdBytesEvent* other) { CopyIdBytes(self, other); }
void fn_7100cd4a74(UnkIdBytesEvent* self, const UnkIdBytesEvent* other) { CopyIdBytes(self, other); }
