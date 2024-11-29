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
end

function on_destroy()
    -- print("OnDestroy()")
end