local process = function(inCaster, inSpell)
	Pixy.Log("I'm summoning a Fetish: Zij!")
  tolua.cast(inCaster, "Pixy::Puppet")

	if (inCaster:getWP() <= inSpell:getCostWP()) then
	  Pixy.Log("ERROR! Puppet doesn't have enough WP to cast!")
	  return false
	end

	-- create the unit and broadcast it to the players
  Instance:create_unit("Zij", inCaster)

  -- create the cast spell event
  do
    local evt = Pixy.Event:new()
    evt.UID = Pixy.EventUID.CastSpell
    evt:setProperty("Spell", inSpell:getUID())
    Instance:broadcast(evt)
  end

  -- update the puppet stats
  do
    inCaster:setWP(inCaster:getWP() - inSpell:getCostWP())
    local evt = Pixy.Event:new()
    evt.UID = Pixy.EventUID.UpdatePuppet
    evt:setProperty("WP", inCaster:getWP())
    Instance:broadcast(evt)
  end

	return true
end

subscribeToSpell("Summon: Fetish Zij", process)
