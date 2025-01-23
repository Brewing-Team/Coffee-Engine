-- Test script for CoffeeEngine

--[[export]] exampleInt = 42
--[[export]] exampleFloat = 3.14
-- [[header]] Esto es un header
--[[export]] exampleString = "Hello, ImGui!"
--[[export]] exampleBool = true

function OnCreate()
    print("OnCreate()")
    log("OnCreate()")
    log_error("OnCreate()")
    log_warning("OnCreate()")
    log_critical("OnCreate()")
    return 1
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
    -- print("OnDestroy()")
    return 1
end