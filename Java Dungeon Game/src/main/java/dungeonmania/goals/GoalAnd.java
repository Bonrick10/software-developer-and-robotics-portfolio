package dungeonmania.goals;

import dungeonmania.Game;

public class GoalAnd extends Goal {
    private static final String AND = " AND ";
    private Goal goal1;
    private Goal goal2;

    public GoalAnd(Goal goal1, Goal goal2) {
        this.goal1 = goal1;
        this.goal2 = goal2;
    }

    @Override
    public boolean checkAchieve(Game game) {
        return goal1.achieved(game) && goal2.achieved(game);
    }

    @Override
    public String returnString(Game game) {
        return "(" + goal1.toString(game) + AND + goal2.toString(game) + ")";
    }
}
