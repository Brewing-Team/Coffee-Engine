function OnCreate()
    print("OnCreate()")
    log("OnCreate()")
    log_error("OnCreate()")
    log_warning("OnCreate()")
    log_critical("OnCreate()")
end

function OnUpdate()
    -- print("OnUpdate()")
    if is_key_pressed(keycode.SPACE) then
        log("SPACE")
    end

    if is_mouse_button_pressed(mousecode.LEFT) then
        log("LEFT")
        local x, y = get_mouse_position()
        log("Mouse position: (" .. x .. ", " .. y .. ")")
    end
end

function on_destroy()
    -- print("OnDestroy()")
end