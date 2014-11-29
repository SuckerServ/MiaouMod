local server = {}

local properties = core.vars

setmetatable(server,{
    
    __index = function(table, key)
        
        local value = core[key]
        
        if not value then
            value = properties[key]
            if type(value) == "function" then
                value = value()
            end
        end
        
        return value
    end,
    
    __newindex = function(table, key, value)
        
        local existing_property = properties[key]
        
        if existing_property and type(existing_property) == "function" then
            existing_property(value)
        else
            core[key] = value
        end
    end
});

return server
