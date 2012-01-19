tilemap = Tilemap.open('res/maps/untitled.lua')
w, h = tilemap:get_size()

player = dofile('res/scripts/entity.lua')
player:set_pos(27*16, 3*16)
player:set_vel(0, 0)

function set_random_pos(entity)
    repeat
        x = math.random(w) - 1
        y = math.random(h) - 1
    until not tilemap:is_tile_occupied(x, y)
    entity:set_pos(x * 16, y * 16)
end

function set_random_vel(entity)
    entity:set_vel(math.random(-1, 1), math.random(-1, 1))
end

-- returns the distance between two points
function distance(x1, y1, x2, y2)
    return math.sqrt((x1 - x2) ^ 2 + (y1 - y2) ^ 2)
end

-- create a guy who basically follows the player around
function create_stalker()
    local entity = dofile('res/scripts/entity.lua')
    set_random_pos(entity)
    entity:set_update_callback(function()
        x, y = entity:get_pos()
        dx, dy = 0, 0
        px, py = player:get_pos()
        if distance(x, y, px, py) > 32 then
            if x < px then
                dx = 1
            end
            if x > px then
                dx = -1
            end
            if y < py then
                dy = 1
            end
            if y > py then
                dy = -1
            end
        end
        entity:set_vel(dx, dy)
    end)
end

-- create an entity that just runs around bouncing off walls
function create_bouncer()
    local entity = dofile('res/scripts/entity.lua')
    set_random_pos(entity)
    set_random_vel(entity)
    entity:set_update_callback(function()
        x, y, w, h = entity:get_pos()
        dx, dy = entity:get_vel()
        if tilemap:is_region_occupied(x + dx, y, w, h) then
            dx = -dx
        end
        if tilemap:is_region_occupied(x, y + dy, w, h) then
            dy = -dy
        end
        entity:set_vel(dx, dy)
    end)
end

for i = 1,5 do
    create_stalker()
    create_bouncer()
end
