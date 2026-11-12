// Unit 1444 of tools/tu_map.py (see src/game/UnkUnit1444/): the class built on the base of unit 1440
// (fn_7100969000): its constructor, the message handler that forwards to the member at +0x90, and
// the step run when the pending flag at +0x203 is set. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/CAnimationComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU1130.h"
#include "game/UnkTU1440.h"
#include "game/UnkTU1444.h"
#include "game/UnkTU376.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

struct UnkUnit1444Msg_ovb0177;
struct UnkTrackSelf_b0143;
struct UnkGroundAI_ovb0146;

struct UnkUnit1441Apply_A4_b74;
struct UnkUnit1441Msg_A4_b74;
void fn_7100707ef0(void* member, const UnkUnit1444Msg_ovb0177* msg, bool reset);
bool fn_71010faac0(UnkTrackSelf_b0143* self);
void fn_7101110030(UnkGroundAI_ovb0146* self);

struct UnkUnit1444Msg_ovb0177 {
    u8 pad_0x0[0x38];
    u32 kind;  // 0x38
};

struct UnkOwner_ovb0177 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x18];
    void* field_0x240;
    u8 pad_0x248[0x50];
    void* field_0x298;
    UnkCollisionContainer* field_0x2a0;
    u8 pad_0x2a8[0x20];
    void* field_0x2c8;
};

struct UnkEntry_ovb0177 {
    virtual ~UnkEntry_ovb0177();
    u8 pad_0x8[0x10];
};

struct UnkMember_ovb0177 {
    void* vtable;
    u8 pad_0x8[4];
    bool field_0xc;
    u8 pad_0xd[7];
    bool field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 mode;  // 0x38
    u8 pad_0x3c[4];
    u8 object[0x100];  // 0x40
};
static_assert(sizeof(UnkMember_ovb0177) == 0x140, "UnkMember_ovb0177 layout");

struct UnkUnit1444_ovb0177 {
    void* vtable;
    u8 pad_0x8[0x28];
    UnkOwner_ovb0177* owner;  // 0x30
    void* field_0x38;
    u8 pad_0x40[0x50];
    UnkMember_ovb0177 member;  // 0x90
    bool field_0x1d0;
    u8 pad_0x1d1;
    u8 field_0x1d2;
    u8 pad_0x1d3;
    u8 field_0x1d4;
    u8 pad_0x1d5[3];
    u8 field_0x1d8[0x25];
    bool field_0x1fd;
    bool field_0x1fe;
    u8 pad_0x1ff;
    bool field_0x200;
    bool field_0x201;
    bool field_0x202;
    bool field_0x203;
    u8 field_0x204[2];
    u8 pad_0x206[2];
    base::UnkVector<UnkEntry_ovb0177> entries;  // 0x208
};
static_assert(__builtin_offsetof(UnkUnit1444_ovb0177, entries) == 0x208, "UnkUnit1444_ovb0177 layout");

void fn_710097170c(UnkUnit1444_ovb0177* self, const UnkUnit1444Msg_ovb0177* msg, bool reset) {
    fn_71009690a0(reinterpret_cast<UnkUnit1441Apply_A4_b74*>(self), reinterpret_cast<const UnkUnit1441Msg_A4_b74*>(msg), reset);
    if (msg->kind == 0 || msg->kind == 4)
        fn_7100707ef0(&self->member, msg, reset);
}

void fn_71009738ec(UnkUnit1444_ovb0177* self);
struct UnkUnit1444_Event;
void fn_7100973f30(UnkUnit1444_ovb0177* self);

// Tells the owner's AI about the pending state: the kind is 2 when the stored object's byte at +0x1c is
// set, else 1 (slot 0x5a8).
static inline void SendKind(UnkUnit1444_ovb0177* self, void* ai, u32 flag = 0) {
    alignas(8) u8 raw[0x100];
    UnkObject_0x100* tmp = reinterpret_cast<UnkObject_0x100*>(raw);
    fn_71002b57bc(tmp);
    if (self->field_0x1d0) {
        fn_71002b5c90(tmp, reinterpret_cast<const UnkObject_0x100*>(self->member.object));
        if (self->field_0x1d0) {
            u32 kind = raw[0x1c] != 0xff ? 2 : 1;
            reinterpret_cast<void (*)(void*, u32*, u32)>((*reinterpret_cast<void***>(ai))[0x5a8 / 8])(ai, &kind, flag);
        }
    }
    fn_71002b6094(tmp);
}

void fn_7100974508(UnkUnit1444_ovb0177* self) {
    void* ai = self->owner->field_0x240;
    if (ai) {
        if (!fn_71010faac0(reinterpret_cast<UnkTrackSelf_b0143*>(ai)))
            fn_7101110030(reinterpret_cast<UnkGroundAI_ovb0146*>(self->owner->field_0x240));
    }
    if (self->field_0x203) {
        self->field_0x200 = true;
        self->field_0x201 = true;
        fn_71009738ec(self);
        UnkOwner_ovb0177* owner = self->owner;
        self->field_0x201 = false;
        if (void* target = owner->field_0x240) {
            SendKind(self, target);
            owner = self->owner;
        }
        void* poked = owner->field_0x298;
        if (poked)
            reinterpret_cast<void (*)(void*, f32)>((*reinterpret_cast<void***>(poked))[0x98 / 8])(poked, 0.0f);
    }
}

