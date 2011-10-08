local damage = 2
local lifetap = 3
local process = function(inCaster, inTarget, inSpell)
  inTarget = Instance:get_enemy(inCaster:getUID())
	Pixy.Log("Casting Kiss of Death on " .. inTarget:getName() .. "#" .. inTarget:getUID() .. " by " .. inCaster:getUID() .. "!")

  -- update the target stats
  do
    inTarget:setHP(inTarget:getHP() - damage)
    inCaster:setHP(inCaster:getHP() + lifetap)

    local e = Pixy.Event(Pixy.EventUID.UpdatePuppet)
    e:setProperty("UID", inCaster:getUID())
    e:setProperty("HP", inCaster:getHP())
    Instance:broadcast(e)

    e:setProperty("UID", inTarget:getUID())
    e:setProperty("HP", inTarget:getHP())
    Instance:broadcast(e)

  end

	return true
end

subscribe_spell("Kiss of Death", process)
