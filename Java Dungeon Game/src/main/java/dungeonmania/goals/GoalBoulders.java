package dungeonmania.goals;

import dungeonmania.Game;
import dungeonmania.entities.Switch;

public class GoalBoulders extends Goal {
    private static final String BOULDERS = ":boulders";

    @Override
    public boolean checkAchieve(Game game) {
        return game.getMap().getEntities(Switch.class).stream().allMatch(s -> s.isConduct());
    }

    @Override
    public String returnString(Game game) {
        return BOULDERS;
    }
}
