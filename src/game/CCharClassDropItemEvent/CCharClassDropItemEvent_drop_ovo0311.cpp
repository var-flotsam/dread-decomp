// CCharClassDropItemEvent's unit (tools/tu_map.py): the event for an actor dropping an item.
// With the actor's AI component flagged (+0x15d8), the drop position is the player's position
// (z from the target actor), moved to the drop component's node when it names one and the shape
// has it, and handed on with the AI component's own position (+0x15dc); otherwise the item is
// dropped through fn_71007c1fb0 unless the character class has the ignore track. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include <new>
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkEntity;
struct UnkPlayer;
struct UnkDropCameraVec3;
struct UnkSwarmSelf_b0143;
struct UnkDropSpawnComp_ovb0096;
struct UnkXformAtF0;

bool fn_71011735e8(UnkEntity* entity, const void* key, UnkMatrix44* out, u32 flags);
UnkVec3f fn_7100076acc(const UnkXform* x);
char* fn_71004dad10(UnkActor*);
void fn_710110e278(UnkSwarmSelf_b0143* self, void* a, void* b, void* c, bool flag);
void fn_71007c1ab0x(UnkDropSpawnComp_ovb0096* self, void* unused) asm("fn_71007c1fb0");
UnkVec3f fn_710007a450(void* xform, const UnkVec3f* v);

struct UnkDropEvent_ovb0267 {
    UnkActor* actor;
};

struct UnkDropNode_ovb0267 {
    u8 pad_0x0[0xe8];
    base::global::UnkStringData* field_0xe8;
    u8 field_0xf0[0x40];
};

struct UnkDropRecord_ovb0267 {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
};

struct UnkDropAi_ovb0267 {
    u8 pad_0x0[0x15d8];
    bool field_0x15d8;
    UnkVec3f field_0x15dc;
};

void fn_7100cb5cbc(UnkDropSpawnComp_ovb0096* self, UnkDropEvent_ovb0267* event) {
    if (!event->actor)
        return;
    UnkDropRecord_ovb0267* record = reinterpret_cast<UnkDropRecord_ovb0267*>(fn_71004dad10(event->actor));
    if (!record)
        return;
    UnkDropAi_ovb0267* ai = static_cast<UnkDropAi_ovb0267*>(event->actor->field_0x240);
    UnkVec3f out(0.0f, 0.0f, 0.0f);
    if (ai) {
        out.x = ai->field_0x15dc.x;
        out.y = ai->field_0x15dc.y;
        out.z = ai->field_0x15dc.z;
        if (ai->field_0x15d8) {
            UnkVec3f pos = *reinterpret_cast<const UnkVec3f*>(
                fn_7100d7c9f4(reinterpret_cast<UnkPlayer*>(event->actor)));
            if (UnkActor* target = fn_7100d7c990(event->actor))
                pos.z = target->field_0x64;
            UnkDropNode_ovb0267* node = reinterpret_cast<UnkDropNode_ovb0267*>(record->field_0x10);
            if (node) {
                UnkEntity* shape = static_cast<UnkEntity*>(event->actor->field_0x228);
                bool isDrop = fn_710009d50c(reinterpret_cast<UnkReflObject*>(node)->vf_0x0(), fn_71007be344());
                if (shape && isDrop && node->field_0xe8 != &g_unk_7101d47108->empty) {
                    UnkMatrix44 m = {};
                    m.m[15] = 1.0f;
                    m.m[10] = 1.0f;
                    m.m[5] = 1.0f;
                    m.m[0] = 1.0f;
                    bool found;
                    {
                        u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(node->field_0xe8) + 0x30);
                        found = fn_71011735e8(shape, &key, &m, 0);
                    }
                    UnkVec3f moved;
                    UnkVec3f angle;
                    if (found) {
                        void* frame = node->field_0xf0;
                        pos.x = m.m[3];
                        pos.y = m.m[7];
                        pos.z = m.m[11];
                        new (&angle) UnkVec3f(fn_7100076acc(reinterpret_cast<const UnkXform*>(&m)));
                        new (&moved) UnkVec3f(fn_710007a450(frame, &angle));
                        pos.x = moved.x + pos.x;
                        pos.y = moved.y + pos.y;
                        pos.z = moved.z + pos.z;
                    }
                }
            }
            fn_710110e278(reinterpret_cast<UnkSwarmSelf_b0143*>(ai), nullptr, &pos, &out, false);
            return;
        }
    }
    void* charClass = event->actor->field_0x220;
    if (charClass && fn_710117b348(static_cast<UnkAnimationComponent*>(charClass), fn_7100c2c1e8()))
        return;
    fn_71007c1ab0x(reinterpret_cast<UnkDropSpawnComp_ovb0096*>(record), self);
}
