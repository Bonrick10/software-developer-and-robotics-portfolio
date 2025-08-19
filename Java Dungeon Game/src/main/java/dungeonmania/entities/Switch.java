package dungeonmania.entities;

import java.util.ArrayList;
import java.util.List;

import dungeonmania.entities.collectables.Bomb;
import dungeonmania.entities.logicals.Conductor;
import dungeonmania.entities.logicals.Wire;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;
import dungeonmania.util.triggers.Leaver;
import dungeonmania.util.triggers.Overlapper;

public class Switch extends Entity implements Overlapper, Leaver, Conductor {
    private boolean activated;
    private List<Bomb> bombs = new ArrayList<>();
    private List<Wire> wires = new ArrayList<>();

    public Switch(Position position) {
        super(position.asLayer(Entity.ITEM_LAYER));
    }

    public void subscribe(Bomb b) {
        bombs.add(b);
    }

    public void subscribe(Wire w) {
        wires.add(w);
    }

    public void subscribe(Bomb bomb, GameMap map) {
        bombs.add(bomb);
        if (activated) {
            bombs.stream().forEach(b -> b.notify(map));
        }
    }

    public void unsubscribe(Bomb b) {
        bombs.remove(b);
    }

    public void unsubscribe(Wire w) {
        wires.remove(w);
    }

    @Override
    public boolean canMoveOnto(GameMap map, Entity entity) {
        return true;
    }

    @Override
    public void onOverlap(GameMap map, Entity entity) {
        if (entity instanceof Boulder) {
            activated = true;
            bombs.stream().forEach(b -> b.notify(map));
        }
    }

    @Override
    public void onMovedAway(GameMap map, Entity entity) {
        if (entity instanceof Boulder) {
            activated = false;
        }
    }

    @Override
    public boolean isConduct() {
        return activated;
    }
}
