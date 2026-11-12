// CCameraComponent's unit (tools/tu_map.py, the name from reflection). The component's target
// (+0xc48, a weak handle to the object whose +0xc4 it sets) with its timer (+0xc40, counted down to
// -1) and the values at +0x25c/+0x264; the lookup of an entry by id in the parallel arrays at
// +0x80/+0x88/+0x90 (with fn_71010e41e0 when the entry's flag is set); the clean-up of the
// listener list (+0xa8), the entries and the counted object at +0xc38; the depth margin to the
// player (zero depth while its character class tracks CCharClassForceCameraDetphEntityPositionZeroTrack);
// the switch to the camera controller a name gives (fn_7100df2404 of the game manager, then
// fn_71010e5da4), also as a reflected function; the CAnimatedCameraCtrl entry under the id table's
// id at +0x7c8 (fn_71010ea4fc); and the shutdown step that unregisters +0x280 and drops the entries
// and their ids. Names and layouts are placeholders.
#include "base/UnkParallelFind.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAnimatedCameraCtrl.h"
#include "game/CCameraComponent.h"
#include "game/CComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2732.h"
#include "game/UnkTU39.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkCamTarget_ovb5 {
    u8 pad_0x0[0xc4];
    f32 field_0xc4;
};

struct UnkCamSlot_ovb5 {
    u8 pad_0x0[0x10];
    UnkCamTarget_ovb5* target;
};

struct UnkCamComp_ovb5 {
    u8 pad_0x0[0x25c];
    f32 field_0x25c;
    u8 pad_0x260[0x4];
    f32 field_0x264;
    u8 pad_0x268[0xc40 - 0x268];
    f32 field_0xc40;
    u8 pad_0xc44[0x4];
    UnkCamSlot_ovb5* slot;  // 0xc48

    UnkCamTarget_ovb5* Current() { return slot ? slot->target : nullptr; }
};
static_assert(__builtin_offsetof(UnkCamComp_ovb5, slot) == 0xc48, "layout");

void fn_71010e6470(UnkCamComp_ovb5* self, UnkCamTarget_ovb5* target, bool both, f32 a, f32 b) {
    if (self->Current() != target)
        return;
    self->field_0xc40 = -1.0f;
    target->field_0xc4 = a;
    if (both)
        self->field_0x25c = a;
    self->field_0x264 = b;
}

bool fn_71010e63ec(UnkCamComp_ovb5* self, UnkCamTarget_ovb5* target, f32 a, f32, f32 dt) {
    if (self->Current() != target)
        return false;
    f32 t = self->field_0xc40 - dt;
    self->field_0xc40 = !(t <= -1.0f) ? t : -1.0f;
    if (self->field_0xc40 > 0.0f)
        return false;
    if (self->Current() == target) {
        self->field_0xc40 = -1.0f;
        target->field_0xc4 = a;
        self->field_0x264 = -1.0f;
    }
    return true;
}

struct UnkCamListener_ovb5 {
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
    virtual void vf_0x60();
    virtual void vf_0x68();
};

struct UnkCamEntry_ovb5 : UnkCamListener_ovb5 {
    u8 pad_0x8[0x8];
    bool field_0x10;
};

struct UnkRefCounted_ovb5 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 refcount;
};

struct UnkCamListTag_ovb5;

struct UnkCamComp2_ovb5 {
    u8 pad_0x0[0x80];
    UnkCamEntry_ovb5** values;  // 0x80
    CStrId* ids;               // 0x88
    u32 count;                 // 0x90
    u8 pad_0x94[0x14];
    base::UnkSList<UnkCamListTag_ovb5> listeners;  // 0xa8
    u8 pad_0xd8[0xc38 - 0xd8];
    UnkRefCounted_ovb5* field_0xc38;
};
static_assert(__builtin_offsetof(UnkCamComp2_ovb5, field_0xc38) == 0xc38, "layout");

