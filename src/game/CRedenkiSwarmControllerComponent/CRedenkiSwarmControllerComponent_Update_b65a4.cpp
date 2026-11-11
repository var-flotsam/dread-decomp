// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py): runs the base step fn_710074c684 with the
// argument; then in mode 2 (+0x390) with the word at +0x530 clear, takes the position of fn_7100739c64's
// object (fn_7100745e24) and asks fn_710073fea8 for a point within 50 of it; when it finds one, the
// point goes to the record at +0x31c (fn_7100737d4c, its flag set), fn_710073f7bc gets 1 and +0x568
// becomes 1.5. Names and layouts are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "types.h"

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent_b38.cpp defines it.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};
struct UnkParent_71007455dc;
UnkVec3Result fn_7100745e24(UnkParent_71007455dc* parent);

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent.cpp declares it.
struct UnkRedenkiSwarmController;

struct UnkSwarmPoint_b65a4 {
    u64 value;
};

struct UnkSwarmRecord_b65a4 {
    bool active;
};

struct UnkSwarmUpdate_b65a4 {
    u8 pad_0x0[0x31c];
    UnkSwarmRecord_b65a4 record;  // 0x31c
    u8 pad_0x31d[0x390 - 0x31d];
    u32 mode;  // 0x390
    u8 pad_0x394[0x530 - 0x394];
    u32 field_0x530;
    u8 pad_0x534[0x568 - 0x534];
    f32 field_0x568;
};

void fn_710074c684(UnkSwarmUpdate_b65a4* self, void* arg);

void fn_7100650260(UnkSwarmUpdate_b65a4* self, void* arg) {
    fn_710074c684(self, arg);
    if (self->mode == 2 && self->field_0x530 == 0) {
        UnkVec3Result pos = fn_7100745e24(fn_7100739c64(self));
        UnkSwarmPoint_b65a4 point;
        point.value = 0;
        if (fn_710073fea8(self, &pos, &point, 50.0f)) {
            fn_7100737d4c(&self->record, &point);
            self->record.active = true;
            fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 1);
            self->field_0x568 = 1.5f;
        }
    }
}
