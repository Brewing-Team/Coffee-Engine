-- Test script for CoffeeEngine

--[[export]] exampleInt = 42
--[[export]] exampleFloat = 3.14
--[[export]] exampleString = "Hello, ImGui!"
--[[export]] exampleBool = true

function OnCreate()
    print("OnCreate()")
    log("OnCreate()")
    log_error("OnCreate()")
    log_warning("OnCreate()")
    log_critical("OnCreate()")
end

function OnUpdate()
    --log("OnUpdate()")
    if input.is_key_pressed(input.keycode.SPACE) then
        log("SPACE")
    end

    if input.is_mouse_button_pressed(input.mousecode.LEFT) then
        log("LEFT")
        local x, y = input.get_mouse_position()
        log("Mouse position: (" .. x .. ", " .. y .. ")")
    end

    log("BOOL: " .. tostring(exampleBool))
    log("INT: " .. exampleInt)
    log("FLOAT: " .. exampleFloat)
    log("STRING: " .. exampleString)

    local entityTag = self.GetComponent("TagComponent").tag
    print("Entity tag: " .. entityTag)
end

function on_destroy()
    -- print("OnDestroy()")
end