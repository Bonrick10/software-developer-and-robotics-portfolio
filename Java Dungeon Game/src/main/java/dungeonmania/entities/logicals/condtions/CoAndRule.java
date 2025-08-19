package dungeonmania.entities.logicals.condtions;

import java.util.List;
import java.util.Map;

import dungeonmania.entities.logicals.Conductor;
import dungeonmania.entities.logicals.Logical;
import dungeonmania.map.GameMap;

public class CoAndRule implements LogicRule {
    @Override
    public boolean isSatisfied(GameMap map, Logical logical) {
        List<Conductor> adjConductors = logical.getAdjConductors();
        if (adjConductors.size() == 0) {
            return false;
        }
        Map<Conductor, Integer> activeTickCounter = logical.getActiveTickCounter();
        int commonTick = -1;
        for (Conductor c : adjConductors) {
            if (!c.isConduct()) {
                return false;
            }

            if (commonTick == -1) {
                commonTick = activeTickCounter.get(c);
            }

            if (commonTick != activeTickCounter.get(c)) {
                return false;
            }
        }
        return true;
    }

}
