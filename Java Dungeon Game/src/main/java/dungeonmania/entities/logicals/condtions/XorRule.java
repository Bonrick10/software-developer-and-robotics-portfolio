package dungeonmania.entities.logicals.condtions;

import java.util.List;

import dungeonmania.entities.logicals.Conductor;
import dungeonmania.entities.logicals.Logical;
import dungeonmania.map.GameMap;

public class XorRule implements LogicRule {
    @Override
    public boolean isSatisfied(GameMap map, Logical logical) {
        List<Conductor> adjConductors = logical.getAdjConductors();
        if (adjConductors.size() == 0) {
            return false;
        }
        boolean found = false;
        for (Conductor c : adjConductors) {
            if (c.isConduct()) {
                if (!found) {
                    found = true;
                } else {
                    return false;
                }
            }
        }
        return found;
    }

}
