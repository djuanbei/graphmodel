#include "model/channel.h"

namespace graphsat {

Channel::Channel()
    : local_id(-1), gloabl_id(-1), type(ONE2ONE_CH), action(CHANNEL_RECEIVE),
      is_ref(false) {}

Channel::Channel(int id)
    : local_id(-1), gloabl_id(-1), type(ONE2ONE_CH), action(CHANNEL_RECEIVE),
      is_ref(false) {
  if (id > 0) {
    action = CHANNEL_SEND;
    gloabl_id = id;
  } else {
    action = CHANNEL_RECEIVE;
    gloabl_id = -id;
  }
}
Channel::Channel(int id, bool ref)
    : local_id(-1), gloabl_id(-1), type(ONE2ONE_CH), action(CHANNEL_RECEIVE),
      is_ref(false) {
  is_ref = ref;
  if (id > 0) {
    action = CHANNEL_SEND;
  } else {
    action = CHANNEL_RECEIVE;
    id *= -1;
  }
  if (ref) {
    assert(id > 0);
    local_id = chanIdToFromPid(id);
  } else {
    gloabl_id = id;
  }
}
int ArrayChannel::getGlobalId(int *state) const {
  return array_base + Channel::getGlobalId(state);
}

int IndexChannel::getGlobalId(int *state) const {
  return array_base + index_fun(state);
}

} // namespace graphsat
