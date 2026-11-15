// sound::CMusicManager::CTunableMusicManager unit: switches the manager's state machine to the mode
// named by a string, unless the game manager is in one of four states. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/UnkGameManager.h"
#include "../../../CSensorDoorComponent/UnkDoorStateMachine_b58.h"

bool fn_7100deac44(UnkGameManager* self);
bool fn_7100deac64(UnkGameManager* self);

struct UnkModeName_lfdb640 {
    const char* data;
};


u32 fn_7100fdb794(const char* name);

void fn_7100fdb640(UnkDoorStateMachine_b58* self, const UnkModeName_lfdb640* name) {
    UnkGameManager* manager = g_unk_7101d47310;
    if (fn_7100deabe4(manager) || fn_7100deac04(manager) || fn_7100deac44(manager) || fn_7100deac64(manager))
        return;
    u32 mode = fn_7100fdb794(name->data ? name->data : g_unk_710162e2ed);
    if (mode == 0)
        mode = 3;
    self->pending = mode;
    if (self->field_0xa8 || mode > 7)
        return;
    UnkDoorState_b58* current = self->current;
    if (!current || current->id != (s32)mode) {
        if (UnkDoorState_b58* next = FindState_b58(self, mode)) {
            if (current) {
                current->vf_0x18(mode);
                self->vf_0x28(self->current, next);
            }
            self->previous = self->current;
            self->current = next;
            next->vf_0x10(self->previous ? self->previous->id : 0x7fffffff);
            self->vf_0x20(self->previous, next);
        }
    }
    self->pending = 0x7fffffff;
}
