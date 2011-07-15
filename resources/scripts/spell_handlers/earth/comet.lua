local damage = 4
local process = function(inCaster, inTarget, inSpell)
	Pixy.Log("Casting Comet on " .. inTarget:getName() .. "#" .. inTarget:getUID() .. " by " .. inCaster:getUID() .. "!")
  tolua.cast(inCaster, "Pixy::Puppet")

	if (inCaster:getWP() <= inSpell:getCostWP()) then
	  Pixy.Log("ERROR! Puppet doesn't have enough WP to cast!")
	  return false
	end

  if (not inTarget) then
    Pixy.Log("ERROR! Casting Comet without a target!")
    return false
  end

  if (inTarget:getUID() == inCaster:getUID()) then
    Pixy.Log("ERROR! Caster is casting Comet on self!")
    return false
  end

  -- create the cast spell event
  do
    local evt = Pixy.Event:new()
    evt.UID = Pixy.EventUID.CastSpell
    evt.Feedback = Pixy.EventFeedback.Ok
    evt:setProperty("Spell", inSpell:getUID())
    evt:setProperty("T", inTarget:getUID())
    Instance:broadcast(evt)
    evt:delete()
  end

  -- update the puppet stats
  do
    inCaster:setWP(inCaster:getWP() - inSpell:getCostWP())

    local evt = Pixy.Event:new()
    evt.UID = Pixy.EventUID.UpdatePuppet
    evt.Feedback = Pixy.EventFeedback.Ok
    evt:setProperty("UID", inCaster:getUID())
    evt:setProperty("WP", inCaster:getWP())
    Instance:broadcast(evt)
    evt:delete()
  end

  -- update the target stats
  do
    inTarget:setHP(inTarget:getHP() - damage)

    local evt = Pixy.Event:new()
    if (inTarget:getRank() == Pixy.PUPPET) then
      evt.UID = Pixy.EventUID.UpdatePuppet
    else
      evt.UID = Pixy.EventUID.UpdateUnit
    end
    evt.Feedback = Pixy.EventFeedback.Ok
    evt:setProperty("UID", inTarget:getUID())
    evt:setProperty("HP", inTarget:getHP())
    Instance:broadcast(evt)
    evt:delete()
  end

  if (inTarget:isDead()) then
    inTarget:getOwner():detachUnit(inTarget:getUID())
  end

	return true
end

subscribe_spell("Comet", process)
