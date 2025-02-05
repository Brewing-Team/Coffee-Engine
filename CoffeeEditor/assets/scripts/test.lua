-- Test script for CoffeeEngine

--[[export]] exampleInt = 42
--[[export]] exampleFloat = 3.14
-- [[header]] Esto es un header
--[[export]] exampleString = "Hello, ImGui!"
--[[export]] exampleBool = true

--[[export]] child_amplitude = 2

local parent
local camera


local childs = {}
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

    for i, entity in ipairs(childs) do
        local transform = entity:get_component("TransformComponent")
        transform.position = Vector3.new(math.sin(time + i) * child_amplitude, math.cos(time + i) * child_amplitude, 0)
        transform.rotation = Vector3.new(time * 100, time * 100, 0)
    end

    time = time + delta

    parent_script = parent:get_component("ScriptComponent")

    log(tostring(parent_script.lives))

    parent_script.lives = parent_script.lives + 1

end

function on_destroy()
    -- print("OnDestroy()")
    return 1
end