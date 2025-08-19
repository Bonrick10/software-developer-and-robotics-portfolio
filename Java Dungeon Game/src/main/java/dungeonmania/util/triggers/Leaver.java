package dungeonmania.util.triggers;

import dungeonmania.entities.Entity;
import dungeonmania.map.GameMap;

public interface Leaver {
    public void onMovedAway(GameMap map, Entity entity);
}
