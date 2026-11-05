// fn_7100a23ae8 (CFreezeRoomComponent unit 1537) and its copy fn_7100a27380 (CHeatRoomComponent unit 1538):
// the owner's object at +0x28 is cast to the class of fn_71011abbd4 (null when it is not one), then
// fn_7100f0e4e4 runs on the game manager's +0x38, the cast object's +0x98, the owner's +0x30 and the
// argument's +0x30. Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

#include "base/reflection/UnkClassGetters.h"

void fn_7100f0e4e4(void* a, void* b, void* c, void* d);

struct UnkRoomOwner_A5_b74 {
    u8 pad_0x0[0x28];
    base::reflection::UnkReflObject* object;
};

struct UnkRoomComponent_A5_b74 {
    u8 pad_0x0[0x30];
    UnkRoomOwner_A5_b74* owner;
};

static inline __attribute__((always_inline)) void RoomApply_A5_b74(UnkRoomComponent_A5_b74* self, char* arg) {
    char* manager = reinterpret_cast<char*>(g_unk_7101d47310) + 0x38;
    base::reflection::UnkReflObject* obj = self->owner->object;
    if (obj && !fn_710009d50c(obj->vf_0x0(), fn_71011abbd4()))
        obj = nullptr;
    fn_7100f0e4e4(manager, reinterpret_cast<char*>(obj) + 0x98,
                  reinterpret_cast<char*>(self->owner) + 0x30, arg + 0x30);
}

void fn_7100a23ae8(UnkRoomComponent_A5_b74* self, char* arg) {
    RoomApply_A5_b74(self, arg);
}

void fn_7100a27380(UnkRoomComponent_A5_b74* self, char* arg) {
    RoomApply_A5_b74(self, arg);
}
