-- Project: Slew
-- File: extras/test_all/geom.lua
-- Description: Lua script for placing some entities in the test_all example.
-- License: MIT
-- https://github.com/colleen05/Slew

DEG2RAD = 0.0174532925199

function make_ball_circle()
    radius = 1.5
    scale = 0.125

    for i = 1,36 do
        j = i * 10
        ent_create_ball(
            math.cos(j * DEG2RAD) * radius,
            0,
            math.sin(j * DEG2RAD) * radius,
            scale, j
        )
    end
end

function make_ball_plane()
    for i = 1,32 do
        for j = 1,32 do
            px = i/4 - 4.0
            py = j/4 - 4.0
            
            ent_create_ball(
                px,
                (math.cos(px*2) + math.cos(py*2)) / 4.0,
                py,
                0.125, (math.abs(i-j)/16)*360
            )
        end
    end
end

print("make_ball_circle() ready.")
print("make_ball_plane() ready.")

ents_clear()
make_ball_plane()
