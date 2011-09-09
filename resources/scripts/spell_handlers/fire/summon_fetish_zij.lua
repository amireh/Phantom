local process = function(inCaster, inTarget, inSpell)
	Pixy.Log("I'm summoning a Fetish: Zij!")
  tolua.cast(inCaster, "Pixy::Puppet")

  --[[
  if (inCaster:getWP() <= inSpell:getCostWP()) then
	  Pixy.Log("ERROR! Puppet doesn't have enough WP to cast!")
	  return false
	end
  ]]--

	-- create the unit and broadcast it to the players
  local unit = Instance:create_unit("Zij", inCaster)

  -- create the cast spell event
  --~ do
    --~ local evt = Pixy.Event:new()
    --~ evt.UID = Pixy.EventUID.CastSpell
    --~ if (unit) then
      --~ evt.Feedback = Pixy.EventFeedback.Ok
    --~ else
      --~ evt.Feedback = Pixy.EventFeedback.InvalidRequest
    --~ end
    --~ evt:setProperty("Spell", inSpell:getUID())
    --~ Instance:broadcast(evt)
    --~ evt:delete()
  --~ end

  if (not unit) then return false end

  -- update the puppet stats
  --~ do
    --~ --inCaster:setWP(inCaster:getWP() - inSpell:getCostWP())
--~
    --~ local evt = Pixy.Event:new()
    --~ evt.UID = Pixy.EventUID.UpdatePuppet
    --~ evt.Feedback = Pixy.EventFeedback.Ok
    --~ evt:setProperty("UID", inCaster:getUID())
--~
    --~ apply_spell_cost(inSpell, inCaster, evt)
--~
    --~ --evt:setProperty("WP", inCaster:getWP())
--~
    --~ Instance:broadcast(evt)
    --~ evt:delete()
  --~ end

	return true
end

subscribe_spell("Summon: Fetish Zij", process)
