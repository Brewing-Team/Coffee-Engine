-- Task list
tasks = {
    {id = 1, description = "Mision 1"},
    {id = 2, description = "Mision 2"},
    {id = 3, description = "Mision 3"}
}

local currentTaskIndex = 1  -- Current task index

-- Initialize tasks when the engine is ready
function on_ready()
    getCurrentTask()  -- Display the current task
end

-- Perform task completion check in each frame update
function on_update(dt)

    local playerHasKilledWolf = true

    if playerHasKilledWolf then
        completeCurrentTask()  
    end

end

-- Cleanup when the game exits
function on_exit()
    print("Mission cleared!")
end



-- Complete the current task
function completeCurrentTask()
    if currentTaskIndex > #tasks then
        print("All missions done!")
        return
    end

    print("Mission done: " .. tasks[currentTaskIndex].description)
    currentTaskIndex = currentTaskIndex + 1  -- Move to the next task
end

-- Get the current task
function getCurrentTask()
    if currentTaskIndex > #tasks then
        print("No mission")
    else
        print("Current mission: " .. tasks[currentTaskIndex].description)
    end
end
