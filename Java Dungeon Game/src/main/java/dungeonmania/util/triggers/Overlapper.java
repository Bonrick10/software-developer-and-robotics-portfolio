package dungeonmania.util.triggers;

import dungeonmania.entities.Entity;
import dungeonmania.map.GameMap;

public interface Overlapper {
    public void onOverlap(GameMap map, Entity entity);
}