void fn_71010e4168(UnkCamComp2_ovb5* self, const CStrId* id, bool flag) {
    UnkCamEntry_ovb5** slot = UnkParallelFindSlot(self, *id);
    if (slot != &self->values[self->count] && (*slot)->field_0x10)
        fn_71010e41e0(self, *slot, flag);
}

void fn_71010e20b0(UnkCamComp2_ovb5* self) {
    using List = base::UnkSList<UnkCamListTag_ovb5>;
    List& list = self->listeners;
    for (List::Node* it = list.head; it; it = it->next)
        reinterpret_cast<UnkCamListener_ovb5*>(it->value)->vf_0x48();
    for (List::Node* it = list.head; it;) {
        List::Node* next = it->next;
        list.allocator->vf_0x48(it);
        it = next;
    }
    list.count = 0;
    list.head = list.tail = nullptr;
    for (UnkCamEntry_ovb5** it = self->values; it != self->values + self->count; ++it)
        (*it)->vf_0x68();
    if (UnkRefCounted_ovb5* p = self->field_0xc38) {
        if (--p->refcount == 0)
            p->vf_0x10();
        self->field_0xc38 = nullptr;
    }
}

struct UnkPlayer;
struct UnkDropCameraVec3;

struct UnkCamComp3_ovb5 {
    u8 pad_0x0[0x4c];
    f32 field_0x4c;
    u8 pad_0x50[0x278 - 0x50];
    f32 field_0x278;
};

struct UnkVec3_ovb5 {
    f32 x, y, z;
};

struct UnkPlayerView_ovb5 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

f32 fn_71010e49c8(UnkCamComp3_ovb5* self, UnkPlayer* target, const UnkVec3_ovb5* pos, f32 margin) {
    if (!(self->field_0x4c < 0.0f))
        return self->field_0x4c;
    if (!target)
        return 0.0f;
    if (!(margin >= 0.0f))
        margin = self->field_0x278;
    f32 z = reinterpret_cast<const UnkVec3_ovb5*>(fn_7100d7c9f4(target))->z;
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
        if (UnkAnimationComponent* cls = reinterpret_cast<UnkPlayerView_ovb5*>(player)->field_0x220) {
            if (fn_710117b348(cls, fn_7100cfa6b8()))
                z = 0.0f;
        }
    }
    f32 d = __builtin_fabsf(z - pos->z) - margin;
    return !(d <= 1.0f) ? d : 1.0f;
}

struct UnkIdTable_ovb5 {
    UnkIdTable_ovb5();  // fn_7100031894
    ~UnkIdTable_ovb5();
    u8 pad_0x0[0x7c8];
    CStrId field_0x7c8;
};

inline UnkIdTable_ovb5& IdTable_ovb5() {
    static UnkIdTable_ovb5 s_table;
    return s_table;
}

struct UnkCamComp7_ovb5 {
    u8 pad_0x0[0x80];
    UnkCameraCtrl** values;  // 0x80
    CStrId* ids;             // 0x88
    u32 count;               // 0x90
};

void fn_71010e51e4(UnkCamComp7_ovb5* self) {
    UnkCameraCtrl* ctrl = UnkParallelFind(self, IdTable_ovb5().field_0x7c8);
    if (ctrl && ctrl->vf_0x0() == fn_71010e7f90())
        fn_71010ea4fc(ctrl);
}

struct UnkCamCtrl_ovb5;
struct UnkCamComp6_ovb5;

void fn_71010e5da4(UnkCamComp6_ovb5* self, UnkCamCtrl_ovb5* ctrl, bool flag, bool a, bool b);

struct UnkCamBuiltId_ovb5 {
    union {
        CStrId id;
    };

