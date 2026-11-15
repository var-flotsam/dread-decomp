// CLightManager's unit: points an iterator at the first node of a linked list, keeping the current element in a
// two-state variant (as UnkListIterVariantNext_b43.cpp's advance). Names and layouts are placeholders.
#include "types.h"

void fn_7100e0ccf8(char*);

struct UnkLightListNode_e0ebdc {
    u8 unk_0x0[0x98];
    UnkLightListNode_e0ebdc* next;
};

struct UnkLightListHead_e0ebdc {
    u8 pad_0x0[0x40];
    UnkLightListNode_e0ebdc* head;
};

struct UnkLightListVariant_e0ebdc {
    union {
        char object[0x98];
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    void Reset() {
        switch (state) {
        case 1:
            fn_7100e0ccf8(object);
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    void SetPair(void* first, void* second) {
        s32* p = &state;
        switch (*p) {
        case 1:
            fn_7100e0ccf8(object);
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = 2;
        pair.first = first;
        pair.second = second;
    }
};

struct UnkLightListIter_e0ebdc {
    UnkLightListNode_e0ebdc* node;
    UnkLightListVariant_e0ebdc variant;
};

void fn_7100e0ebdc(const UnkLightListHead_e0ebdc* list, UnkLightListIter_e0ebdc* out) {
    UnkLightListNode_e0ebdc* n = list->head;
    out->node = n;
    if (!n) {
        out->variant.Reset();
        return;
    }
    out->variant.SetPair(n, reinterpret_cast<u8*>(n) + 8);
}
