local grant_lifetap = function(e)
  local unit = e.Any
  if not unit:hasLifetap() then unit:setHasLifetap(true) end
end

local warlord_alive = function(e)
  -- Warlord grants Lifetap to all ally skeletons
  local exporter = Pixy.UnitListExporter()
  exporter:export(Instance, inCaster:getUnits(), "Pixy::Unit", "Temp")
  for unit in list_iter(Temp) do
    print("Lua: granting Lifetap to " .. unit:getName())
    if not unit:hasLifetap() then unit:setHasLifetap(true) end
  end
  Temp = nil

  subscribe_generic_unit_handler("Alive", grant_lifetap)
end

local warlord_dead = function(e)
  -- Warlord grants Lifetap to all ally skeletons
  -- if there are no other warlords alive, we withdraw lifetap from the units
  local exporter = Pixy.UnitListExporter()
  exporter:export(Instance, inCaster:getUnits(), "Pixy::Unit", "Temp")
  local warlord_found = false
  for unit in list_iter(Temp) do
    warlord_found = warlord or unit:getName() == "Skeleton Warlord"
  end

  if warlord_found then
    Pixy.Log("another Warlord exists, not draining lifetap")
  else
    for unit in list_iter(Temp) do
      unit:setHasLifetap(false)
    end

    unsubscribe_generic_unit_handler("Alive", grant_lifetap)
  end

  Temp = nil

  return true
end

subscribe_unit("Skeleton Warlord", "Alive", warlord_alive)
subscribe_unit("Skeleton Warlord", "Dead", warlord_alive)
