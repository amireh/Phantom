local process = function(inCaster, inTarget, inSpell)
	Pixy.Log("Casting Sudden Craven on " .. inCaster:getName() .. "#" .. inCaster:getUID())

  if inCaster:getRank() ~= Pixy.PUPPET then return false end

  local exporter = Pixy.UnitListExporter()
  exporter:export(Instance, inCaster:getUnits(), "Pixy::Unit", "Temp")

  local nr_skeletons = 0
  for unit in list_iter(Temp) do
    if unit:getName():find("Skeleton") then
      nr_skeletons = nr_skeletons + 1
    end
  end

  if nr_skeletons == 0 then return false end

  inCaster:setHP(inCaster:getHP() + nr_skeletons)

	return true
end

subscribe_spell("Bone Harvest", process)
