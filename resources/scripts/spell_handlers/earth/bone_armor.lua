local process = function(inCaster, inTarget, inSpell)
	Pixy.Log("Casting Sudden Craven on " .. inCaster:getName() .. "#" .. inCaster:getUID())
  tolua.cast(inCaster, "Pixy::Puppet")

  if (inTarget:getUID() == inCaster:getUID()
  or inTarget:getRank() == Pixy.PUPPET
  or inTarget:getOwner():getUID() ~= inCaster:getUID()
  ) then
    Pixy.Log("ERROR! Invalid target for Bone Armor!")
    return false
  end

  inTarget:setBaseAP(inTarget:getBaseAP() + 1)
  inTarget:setBaseHP(inTarget:getBaseHP() + 1)

  local e = Pixy.Event(Pixy.EventUID.UpdateUnit)
  e:setProperty("UID", inTarget:getUID())
  e:setProperty("BHP", inTarget:getBaseHP())
  e:setProperty("BAP", inTarget:getBaseAP())
  Instance:broadcast(e)

	return true
end

subscribe_spell("Bone Armor", process)
