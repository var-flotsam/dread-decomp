// Unit 2563 (CMinimapData hint): appends a record under a u64 key to the keyed array of 0x60-byte
// records (records at +0x0, keys at +0x8, count +0x10, capacity +0x14, step +0x18), growing it by the
// step when full (fn_7100e54ad8); the record is a 0x28-byte part (fn_71000cbae4), an array of 16-bit
// values and four words. Names and layouts are placeholders.
#include <cstring>
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "game/UnkTU2563.h"
#include "game/UnkTU83.h"
#include "types.h"

struct UnkWordArray_e54348 : UnkMovedArray<u16> {
    UnkWordArray_e54348(const UnkWordArray_e54348& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        if (capacity < other.capacity) {
            u16* p = static_cast<u16*>(allocator->vf_0x40((u64)other.capacity * sizeof(u16), 8, flags));
            if (data) {
                std::memcpy(p, data, (u64)count * sizeof(u16));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = other.capacity;
        }
        std::memcpy(data, other.data, (u64)other.count * sizeof(u16));
        count = other.count;
    }
};

struct UnkRecord_e54348 {
    u8 part[0x28];            // 0x0
    UnkWordArray_e54348 ids;  // 0x28
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

};
static_assert(sizeof(UnkRecord_e54348) == 0x60, "placeholder layout");

struct UnkRecords_e54348 {
    UnkRecord_e54348* values;  // 0x0
    u64* keys;                 // 0x8
    u32 count;                 // 0x10
    u32 capacity;              // 0x14
    u32 step;                  // 0x18
};

void fn_7100e54348(UnkRecords_e54348* self, const u64* key, const UnkRecord_e54348* record) {
    if (self->count == self->capacity)
        ReserveRecord60AndPointerArrays(reinterpret_cast<UnkRecord60Array*>(self), self->step + self->count);
    self->keys[self->count] = *key;
    UnkRecord_e54348* values = self->values;
    u32 i = self->count;
    fn_71000cbae4(reinterpret_cast<UnkPart48*>(values[i].part), reinterpret_cast<const UnkPart48*>(record->part));
    new (&values[i].ids) UnkWordArray_e54348(record->ids);
    values[i].field_0x50 = record->field_0x50;
    values[i].field_0x54 = record->field_0x54;
    values[i].field_0x58 = record->field_0x58;
    values[i].field_0x5c = record->field_0x5c;
    ++self->count;
}