void fn_7100973e28(UnkUnit1444_ovb0177* self) {
    fn_7100969148(self);
    UnkOwner_ovb0177* owner = self->owner;
    if (owner->field_0x220) {
        if (fn_7101179c3c(owner->field_0x220, 0xf4061ff25a2163d6))
            return;
        owner = self->owner;
    }
    if (owner->field_0x2c8) {
        if (fn_71007f9f20(owner->field_0x2c8))
            return;
    }
    bool ready = self->field_0x1fe;
    self->field_0x1fd = false;
    if (ready || (self->field_0x1fe = fn_7100971ee8(reinterpret_cast<UnkUnit1444_Event*>(self))))
        fn_7100973f30(self);
    if (self->field_0x201) {
        fn_71009738ec(self);
        UnkOwner_ovb0177* now = self->owner;
        self->field_0x201 = false;
        if (void* target = now->field_0x240)
            SendKind(self, target);
    }
}

// A 0x100-byte object built by fn_71002b57bc and destroyed by fn_71002b6094.
struct UnkTempObject_ovb0177 {
    alignas(8) u8 raw[0x100];

    UnkTempObject_ovb0177() { fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(raw)); }
    ~UnkTempObject_ovb0177() { fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(raw)); }
};

// The unit normal of the cell side the stored object names, or straight up when there is no mesh or
// the object names no side.
UnkVec3f fn_71009737e8(UnkUnit1444_ovb0177* self) {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    UnkNavMesh* mesh = manager->navmesh;
    if (mesh) {
        UnkTempObject_ovb0177 stored;
        if (self->field_0x1d0)
            fn_71002b5c90(reinterpret_cast<UnkObject_0x100*>(stored.raw),
                          reinterpret_cast<const UnkObject_0x100*>(self->member.object));
        if (stored.raw[0x1c] != 0xff) {
            UnkNavMeshSideSegment segment;
            segment.start.x = 0.0f;
            segment.start.y = 0.0f;
            segment.end.x = 0.0f;
            segment.end.y = 0.0f;
            segment.field_0x10 = -1.0f;
            UnkNavMeshCell* cell = fn_710027a33c(mesh, *reinterpret_cast<u16*>(stored.raw + 0x1e));
            {
                u8 side = stored.raw[0x1c];
                fn_71002afea8(cell, &side, &segment);
            }
            UnkVec2Result normal = fn_7100125e78(reinterpret_cast<const UnkSegment2D*>(&segment));
            return UnkVec3f(normal.x, normal.y, 0.0f);
        }
    }
    return UnkVec3f(0.0f, 1.0f, 0.0f);
}

// Stops the component: tells the owner's navigation it is blocked (a parameter block of kind 5), marks
// the component blocked, and, with bit 0 of `mode` set, asks the owner's AI whether the stored object still
// counts (slot 0x590) before sending it the kind (slot 0x5a8) and clearing the timed entries.
void fn_7100973cf4(UnkUnit1444_ovb0177* self);

void fn_7100974798(UnkUnit1444_ovb0177* self, u32 mode) {
    void* ai = self->owner->field_0x240;
    UnkNavigation* navigation = nullptr;
    if (ai)
        navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai));
    UnkParams params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x30 = 0;
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    fn_71006f4b0c(navigation, &params, 1);
    bool blocked = false;
    self->field_0x202 = true;
    if (mode & 1) {
        if (!self->owner->field_0x220) {
            blocked = true;
        } else {
            UnkTempObject_ovb0177 stored;
            blocked = true;
            if (self->field_0x1d0) {
                fn_71002b5c90(reinterpret_cast<UnkObject_0x100*>(stored.raw),
                              reinterpret_cast<const UnkObject_0x100*>(self->member.object));
                if (self->field_0x1d0) {
                    if (void* target = self->owner->field_0x240) {
                        blocked = !reinterpret_cast<bool (*)(void*, UnkObject_0x100*)>(
                            (*reinterpret_cast<void***>(target))[0x590 / 8])(
                            target, reinterpret_cast<UnkObject_0x100*>(stored.raw));
                    }
                }
            }
        }
    }
    if (void* target = self->owner->field_0x240)
        SendKind(self, target, blocked);
    fn_7100973cf4(self);
}

// As src/game/CCollisionComponent/fn_71008651c8.cpp declares the lookup of the colliders under a key.
struct UnkStruct_71008651c8_Entry;
struct UnkStruct_71008651c8;
using UnkOut_71008651c8 = base::UnkVector<UnkStruct_71008651c8_Entry*>;
void fn_71008651c8(UnkStruct_71008651c8* self, void* const* key, UnkOut_71008651c8* out);
struct UnkPlayerMovementSource;

extern void* const g_unk_717273a790 __attribute__((visibility("hidden")));
extern void* const g_unk_717273a788 __attribute__((visibility("hidden")));

void fn_7100973cf4(UnkUnit1444_ovb0177* self) {
    self->entries.Clear();
    UnkCollisionContainer* container = self->owner->field_0x2a0;
    if (!container)
        return;
    UnkOut_71008651c8 found(0xf);
    fn_71008651c8(reinterpret_cast<UnkStruct_71008651c8*>(container), &g_unk_717273a790, &found);
    for (UnkStruct_71008651c8_Entry **it = found.data, **end = found.data + found.count; it != end; ++it) {
        if (*it) {
            if (UnkCollisionObject* collider = fn_71008640a4(container, reinterpret_cast<const base::global::CStrId*>(*it)))
                fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), false);
        }
    }
    if (void* collider = fn_710086406c(reinterpret_cast<UnkPlayerMovementSource*>(container), &g_unk_717273a788))
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), false);
    found.Free();
}
