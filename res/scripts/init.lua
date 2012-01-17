local tilemap = Tilemap.load('res/maps/untitled.lua')
w, h = tilemap:get_size()
for i = 1,10 do
    repeat
        x = math.random(0, w - 1)
        y = math.random(0, h - 1)
    until not tilemap:tile_is_occupied(x, y)
    entity = Entity.new()
    entity:set_pos(x * 16, y * 16)
    entity:set_vel(math.random(-1, 1), math.random(-1, 1))
    entity:set_size(16, 18)
    entity:set_sprite('res/player1.bmp')
    entity:add_frames(ACTION_WALKING, DIRECTION_UP, {0, 2})
    entity:add_frames(ACTION_STANDING, DIRECTION_UP, {1})
    entity:add_frames(ACTION_WALKING, DIRECTION_RIGHT, {3, 5})
    entity:add_frames(ACTION_STANDING, DIRECTION_RIGHT, {4})
    entity:add_frames(ACTION_WALKING, DIRECTION_DOWN, {6, 8})
    entity:add_frames(ACTION_STANDING, DIRECTION_DOWN, {7})
    entity:add_frames(ACTION_WALKING, DIRECTION_LEFT, {9, 11})
    entity:add_frames(ACTION_STANDING, DIRECTION_LEFT, {10})
    entity = nil
end
