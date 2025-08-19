package dungeonmania.entities.logicals;

import java.util.List;
import java.util.Map;

import dungeonmania.entities.logicals.condtions.LogicRule;
import dungeonmania.map.GameMap;

public interface Logical {
    public LogicRule getCondition();

    public List<Conductor> getAdjConductors();

    public Map<Conductor, Integer> getActiveTickCounter();

    public void subscribe(Conductor c);

    public void initTickCounter();

    public void updateTickCounter();

    public void findAdjConductors(GameMap map);

    public void checkCondition(GameMap map);
}
