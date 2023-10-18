-- Project: Slew
-- File: extras/test_all/fractal.lua
-- Description: Lua script for making a fractal in the test_all example console.
-- License: MIT
-- https://github.com/colleen05/Slew

function print_fractal(n)
    output = ""
    n = 2 ^ n
    y = n - 1
    while y >= 0 do
        i = 0
        
        while i < y do
            output = output .. " "
            i = i + 1
        end
  
        x = 0
        while x + y < n do
            if (x & y) ~= 0 then
                output = output .. "  "
            else
                output = output .. "* "
            end
            x = x + 1
        end
        output = output .. "\n"
        y = y - 1
    end
    
    print(output)
end

print("Now try \"print_fractal(n)\". ;)")

-- This code is adapted from Nikita Tiwari.
