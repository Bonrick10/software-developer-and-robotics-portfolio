package dungeonmania.goals;

import dungeonmania.Game;

public class GoalTreasure extends Goal {
    private static final String TREASURE = ":treasure";
    private int target;

    public GoalTreasure(int target) {
        this.target = target;
    }

    @Override
    public boolean checkAchieve(Game game) {
        return game.getCollectedTreasureCount() >= target;
    }

    @Override
    public String returnString(Game game) {
        return TREASURE;
    }
}
