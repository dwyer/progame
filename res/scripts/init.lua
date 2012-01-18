tilemap = Tilemap.open('res/maps/untitled.lua')
w, h = tilemap:get_size()
for i = 1,10 do
    entity = dofile('res/scripts/entity.lua')
    repeat
        x = math.random(w) - 1
        y = math.random(h) - 1
    until not tilemap:is_tile_occupied(x, y)
    entity:set_pos(x * 16, y * 16)
    entity:set_vel(math.random(-1, 1), math.random(-1, 1))
end
