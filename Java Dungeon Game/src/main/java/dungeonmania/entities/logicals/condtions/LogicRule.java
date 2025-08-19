package dungeonmania.entities.logicals.condtions;

import dungeonmania.entities.logicals.Logical;
import dungeonmania.map.GameMap;

public interface LogicRule {
    public boolean isSatisfied(GameMap map, Logical logical);
}
