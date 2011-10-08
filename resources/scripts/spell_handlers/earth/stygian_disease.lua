local turns_left = 0
local _dmg = 1
local apply_buff = function(inCaster, inTarget, inSpell)
  Pixy.Log("Applying Stygian Disease on " .. inTarget:getName() .. "#" .. inTarget:getUID() .. " by " .. inCaster:getName() .. "!")

  -- can only be cast on enemy units or puppet
  if inTarget:getRank() == Pixy.PUPPET then return false end

  -- attach the buff to the puppet
  inTarget:attachBuff(inSpell)
  turns_left = inSpell:getDuration()
  inSpell:setExpired(false)

  return true
end

local process_buff = function(inCaster, inTarget, inSpell)
  Pixy.Log("Processing Stygian Disease on " .. inCaster:getName() .. "#" .. inCaster:getUID() .. "!")
  inTarget:getOwner():setHP(inTarget:getOwner():getHP() - _dmg)

  turns_left = turns_left - 1
  -- remove spell when the duration expires
  if (turns_left == 0) then
    inSpell:setExpired(true)
  end

  return true
end

local process = function(inCaster, inTarget, inSpell)
  if (inTarget:hasBuff(inSpell:getUID())) then
    return process_buff(inCaster, inTarget, inSpell)
  else
    return apply_buff(inCaster, inTarget, inSpell)
  end

end

subscribe_spell("Stygian Disease", process)
