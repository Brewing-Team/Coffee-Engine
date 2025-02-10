-- Test script for CoffeeEngine

--[[export]] lives = 5

function OnReady()
end

function OnUpdate()
    --log("OnUpdate()")
    if input.is_key_pressed(input.keycode.Space) then
        log("SPACE")
    end

    if input.is_mouse_button_pressed(input.mousecode.Left) then
        log("LEFT")
        local x, y = input.get_mouse_position()
        log("Mouse position: (" .. x .. ", " .. y .. ")")
    end

    log("BOOL 2: " .. tostring(exampleBool))
    log("INT 2: " .. exampleInt)
    log("FLOAT 2: " .. exampleFloat)
    log("STRING 2: " .. exampleString)
    return 1
end

function on_destroy()
end