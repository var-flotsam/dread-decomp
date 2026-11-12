// CCollisionComponent unit (tools/tu_map.py, unit 1319): clears the pending flags, caches the actor at +0x10 cast to
// CCharClassCollisionComponent's class (+0xc0) and copies its flag at +0x88, hands each record's value at +0x20 to its
// object (slot 0x60), notifies the owner's object at +0x1f0 (slot 0x258), then hooks the callback fn_7100866c4c up to
// the owner's tracked object (+0x220) as the track-link family
// (src/game/CFrozenComponent/CFrozenComponent_Start_A4_b74.cpp). Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

u32 fn_7100866c4c(char* p0, char* p1);

struct UnkCollisionTrackLink_A4_b75 {
    u8 pad_0x0[0x8];
    void* context;
    u32 (*callback)(char*, char*);
};

struct UnkCollisionNotified_A4_b75 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60(u32 value);
};

struct UnkCollisionStart_A4_b75 {
    u8 pad_0x0[0x10];
    UnkReflObject* actor;
    u8 pad_0x18[0x18];
    char* owner;
    u8 pad_0x38[0x10];
    base::UnkVector<UnkCollisionRecord> records;  // 0x48
    u8 pad_0x70[0x50];
    UnkReflObject* cast;  // 0xc0
    u8 pad_0xc8[0x1];
    u8 field_0xc9;
    u16 field_0xca;
    u8 pad_0xcc[0x4];
    u64 field_0xd0;
    u8 pad_0xd8[0x30];
    UnkCollisionTrackLink_A4_b75 link;  // 0x108
    UnkWeakHandle* handle;              // 0x120
    bool field_0x128;
};

void fn_7100860ee0(UnkCollisionStart_A4_b75* self) {
    self->field_0xca = 0;
    self->field_0xd0 = 0;
    UnkReflObject* actor = self->actor;
    UnkReflObject* cast = actor && fn_710009d50c(actor->vf_0x0(), fn_710079a218()) ? actor : nullptr;
    self->cast = cast;
    self->field_0xc9 = *reinterpret_cast<u8*>(reinterpret_cast<char*>(cast) + 0x88);
    for (u32 i = 0; i < self->records.count; ++i)
        reinterpret_cast<UnkCollisionNotified_A4_b75*>(fn_71007253b8(&self->records, i).field_0x28)
            ->vf_0x60(*reinterpret_cast<u32*>(&fn_71007253b8(&self->records, i).field_0x20));
    if (UnkCollisionNotified_A4_b75* o = *reinterpret_cast<UnkCollisionNotified_A4_b75**>(self->owner + 0x1f0))
        (*reinterpret_cast<void (***)(UnkCollisionNotified_A4_b75*)>(o))[0x258 / 8](o);
    UnkCollisionTrackLink_A4_b75* link = &self->link;
    link->context = self;
    link->callback = fn_7100866c4c;
    UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    self->field_0x128 = false;
}
