local process = function(inCaster, inSpell)
	Pixy.Log("I'm summoning a Fetish: Herdkeeper!")
  tolua.cast(inCaster, "Pixy::Puppet")

	if (inCaster:getWP() <= inSpell:getCostWP()) then
	  Pixy.Log("ERROR! Puppet doesn't have enough WP to cast!")
	  return false
	end

	-- create the unit and broadcast it to the players
  Instance:create_unit("Herdkeeper", inCaster)

  -- create the cast spell event
  do
    local evt = Pixy.Event:new()
    evt.UID = Pixy.EventUID.CastSpell
    evt.Feedback = Pixy.EventFeedback.Ok
    evt:setProperty("Spell", inSpell:getUID())
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

	return true
end

subscribeToSpell("Summon: Fetish Herdkeeper", process)
