package dungeonmania.entities.enemies.enemyMoveStrategy;

import dungeonmania.Game;
import dungeonmania.entities.Player;
import dungeonmania.entities.enemies.Enemy;
import dungeonmania.entities.enemies.Mercenary;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class MercAllyStrat implements EnemyMoveStrategy {
    @Override
    public Position nextMove(Game game, Enemy enemy) {
        Position nextPos;
        GameMap map = game.getMap();
        Player player = game.getPlayer();
        Mercenary merc = (Mercenary) enemy;
        nextPos = merc.isAdjacentToPlayer() ? player.getPreviousDistinctPosition()
                : map.dijkstraPathFind(enemy.getPosition(), player.getPosition(), enemy).getPosition();
        if (!merc.isAdjacentToPlayer() && Position.isAdjacent(player.getPosition(), nextPos))
            merc.setAdjacentToPlayer(true);

        return nextPos;
    }
}
