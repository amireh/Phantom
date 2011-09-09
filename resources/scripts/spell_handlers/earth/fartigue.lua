local turns_left = 0
local apply_buff = function(inCaster, inTarget, inSpell)
  Pixy.Log("Applying Fartigue on " .. inTarget:getName() .. "#" .. inTarget:getUID() .. " by " .. inCaster:getName() .. "!")

  if (inCaster:getWP() <= inSpell:getCostWP()) then
	  Pixy.Log("ERROR! Puppet doesn't have enough WP to cast!")
	  return false
	end
  -- can only be cast on enemy units or puppet
  if (inTarget:getOwner() == inCaster) then return false end

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

  -- attach the buff to the puppet
  inTarget:attachBuff(inSpell)
  turns_left = inSpell:getDuration()
  inSpell:setExpired(false)

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

  return true
end

local process_buff = function(inCaster, inTarget, inSpell)
  Pixy.Log("Processing Fartigue on " .. inCaster:getName() .. "#" .. inCaster:getUID() .. "!")
  inTarget:setHP(inTarget:getHP() - 2)

  if (inTarget:isDead()) then
    return true
  end

  turns_left = turns_left - 1
  -- remove spell when the duration expires
  if (turns_left == 0) then
    inSpell:setExpired(true)
  end

  return true
end

local process = function(inCaster, inTarget, inSpell)
  tolua.cast(inCaster, "Pixy::Puppet")

  if (inTarget:hasBuff(inSpell:getUID())) then
    return process_buff(inCaster, inTarget, inSpell)
  else
    return apply_buff(inCaster, inTarget, inSpell)
  end

end

subscribe_spell("Fartigue", process)
