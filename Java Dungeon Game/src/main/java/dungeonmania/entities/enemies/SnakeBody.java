package dungeonmania.entities.enemies;

import dungeonmania.entities.playerState.State;
import dungeonmania.Game;
import dungeonmania.util.Position;

public class SnakeBody extends Enemy {
    private SnakeHead snakeHead;

    public SnakeBody(Position position, SnakeHead snakeHead) {
        super(position, snakeHead.getHealth(), snakeHead.getAttack());
        this.snakeHead = snakeHead;
    }

    public void updateBattleStatistics() {
        updateBattleStatistics(snakeHead.getHealth(), snakeHead.getAttack());
    }

    // the movement in snakeBody will be implemented in the SnakeHead class
    public void move(Game game) {
        return;
    }

    public SnakeHead getHead() {
        return snakeHead;
    }

    public State getState() {
        return snakeHead.getState();
    }
}
