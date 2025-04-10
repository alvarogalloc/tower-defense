export module components.tags;

import ginseng;
export namespace components::tags {

  using player = ginseng::tag<struct _player>;
  using enemy = ginseng::tag<struct _enemy>;
  using bullet = ginseng::tag<struct _bullet>;
} // namespace components::tags