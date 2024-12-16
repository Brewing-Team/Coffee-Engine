--[[export]] sensitivity = 0.1
--[[export]] speed = 0.05

local yaw = 0.0
local pitch = 0.0
local lastMouseX, lastMouseY = input.get_mouse_position()

function OnCreate()
    print("FPS Camera Controller Created")
    return 1
end

function OnUpdate()
    local transform = entity:GetComponent("TransformComponent")
    local entityPosition = transform.position
    local entityRotation = transform.rotation

    -- Mouse look
    local mouseX, mouseY = input.get_mouse_position()
    local deltaX = mouseX - lastMouseX
    local deltaY = mouseY - lastMouseY
    lastMouseX = mouseX
    lastMouseY = mouseY

    if input.is_mouse_button_pressed(input.mousecode.RIGHT) then
        yaw = yaw - deltaX * sensitivity
        pitch = pitch - deltaY * sensitivity

        -- Clamp pitch
        if pitch > 89.0 then pitch = 89.0 end
        if pitch < -89.0 then pitch = -89.0 end

        -- Update entity rotation
        entityRotation.x = pitch
        entityRotation.y = yaw
        entityRotation.z = 0.0

        transform.rotation = entityRotation

        print("Yaw: " .. yaw .. " Pitch: " .. pitch)
    end

    -- Calculate direction
    local front = {
        x = math.cos(math.rad(yaw)) * math.cos(math.rad(pitch)),
        y = math.sin(math.rad(pitch)),
        z = math.sin(math.rad(yaw)) * math.cos(math.rad(pitch))
    }

    -- Normalize direction
    local length = math.sqrt(front.x * front.x + front.y * front.y + front.z * front.z)
    front.x = front.x / length
    front.y = front.y / length
    front.z = front.z / length

    -- Calculate right vector
    local worldUp = { x = 0.0, y = 1.0, z = 0.0 }
    local right = {
        x = front.z * worldUp.y - front.y * worldUp.z,
        y = front.x * worldUp.z - front.z * worldUp.x,
        z = front.y * worldUp.x - front.x * worldUp.y
    }

    -- Normalize right vector
    length = math.sqrt(right.x * right.x + right.y * right.y + right.z * right.z)
    right.x = right.x / length
    right.y = right.y / length
    right.z = right.z / length

    -- Movement
    if input.is_key_pressed(input.keycode.W) then
        entityPosition.x = entityPosition.x + front.x * speed
        entityPosition.y = entityPosition.y + front.y * speed
        entityPosition.z = entityPosition.z + front.z * speed
    end
    if input.is_key_pressed(input.keycode.S) then
        entityPosition.x = entityPosition.x - front.x * speed
        entityPosition.y = entityPosition.y - front.y * speed
        entityPosition.z = entityPosition.z - front.z * speed
    end
    if input.is_key_pressed(input.keycode.A) then
        entityPosition.x = entityPosition.x - right.x * speed
        entityPosition.y = entityPosition.y - right.y * speed
        entityPosition.z = entityPosition.z - right.z * speed
    end
    if input.is_key_pressed(input.keycode.D) then
        entityPosition.x = entityPosition.x + right.x * speed
        entityPosition.y = entityPosition.y + right.y * speed
        entityPosition.z = entityPosition.z + right.z * speed
    end

    -- Update position
    transform.position = entityPosition

    return 1
end

function OnDestroy()
    print("FPS Camera Controller Destroyed")
    return 1
end
