// Destructors (complete and deleting) of an object in the CCharClassCollisionComponent unit
// (vtable 0x7101a6fd78): its keyed string ids at +0xa0 (fn_710079b944), the string at +0x58, then
// the base destructor fn_71011acd74. Names and layouts are placeholders.
#include <new>

#include "base/UnkKeyedStrIds.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

struct UnkObject_71011ace00;
struct UnkIdObject;
void fn_71011acd74(UnkIdObject* self);
void fn_710079b944(UnkKeyedStrIds* self);
extern void* g_unk_7101a6fd78[];

struct UnkCollisionBase {
    void** vtable;
    u8 unk_0x8[0x50];
    __attribute__((always_inline)) ~UnkCollisionBase() {
        fn_71011acd74(reinterpret_cast<UnkIdObject*>(this));
    }
};

struct UnkCollisionKeyedIds {
    UnkKeyedStrIds ids;
    __attribute__((always_inline)) ~UnkCollisionKeyedIds() { fn_710079b944(&ids); }
};

struct UnkCollisionDef : UnkCollisionBase {
    CRntString name;
    u8 unk_0x88[0x18];
    UnkCollisionKeyedIds keyed;

    __attribute__((always_inline)) ~UnkCollisionDef() { vtable = g_unk_7101a6fd78 + 2; }
};

void fn_710079a998(UnkCollisionDef* self) { self->~UnkCollisionDef(); }
void fn_710079a9dc(UnkCollisionDef* self) {
    self->~UnkCollisionDef();
    operator delete(self);
}
