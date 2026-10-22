// Dispatches an event to a chain of handlers (one of three handler functions, vf_0x0 to
// vf_0x10): while dispatching, the owner is marked busy; a handler is called when the owner's
// mask matches the event's first mask and the handler's mask its second. Stops at the first
// handler that returns 1; the result is the last handler's result (0 when none ran).
#include "types.h"

struct UnkEvent {
    u8 unk_0x0[0x58];
    u32 mask_0x58;
    u32 mask_0x5c;
};

struct UnkHandler {
    virtual s32 vf_0x0(UnkEvent* event);
    virtual s32 vf_0x8(UnkEvent* event);
    virtual s32 vf_0x10(UnkEvent* event);
};

struct UnkHandlerNode {
    UnkHandler* handler;
    u32 mask;
    UnkHandlerNode* next;
};

struct UnkDispatcher {
    u8 unk_0x0[0x64];
    u32 mask;
    u8 unk_0x68[0x98];
    UnkHandlerNode* head;
    u8 unk_0x108[0x18];
    bool busy;
};

template <s32 (UnkHandler::*Handle)(UnkEvent*)>
__attribute__((always_inline)) static inline s32 Dispatch(UnkDispatcher* self, UnkEvent* event) {
    self->busy = true;
    s32 result = 0;
    for (UnkHandlerNode* node = self->head; node; node = node->next) {
        if ((self->mask & event->mask_0x58) && (node->mask & event->mask_0x5c)) {
            result = (node->handler->*Handle)(event);
            if (result == 1)
                break;
        }
    }
    self->busy = false;
    return result;
}

s32 fn_7100d35cfc(UnkDispatcher* self, UnkEvent* event) { return Dispatch<&UnkHandler::vf_0x0>(self, event); }
s32 fn_7100d35d98(UnkDispatcher* self, UnkEvent* event) { return Dispatch<&UnkHandler::vf_0x8>(self, event); }
s32 fn_7100d35e34(UnkDispatcher* self, UnkEvent* event) { return Dispatch<&UnkHandler::vf_0x10>(self, event); }
