package dungeonmania.entities.logicals;

import java.util.ArrayList;
import java.util.List;

import dungeonmania.entities.Entity;
import dungeonmania.entities.Switch;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class Wire extends Entity implements Conductor {
    private List<Switch> sources = new ArrayList<>();

    public Wire(Position position) {
        super(position);
    }

    @Override
    public boolean canMoveOnto(GameMap map, Entity entity) {
        return true;
    }

    public List<Switch> getSources() {
        return sources;
    }

    public void subscribe(Switch s) {
        sources.add(s);
    }

    public void shareSubs(Wire w) {
        for (Switch s : sources) {
            if (!w.containsSource(s)) {
                w.subscribe(s);
                s.subscribe(w);
            }
        }
    }

    public boolean containsSource(Switch s) {
        return sources.contains(s);
    }

    public boolean subsAdjacent(GameMap map) {
        boolean found = false;
        List<Wire> wires = map.getEntities(Wire.class);
        for (Wire w : wires) {
            if (Position.isAdjacent(getPosition(), w.getPosition())
                    && !(sources.containsAll(w.getSources()) && w.getSources().containsAll(sources))) {
                shareSubs(w);
                w.shareSubs(this);
                found = true;
            }
        }
        return found;
    }

    @Override
    public boolean isConduct() {
        for (Switch s : sources) {
            if (s.isConduct()) {
                return true;
            }
        }
        return false;
    }
}
