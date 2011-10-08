local turns_left = 0
local caster = nil
-- Shadow Dance increases the AP of units by 1 until the end of turn
local _ap = 1

local increase_ap = function(unit)
  if unit:getOwner():getUID() == caster:getUID() then
    unit:setBaseAP(unit:getBaseAP() + _ap)
  end

  return true
end

local revert_ap = function(unit)
  if unit:getOwner():getUID() == caster:getUID() then
    unit:setBaseAP(unit:getBaseAP() - _ap)
  end

  return true
end

local apply_buff = function(inCaster, inTarget, inSpell)
  if inCaster:getRank() ~= Pixy.PUPPET then return false end
  caster = inCaster

  -- attach the buff to the puppet
  inCaster:attachBuff(inSpell)
  turns_left = inSpell:getDuration()
  inSpell:setExpired(false)

  -- increase the cost of all the puppet's current spells
  local exporter = Pixy.UnitListExporter()
  exporter:export(Instance, inCaster:getUnits(), "Pixy::Unit", "Temp")
  for unit in list_iter(Temp) do
    increase_ap(unit)
  end
  Temp = nil

  -- increase the cost of all spells to be drawn by the puppet
  subscribe_generic_unit_handler("Alive", increase_ap)

  return true
end

local process_buff = function(inCaster, inTarget, inSpell)

  turns_left = turns_left - 1
  -- remove spell when the duration expires
  if (turns_left == 0) then
    inSpell:setExpired(true)

    -- revert the cost of all the puppet's current spells
    local exporter = Pixy.UnitListExporter()
    exporter:export(Instance, inCaster:getUnits(), "Pixy::Unit", "Temp")
    for unit in list_iter(Temp) do
      revert_ap(unit)
    end
    Temp = nil
    unsubscribe_generic_unit_handler("Alive", increase_ap)
  end

  return true
end

local process = function(inCaster, inTarget, inSpell)
  if (inCaster:hasBuff(inSpell:getUID())) then
    return process_buff(inCaster, inTarget, inSpell)
  else
    return apply_buff(inCaster, inTarget, inSpell)
  end

end

subscribe_spell("Shadow Dance", process)