    explicit UnkCamBuiltId_ovb5(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkCamBuiltId_ovb5() { id.~CStrId(); }
};

void fn_71010e64a4(UnkCamComp6_ovb5* self, const CRntString& name, bool flag) {
    UnkCamCtrl_ovb5* ctrl = fn_7100df2404(g_unk_7101d47310, UnkCamBuiltId_ovb5(name).id);
    if (ctrl)
        fn_71010e5da4(self, ctrl, flag, true, true);
}

static inline __attribute__((always_inline)) void* ArgPtr_ovb5(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

void fn_71010e76f0(char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* target = *reinterpret_cast<UnkCamComp6_ovb5**>(obj + 8);
    const CRntString* name = static_cast<const CRntString*>(ArgPtr_ovb5(&a[0]));
    const u8* flag = static_cast<const u8*>(ArgPtr_ovb5(&a[1]));
    fn_71010e64a4(target, *name, *flag);
}

struct UnkCamTarget2_ovb5 : UnkWeakTarget {
    u8 pad_0x8[0xb8];
    f32 field_0xc0;
    f32 field_0xc4;
};

struct UnkCamComp8_ovb5 {
    u8 pad_0x0[0x25c];
    f32 field_0x25c;
    u8 pad_0x260[0x4];
    f32 field_0x264;
    u8 pad_0x268[0xc40 - 0x268];
    f32 field_0xc40;
    u8 pad_0xc44[0x4];
    UnkWeakHandle* target;  // 0xc48
};
static_assert(__builtin_offsetof(UnkCamComp8_ovb5, target) == 0xc48, "layout");

void fn_71010e6278(UnkCamComp8_ovb5* self, UnkCamTarget2_ovb5* target, f32 a, f32 b, f32 time) {
    UnkCamTarget2_ovb5* cur = self->target ? static_cast<UnkCamTarget2_ovb5*>(self->target->target) : nullptr;
    if (cur && cur != target)
        cur->field_0xc4 = cur->field_0xc0;
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    AssignRef(&self->target, handle);
    self->field_0xc40 = time;
    target->field_0xc4 = a;
    if (b < 0.0f)
        self->field_0x25c = a;
    self->field_0x264 = b;
}

struct UnkHandleOwner_71010c4230;

struct UnkCamObj_ovb5 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
};

struct UnkCamComp9_ovb5 {
    u8 pad_0x0[0x40];
    char* field_0x40;  // a scene object; its item is at +8
    u8 pad_0x48[0x18];
    UnkCamObj_ovb5** field_0x60;  // the first controller's slot
    u8 pad_0x68[0x18];
    UnkCamObj_ovb5** values;  // 0x80
    CStrId* ids;              // 0x88
    u32 count;                // 0x90
    u8 pad_0x94[0x5c];
    u8 field_0xf0[0xb8];
    u8 field_0x1a8[0xd8];
    u8 field_0x280[0x8];
};
static_assert(__builtin_offsetof(UnkCamComp9_ovb5, field_0x280) == 0x280, "layout");

void fn_71010e1ef4(UnkCamComp9_ovb5* self) {
    if (!g_unk_7172729910) {
        void* p = fn_71000839a0(0x2208, 0x3f);
        fn_710119aee8(p);
        g_unk_7172729910 = p;
    }
    fn_710119ae24(g_unk_7172729910, self->field_0x280, true);
    if (self->field_0x60) {
        if (UnkCamObj_ovb5* first = *self->field_0x60)
            first->vf_0x38();
    }
    UnkRenderManager* render = g_unk_7101d47300;
    char* object = self->field_0x40;
    fn_71002199d8(render, reinterpret_cast<UnkItemBase_71011a91c4*>(object ? object + 8 : nullptr), 0);
    self->field_0x40 = nullptr;
    for (UnkCamObj_ovb5** it = self->values; it != self->values + self->count; ++it) {
        if (*it)
            (*it)->vf_0x10();
    }
    if (self->count && self->ids) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].Reset();
    }
    self->count = 0;
    fn_7100030ce0(self->field_0xf0);
    fn_7100030ce0(self->field_0x1a8);
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
}
