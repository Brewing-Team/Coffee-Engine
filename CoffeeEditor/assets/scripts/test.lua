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

    local entityTag = entity:GetComponent("TagComponent").tag
    log(entityTag)

    local transform = entity:GetComponent("TransformComponent")
    local entityPosition = transform.position
    log(tostring(entityPosition.x) .. ", " .. tostring(entityPosition.y) .. ", " .. tostring(entityPosition.z))

    if input.is_key_pressed(input.keycode.W) then
        entityPosition.y = entityPosition.y + 0.01
    end
    if input.is_key_pressed(input.keycode.A) then
        entityPosition.x = entityPosition.x - 0.01
    end
    if input.is_key_pressed(input.keycode.S) then
        entityPosition.y = entityPosition.y - 0.01
    end
    if input.is_key_pressed(input.keycode.D) then
        entityPosition.x = entityPosition.x + 0.01
    end

    log(tostring(entityPosition.x) .. ", " .. tostring(entityPosition.y) .. ", " .. tostring(entityPosition.z))

    if input.is_mouse_button_pressed(input.mousecode.LEFT) then
        log("LEFT")
        local x, y = input.get_mouse_position()
        log("Mouse position: (" .. x .. ", " .. y .. ")")
    end
    return 1
end

function on_destroy()
    -- print("OnDestroy()")
    return 1
end