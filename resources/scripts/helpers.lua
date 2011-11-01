function list_iter(t)
  local i = 0
  local n = table.getn(t)
  return function ()
    i = i + 1
    if i <= n then return t[i] else return nil end
  end
end

function rlist_iter(t)
  local n = table.getn(t)
  local i = n+1
  return function ()
    i = i - 1
    if i > 0 then return t[i] else return nil end
  end
end

function removeByValue(inTable, inValue)
	local i = 1
	for val in list_iter(inTable) do
	  if val == inValue then
	    table.remove(inTable, i)
			break
	  end
	  i = i+1
	end
end

function apply_spell_cost(spell, caster, evt)
  if (caster:getRank() == Pixy.PUPPET) then
    if (spell:getCostWp() > 0) then
      caster:setWP(caster:getWP() - spell:getCostWP())
      evt.setProperty("WP", caster:getWP())
    end
    if (spell:getCostChannels() > 0) then
      caster:setChannels(caster:getChannels() - spell:getCostChannels())
      evt.setProperty("Channels", caster:getChannels())
    end
  end

  if (spell:getCostHP() > 0) then
    caster:setHP(caster:getHP() - spell:getCostHP())
    evt.setProperty("HP", caster:getHP())
  end
end

function all_words(str)
  local t = {}
  local b = 0
  local e = 0
  b,e = str:find("%w+", e+1)
  while b do
    table.insert(t, str:sub(b,e))
    b,e = str:find("%w+", e+1)
  end
  return t
end
