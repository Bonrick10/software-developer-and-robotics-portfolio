package dungeonmania.entities.logicals;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import dungeonmania.entities.Entity;
import dungeonmania.entities.enemies.Spider;
import dungeonmania.entities.logicals.condtions.LogicRule;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class SwitchDoor extends Entity implements Logical {
    private LogicRule condition;
    private List<Conductor> adjConductors = new ArrayList<>();
    private Map<Conductor, Integer> activeTickCounter;
    private boolean open = false;

    public SwitchDoor(Position position, LogicRule condition) {
        super(position);
        this.condition = condition;
    }

    @Override
    public boolean canMoveOnto(GameMap map, Entity entity) {
        return open || entity instanceof Spider;
    }

    @Override
    public void checkCondition(GameMap map) {
        this.open = condition.isSatisfied(map, this);
    }

    public boolean isOpen() {
        return open;
    }

    @Override
    public LogicRule getCondition() {
        return condition;
    }

    @Override
    public List<Conductor> getAdjConductors() {
        return adjConductors;
    }

    @Override
    public Map<Conductor, Integer> getActiveTickCounter() {
        return activeTickCounter;
    }

    @Override
    public void subscribe(Conductor c) {
        adjConductors.add(c);
    }

    @Override
    public void initTickCounter() {
        activeTickCounter = new HashMap<Conductor, Integer>();
        for (Conductor adjC : adjConductors) {
            activeTickCounter.put(adjC, 0);
        }
    }

    @Override
    public void updateTickCounter() {
        for (Conductor adjC : adjConductors) {
            if (!adjC.isConduct()) {
                activeTickCounter.put(adjC, 0);
            } else {
                activeTickCounter.put(adjC, activeTickCounter.get(adjC) + 1);
            }
        }
    }

    @Override
    public void findAdjConductors(GameMap map) {
        adjConductors = new ArrayList<>();
        List<Position> cardinals = getPosition().getCardinallyAdjacentPositions();
        for (Position pos : cardinals) {
            List<Entity> entites = map.getEntities(pos);
            for (Entity e : entites) {
                if (e instanceof Conductor) {
                    subscribe((Conductor) e);
                    break;
                }
            }
        }
    }

}
