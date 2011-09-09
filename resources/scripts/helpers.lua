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
