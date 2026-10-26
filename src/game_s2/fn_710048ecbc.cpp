// Resets the owner's components: looks up four components of the object at +0x280 of the owner by
// hash (keeping those the kind checks accept), resets them all, then runs fn_7100232c68 on the
// targets of the two handles at +0x154e8 and +0x154f0 of the object.
#include "game/UnkSwitchObject.h"
#include "types.h"

struct UnkComponent {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};

struct UnkObject;

struct UnkOwner {
    u8 pad_0x0[0x280];
    UnkObject* field_0x280;
};

struct UnkHandleTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkHandles {
    UnkHandleTarget* field_0x0;
    UnkHandleTarget* field_0x8;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
    u8 pad_0x38[0x154b0];
    UnkHandles field_0x154e8;
};

UnkComponent* fn_7101152b90(UnkObject* object, u64 hash);
bool fn_710114e680(UnkObject* object, u8 kind);
bool fn_710114e668(UnkObject* object, u8 kind);
void fn_7100145e94(UnkComponent* component, u32 flag);
void fn_7100232c68(void* target);

void fn_710048ecbc(UnkSelf* self) {
    if (UnkObject* object = self->field_0x30->field_0x280) {
        UnkComponent* a = fn_7101152b90(object, 0x89f328dc37ac167bull);
        if (a)
            a = fn_710114e680(object, a->field_0xb) ? a : nullptr;
        UnkComponent* b = fn_7101152b90(object, 0x8c9dfb3ece4e323cull);
        if (b)
            b = fn_710114e680(object, b->field_0xb) ? b : nullptr;
        UnkComponent* c = fn_7101152b90(object, 0xa0cda40259a7a253ull);
        if (c)
            c = fn_710114e668(object, c->field_0xb) ? c : nullptr;
        UnkComponent* d = fn_7101152b90(object, 0xa5a377e0a0458614ull);
        if (d)
            d = fn_710114e668(object, d->field_0xb) ? d : nullptr;
        fn_7100145e94(a, 0);
        fn_7100145e94(b, 0);
        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(c), 0);
        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(d), 0);
    }
    if (self->field_0x154e8.field_0x8 && self->field_0x154e8.field_0x8->field_0x10)
        fn_7100232c68(self->field_0x154e8.field_0x8->field_0x10);
    if (self->field_0x154e8.field_0x0 && self->field_0x154e8.field_0x0->field_0x10)
        fn_7100232c68(self->field_0x154e8.field_0x0->field_0x10);
}
