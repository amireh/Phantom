local process = function(inCaster, inTarget, inSpell)
	Pixy.Log("Casting Chains of Command on " .. inTarget:getName() .. "#" .. inTarget:getUID() .. " by " .. inCaster:getUID() .. "!")

	--[[
  if (inCaster:getWP() <= inSpell:getCostWP()) then
	  Pixy.Log("ERROR! Puppet doesn't have enough WP to cast!")
	  return false
	end

  if (not inTarget) then
    Pixy.Log("ERROR! Casting Chains of Command without a target!")
    return false
  end
  --]]

  -- must not be cast on self
  -- must not be cast on a puppet
  -- must not be cast on a unit under the caster's command
  if (inTarget:getUID() == inCaster:getUID() or
      inTarget:getRank() == Pixy.PUPPET or
      inTarget:getOwner() == inCaster) then
    Pixy.Log("ERROR! Caster is casting Chains of Command on self!")
    return false
  end

  -- create the cast spell event
  --~ do
    --~ local evt = Pixy.Event:new()
    --~ evt.UID = Pixy.EventUID.CastSpell
    --~ evt.Feedback = Pixy.EventFeedback.Ok
    --~ evt:setProperty("Spell", inSpell:getUID())
    --~ evt:setProperty("T", inTarget:getUID())
    --~ Instance:broadcast(evt)
    --~ evt:delete()
  --~ end

  -- change the ownership of the unit
  inTarget:getOwner():detachUnit(inTarget:getUID(), false)
  tolua.cast(inTarget, "Pixy::Unit")
  inCaster:attachUnit(inTarget)

  -- update the puppet stats
  --~ do
    --~ local evt = Pixy.Event:new()
    --~ evt.UID = Pixy.EventUID.UpdatePuppet
    --~ evt.Feedback = Pixy.EventFeedback.Ok
    --~ evt:setProperty("UID", inCaster:getUID())
    --~ apply_spell_cost(inSpell, inCaster, evt)
    --~ Instance:broadcast(evt)
    --~ evt:delete()
  --~ end

  --[[do
    inTarget:setOwner(inCaster)

    local evt = Pixy.Event:new()
    evt.UID = Pixy.EventUID.UpdateUnit
    evt.Feedback = Pixy.EventFeedback.Ok
    evt:setProperty("UID", inTarget:getUID())
    evt:setProperty("O", inCaster:getUID())
    Instance:broadcast(evt)
    evt:delete()
  end]]

	return true
end

subscribe_spell("Chains of Command", process)
