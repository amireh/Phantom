local process = function(inCaster, inTarget, inSpell)
  if inTarget:getRank() == Pixy.PUPPET then return false end
  if inCaster:getRank() ~= Pixy.PUPPET then return false end
  if inTarget:getOwner():getUID() == inCaster:getUID() then return false end
  if not inTarget:hasSummoningSickness() then return false end

	Pixy.Log("Casting Kiss of Death on " .. inTarget:getName() .. "#" .. inTarget:getUID() .. " by " .. inCaster:getUID() .. "!")

  Instance:destroy_unit(inTarget:getUID())

	return true
end

subscribe_spell("Harlequin Curse", process)
