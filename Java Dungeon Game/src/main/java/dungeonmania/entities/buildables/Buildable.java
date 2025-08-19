package dungeonmania.entities.buildables;

import dungeonmania.Game;
import dungeonmania.entities.BattleItem;
import dungeonmania.entities.inventory.InventoryItem;

public abstract class Buildable extends InventoryItem implements BattleItem {
    private int durability;

    public Buildable(int durability) {
        super(null);
        this.durability = durability;
    }

    @Override
    public void use(Game game) {
        durability--;
        if (durability <= 0) {
            game.getPlayer().remove(this);
        }
    }

    @Override
    public int getDurability() {
        return durability;
    }
}
