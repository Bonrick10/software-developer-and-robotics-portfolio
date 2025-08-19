package dungeonmania.entities.enemies.enemyMoveStrategy;

import dungeonmania.Game;
import dungeonmania.entities.enemies.Enemy;
import dungeonmania.util.Position;

public interface EnemyMoveStrategy {
    public Position nextMove(Game game, Enemy enemy);
}
