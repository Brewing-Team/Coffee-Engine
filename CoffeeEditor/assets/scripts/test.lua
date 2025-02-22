-- Test script for CoffeeEngine

--[[export]] exampleInt = 42
--[[export]] exampleFloat = 3.14
-- [[header]] Esto es un header
--[[export]] exampleString = "Hello, ImGui!"
--[[export]] exampleBool = true

--[[export]] child_amplitude = 2

--[[export]] parent = nil
local camera


--[[export]] childs = {}
local all_entities = {}

local time = 0

function OnReady()
    parent = self:get_parent()
    parent_name = parent:get_component("TagComponent").tag
    log("Parent name: " .. parent_name)

    camera = current_scene:get_entity_by_name("Camera")
    
    all_entities = current_scene:get_all_entities()

    childs = self:get_children()
end

function OnUpdate(delta)

    -- sinuodal movement of the camera
    camera:get_component("TransformComponent").position = Vector3.new(0, 0, 5 + math.sin(time) * 2)

    if input.is_key_pressed(input.keycode.Space) then
        log("Space")
    end

    if input.is_mouse_button_pressed(input.mousecode.Left) then
        log("Left")
        local x, y = input.get_mouse_position()
        log("Mouse position: (" .. x .. ", " .. y .. ")")
    end

end

function on_destroy()
    -- print("OnDestroy()")
    return 1
end