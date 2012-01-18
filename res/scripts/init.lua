tilemap = Tilemap.open('res/maps/untitled.lua')
w, h = tilemap:get_size()

function create_player_update_callback(entity)
    return function()
        x, y, w, h = entity:get_pos()
        dx, dy = entity:get_vel()
        if tilemap:is_region_occupied(x + dx, y, w, h) then
            dx = 0
        else
            x = x + dx
        end
        if tilemap:is_region_occupied(x, y + dy, w, h) then
            dy = 0
        else
            y = y + dy
        end
        entity:set_pos(x, y)
        entity:set_vel(dx, dy)
    end
end

function create_entity_update_callback(entity)
    return function()
        x, y, w, h = entity:get_pos()
        dx, dy = entity:get_vel()
        if tilemap:is_region_occupied(x + dx, y, w, h) then
            dx = -dx
        else
            x = x + dx
        end
        if tilemap:is_region_occupied(x, y + dy, w, h) then
            dy = -dy
        else
            y = y + dy
        end
        entity:set_pos(x, y)
        entity:set_vel(dx, dy)
    end
end

player = dofile('res/scripts/entity.lua')
player:set_pos(27*16, 3*16)
player:set_vel(0, 0)
player:set_update_callback(create_player_update_callback(player))

for i = 1,10 do
    local entity = dofile('res/scripts/entity.lua')
    repeat
        x = math.random(w) - 1
        y = math.random(h) - 1
    until not tilemap:is_tile_occupied(x, y)
    entity:set_pos(x * 16, y * 16)
    entity:set_vel(math.random(-1, 1), math.random(-1, 1))
    entity:set_update_callback(create_entity_update_callback(entity))
end
