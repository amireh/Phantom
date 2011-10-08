local process = function(inCaster, inTarget, inSpell)
  -- caster must be a puppet
  if inCaster:getRank() ~= Pixy.PUPPET or inTarget:getRank() == Pixy.PUPPET then return false end

  -- target unit must be owned by the caster
  if inTarget:getOwner():getUID() ~= inCaster:getUID() then return false end

  Instance:destroy_unit(inTarget:getUID())

  inCaster:setChannels(inCaster:getChannels() + 2)
  local e = Pixy.Event(Pixy.EventUID.UpdatePuppet)
  e:setProperty("UID", inCaster:getUID())
  e:setProperty("Channels", inCaster:getChannels())
  Instance:broadcast(e)

	return true
end

subscribe_spell("Death Exchange", process)
