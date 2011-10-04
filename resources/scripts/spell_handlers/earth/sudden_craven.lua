local process = function(inCaster, inTarget, inSpell)
	Pixy.Log("Casting Sudden Craven on " .. inCaster:getName() .. "#" .. inCaster:getUID())
  tolua.cast(inCaster, "Pixy::Puppet")

  Instance:draw_spells(inCaster:getUID(), 2)

	return true
end

subscribe_spell("Sudden Craven", process)
