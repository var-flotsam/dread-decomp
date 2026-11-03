// A Nailong AI message handler (tools/tu_map.py: the CNailongAIComponent unit): after the base
// handler, when the message's flag (+0x20) is set and the owner's component at +0x278 is not busy,
// sets the slot colour through the owner's +0x228 chain: white with alpha cleared in state 2 (+0x148c8)
// after a second call on its +0x138, else opaque black. Names are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;

struct UnkStruct_71001436f4;
bool fn_71001436f4(UnkStruct_71001436f4* self, u32 a, const base::math::CVector4D* b, u32 c);
bool fn_7101136308(void*);

struct UnkNailongSlots_b46a2 {
    u8 pad_0x0[0x138];
    void* field_0x138;
};

struct UnkNailongModel_b46a2 {
    u8 pad_0x0[0x50];
    UnkNailongSlots_b46a2* slots;
};

struct UnkNailongActor_b46a2 {
    u8 pad_0x0[0x228];
    UnkNailongModel_b46a2* model;
    u8 pad_0x230[0x48];
    void* field_0x278;
};

struct UnkNailongAI_b46a2 {
    u8 pad_0x0[0x30];
    UnkNailongActor_b46a2* actor;
    u8 pad_0x38[0x148c8 - 0x38];
    u32 state;
};

struct UnkNailongMsg_b46a2 {
    u8 pad_0x0[0x20];
    bool flag;
};

u32 fn_7101111dc8(UnkNailongAI_b46a2* self, UnkNailongMsg_b46a2* msg);
void fn_71001c287c(void* target, u32 a, u32 b, CVector4D* colour, u32 c);

u32 fn_71006220bc(UnkNailongAI_b46a2* self, UnkNailongMsg_b46a2* msg) {
    u32 r = fn_7101111dc8(self, msg);
    if (msg->flag) {
        void* c = self->actor->field_0x278;
        if (c && !fn_7101136308(c)) {
            if (self->state == 2) {
                CVector4D colour;
                colour.x = 1.0f;
                colour.y = 1.0f;
                colour.z = 1.0f;
                colour.w = 1.0f;
                fn_71001c287c(self->actor->model->slots->field_0x138, 0, 1, &colour, 1);
                colour.w = 0.0f;
                fn_71001436f4(reinterpret_cast<UnkStruct_71001436f4*>(self->actor->model->slots), 1, &colour, 1);
            } else {
                UnkNailongSlots_b46a2* slots = self->actor->model->slots;
                CVector4D colour;
                colour.x = 0.0f;
                colour.y = 0.0f;
                colour.z = 0.0f;
                colour.w = 1.0f;
                fn_71001436f4(reinterpret_cast<UnkStruct_71001436f4*>(slots), 0, &colour, 1);
            }
        }
    }
    return r;
}
