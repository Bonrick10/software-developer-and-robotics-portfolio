package dungeonmania.entities.enemies.enemyMoveStrategy;

import java.util.List;
import java.util.Random;
import java.util.stream.Collectors;

import dungeonmania.Game;
import dungeonmania.entities.enemies.Enemy;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;

public class RandomStrat implements EnemyMoveStrategy {
    @Override
    public Position nextMove(Game game, Enemy enemy) {
        GameMap map = game.getMap();
        Random randGen = new Random();
        List<Position> pos = enemy.getPosition().getCardinallyAdjacentPositions();
        pos = pos.stream().filter(p -> map.canMoveTo(enemy, p)).collect(Collectors.toList());
        if (pos.size() == 0) {
            return enemy.getPosition();
        } else {
            return pos.get(randGen.nextInt(pos.size()));
        }
    }
}
