local BoneArmor = { IsPositive = true }
function BoneArmor:cast()
  self:logMe()

  if self.Target:getUID() == self.Caster:getUID()
  or self.Target:getRank() == Pixy.PUPPET
  or self.Target:getOwner():getUID() ~= self.Caster:getUID()
  then
    Pixy.Log("ERROR! Invalid target for Bone Armor!")
    return false
  end

  self.Target:setBaseAP(self.Target:getBaseAP() + 1)
  self.Target:setBaseHP(self.Target:getBaseHP() + 1)

  local e = Pixy.Event(Pixy.EventUID.UpdateUnit)
  e:setProperty("UID", self.Target:getUID())
  e:setProperty("BHP", self.Target:getBaseHP())
  e:setProperty("BAP", self.Target:getBaseAP())
  Instance:broadcast(e)

	return true
end

function BoneArmor:process()
end

subscribe_spell("Bone Armor", BoneArmor)
