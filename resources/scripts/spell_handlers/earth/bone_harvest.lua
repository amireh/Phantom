local BoneHarvest = {}
function BoneHarvest:cast()
  self:logMe()

  if self.Caster:getRank() ~= Pixy.PUPPET then return false end

  local exporter = Pixy.UnitListExporter()
  exporter:export(Instance, self.Caster:getUnits(), "Pixy::Unit", "Temp")

  local nr_skeletons = 0
  for unit in list_iter(Temp) do
    if unit:getName():find("Skeleton") then
      nr_skeletons = nr_skeletons + 1
    end
  end

  if nr_skeletons == 0 then return false end

  self.Caster:setHP(self.Caster:getHP() + nr_skeletons)
  local e = Pixy.Event(Pixy.EventUID.UpdatePuppet)
  e:setProperty("UID", self.Caster:getUID())
  e:setProperty("HP", self.Caster:getHP())
  Instance:broadcast(e)

  return true
end

subscribe_spell("Bone Harvest", BoneHarvest)
