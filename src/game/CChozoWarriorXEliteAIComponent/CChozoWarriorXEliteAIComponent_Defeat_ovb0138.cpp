// CChozoWarriorXEliteAIComponent's unit (tools/tu_map.py): what the component does when its enemy is
// defeated: the "#MLOG_DEFEAT_BOSS_CWXE_SHIP" message goes to the game manager's callback set at
// +0x2128 (as the E.M.M.I. components send theirs,
// src/game/CEmmyProtoAIComponent/CEmmyProtoAIComponent_Defeat_ovb0003.cpp), then the base handler
// (fn_710048e708) runs. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkMlogText_ovb0138 : base::global::UnkStrIdBase {
    __attribute__((always_inline)) UnkMlogText_ovb0138(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    __attribute__((always_inline)) ~UnkMlogText_ovb0138() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMlogMessage_ovb0138 {
    u32 kind;
    UnkMlogText_ovb0138 text;
    base::UnkVector<base::UnkStrIdMember> args;

    __attribute__((always_inline)) UnkMlogMessage_ovb0138(const CStrId& t) : kind(0), text(t), args(0xc) {}
    __attribute__((always_inline)) ~UnkMlogMessage_ovb0138() { args.Free(); }
};

struct UnkMlogMessage_b42a1;
void fn_7100456b7c(void* callbacks, const UnkMlogMessage_b42a1* message);
void fn_710048e708(void* self, void* arg);

void fn_710047ad94(void* self, void* arg) {
    UnkMlogMessage_ovb0138 message(CStrId("#MLOG_DEFEAT_BOSS_CWXE_SHIP", true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                  reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
    fn_710048e708(self, arg);
}
