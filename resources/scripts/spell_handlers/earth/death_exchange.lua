local DeathExchange = {}
function DeathExchange:cast()
  -- caster must be a puppet
  if self.Caster:getRank() ~= Pixy.PUPPET or self.Target:getRank() == Pixy.PUPPET then return false end

  -- target unit must be owned by the caster
  if self.Target:getOwner():getUID() ~= self.Caster:getUID() then return false end

  Instance:destroy_unit(self.Target:getUID())

  self.Caster:setChannels(self.Caster:getChannels() + 2)
  local e = Pixy.Event(Pixy.EventUID.UpdatePuppet)
  e:setProperty("UID", self.Caster:getUID())
  e:setProperty("Channels", self.Caster:getChannels())
  Instance:broadcast(e)

	return true
end

subscribe_spell("Death Exchange", DeathExchange)
