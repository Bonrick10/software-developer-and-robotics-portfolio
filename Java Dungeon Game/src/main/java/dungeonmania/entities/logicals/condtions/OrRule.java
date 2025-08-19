package dungeonmania.entities.logicals.condtions;

import java.util.List;

import dungeonmania.entities.logicals.Conductor;
import dungeonmania.entities.logicals.Logical;
import dungeonmania.map.GameMap;

public class OrRule implements LogicRule {
    @Override
    public boolean isSatisfied(GameMap map, Logical logical) {
        List<Conductor> adjConductors = logical.getAdjConductors();
        if (adjConductors.size() == 0) {
            return false;
        }
        for (Conductor c : adjConductors) {
            if (c.isConduct()) {
                return true;
            }
        }
        return false;
    }

}
