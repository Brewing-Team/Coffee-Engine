-- Test script for CoffeeEngine

--[[export]] exampleInt = 42
--[[export]] exampleFloat = 3.14
-- [[header]] Esto es un header
--[[export]] exampleString = "Hello, ImGui!"
--[[export]] exampleBool = true

function OnReady()
    print("OnCreate()")
    log("OnCreate()")
    log_error("OnCreate()")
    log_warning("OnCreate()")
    log_critical("OnCreate()")
end

function OnUpdate()
    transform = self:get_component("TransformComponent"):get_world_transform()
    translation = Vector3.new(1.0, 2.0, 3.0)
    transform:translate(translation)
end

function on_destroy()
    -- print("OnDestroy()")
    return 1
end