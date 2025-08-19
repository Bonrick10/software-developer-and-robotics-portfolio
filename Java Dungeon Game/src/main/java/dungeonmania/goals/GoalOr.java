package dungeonmania.goals;

import dungeonmania.Game;

public class GoalOr extends Goal {
    private static final String OR = " OR ";
    private Goal goal1;
    private Goal goal2;

    public GoalOr(Goal goal1, Goal goal2) {
        this.goal1 = goal1;
        this.goal2 = goal2;
    }

    @Override
    public boolean checkAchieve(Game game) {
        return goal1.achieved(game) || goal2.achieved(game);
    }

    @Override
    public String returnString(Game game) {
        return "(" + goal1.toString(game) + OR + goal2.toString(game) + ")";
    }
}
