package dungeonmania.goals;
import dungeonmania.Game;

public class GoalEnemy extends Goal {
    private static final String ENEMY = ":enemies";
    private int target;

    public GoalEnemy(int target) {
        this.target = target;
    }

    @Override
    public boolean checkAchieve(Game game) {
        // all spawners need to be destroyed and target amount of enemies needs to be killed
        return game.getEnemyKillCount() >= target && game.getSpawnerDestroyCount() == game.getSpawnerTotalCount();
    }

    @Override
    public String returnString(Game game) {
        return ENEMY;
    }
}
