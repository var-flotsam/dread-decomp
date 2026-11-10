// CCharClassShowTutorialMessageEvent's unit (tools/tu_map.py): applies the event. Shows the tutorial named by the id at +0x10 on the
// game manager's tutorial composition (+0x2270) unless it is empty, then (unless the message id at +0x18 is empty) logs it as a
// kind 2 message to the game manager's callback set at +0x2128. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkTutorialComposition;
void fn_7100b88f10(UnkTutorialComposition* self, const CStrId* text);

struct UnkMlogMessage_b42a1;
void fn_7100456b7c(void* callbacks, const UnkMlogMessage_b42a1* message);

// The message's text id: copied like a CStrId, released in two steps like base::UnkStrIdMember.
struct UnkMlogText_ovb0139 : base::global::UnkStrIdBase {
    __attribute__((always_inline)) UnkMlogText_ovb0139(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    __attribute__((always_inline)) ~UnkMlogText_ovb0139() {
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

struct UnkMlogMessage_ovb0139 {
    u32 kind;
    UnkMlogText_ovb0139 text;
    base::UnkVector<base::UnkStrIdMember> args;

    __attribute__((always_inline)) UnkMlogMessage_ovb0139(u32 k, const CStrId& t) : kind(k), text(t), args(0xc) {}
    __attribute__((always_inline)) ~UnkMlogMessage_ovb0139() { args.Free(); }
};

struct UnkTutorialHolder_ovb0139 {
    u8 pad_0x0[0x10];
    UnkTutorialComposition* composition;  // 0x10
};

struct UnkShowTutorialEvent_ovb0139 {
    u8 pad_0x0[0x10];
    CStrId tutorial;  // 0x10
    CStrId message;   // 0x18
};

void fn_7100be0d3c(UnkShowTutorialEvent_ovb0139* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    UnkTutorialHolder_ovb0139* holder = static_cast<UnkTutorialHolder_ovb0139*>(manager->field_0x2270);
    if (holder && holder->composition && self->tutorial.data != &g_unk_7101d47108->empty)
        fn_7100b88f10(holder->composition, &self->tutorial);
    if (self->message.data == &g_unk_7101d47108->empty)
        return;
    UnkMlogMessage_ovb0139 message(2, CStrId(self->message));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                  reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
}
