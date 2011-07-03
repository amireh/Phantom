local Handlers = {}

function subscribeToSpell(inSpellName, inMethod)
	Pixy.Log("subscribing to " .. inSpellName)
	Handlers[inSpellName] = inMethod
end

function processSpell(inCaster, inSpell, inEvt)
  --tolua.cast(inCaster, "Pixy::Puppet")
  --tolua.cast(inSpell, "Pixy::Spell")
	Pixy.Log("I have a spell named '" .. inSpell:getName() .. "' to handle!")

	local spellHandler = Handlers[inSpell:getName()]

	if not spellHandler then return true else return spellHandler(inCaster, inSpell) end

end
