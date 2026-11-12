// CCollisionComponent unit (1319): sets the container's flag at +0xc9; when it turns on while +0xc8 is set, every
// record whose flag (+0x18) is set is refreshed through the container's vf_0x170 if its object answers true to
// its slot 0x58, else the record's +0x1c is cleared. Names are placeholders.
#include "game/CCollisionComponent.h"
#include "game/UnkCollisionRecord.h"

typedef void (*UnkCollisionRefresh_A5_b70)(UnkCollisionContainer* self, UnkCollisionRecord* record, s32 a);
typedef u32 (*UnkCollisionQuery_A5_b70)(UnkCollisionObject* object);

void fn_7100861fc8(UnkCollisionContainer* self, bool on) {
    char* raw = reinterpret_cast<char*>(self);
    if (on == *reinterpret_cast<bool*>(raw + 0xc9))
        return;
    *reinterpret_cast<bool*>(raw + 0xc9) = on;
    if (!*reinterpret_cast<bool*>(raw + 0xc8) || !on)
        return;
    for (u32 i = 0; i < self->records.count; ++i) {
        UnkCollisionRecord* record = &fn_71007253b8(&self->records, i);
        if (static_cast<u8>(record->field_0x18)) {
            UnkCollisionObject* object = record->field_0x28;
            if ((*reinterpret_cast<UnkCollisionQuery_A5_b70**>(object))[0x58 / 8](object))
                (*reinterpret_cast<UnkCollisionRefresh_A5_b70**>(self))[0x170 / 8](self, record, 0);
            else
                *reinterpret_cast<u32*>(reinterpret_cast<char*>(record) + 0x1c) = 0;
        }
    }
}

// The same refresh with mode 1 while +0xc8 is set, then fn_71008615b4 when +0x128 is set.
void fn_7100866668(UnkCollisionContainer* self) {
    char* raw = reinterpret_cast<char*>(self);
    if (*reinterpret_cast<bool*>(raw + 0xc8)) {
        for (u32 i = 0; i < self->records.count; ++i) {
            UnkCollisionRecord* record = &fn_71007253b8(&self->records, i);
            if (static_cast<u8>(record->field_0x18)) {
                UnkCollisionObject* object = record->field_0x28;
                if ((*reinterpret_cast<UnkCollisionQuery_A5_b70**>(object))[0x58 / 8](object))
                    (*reinterpret_cast<UnkCollisionRefresh_A5_b70**>(self))[0x170 / 8](self, record, 1);
                else
                    *reinterpret_cast<u32*>(reinterpret_cast<char*>(record) + 0x1c) = 0;
            }
        }
    }
    if (*reinterpret_cast<bool*>(raw + 0x128))
        fn_71008615b4(self);
}
