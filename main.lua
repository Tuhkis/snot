local test = require_tar("test")

snot.create_window(800, 600, 1, "Hey yall", false)
snot.show_window(true)

test:test()

local white = snot.get_white_texture()

local t = 0

while not snot.window_should_close() do
  local dt = snot.get_delta_time()
  t = t + dt * 5
  snot.render_begin()
    snot.clear()
    snot.set_clear_color(0.5, 0.7, 0.5)
    if snot.key_down(snot.KEY_SPACE) then
      snot.set_clear_color(0.5, 0.5, 0.7)
      t = t + dt * 5
    end
    if snot.key_down(snot.KEY_Q) and snot.key_down(snot.KEY_LEFT_CONTROL) then
      break
    end
    snot.draw_rect(math.abs(math.sin(t) * 64) + 32, 64 * math.cos(t * 0.9) + 96, 64, 64, 1, 1, 1.0, 1.0, 1.0, white)
  snot.render_end()
  snot.delay(1000.0 / 60.0)
end

