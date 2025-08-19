package dungeonmania.entities.playerState;
import dungeonmania.entities.Player;
import dungeonmania.entities.enemies.SnakeHead;

public class State {
    private boolean isInvincible = false;
    private boolean isInvisible = false;

    State(boolean isInvincible, boolean isInvisible) {
        this.isInvincible = isInvincible;
        this.isInvisible = isInvisible;
    }

    public boolean isInvincible() {
        return isInvincible;
    };

    public boolean isInvisible() {
        return isInvisible;
    };

    public void transitionBase(Player player) {
        player.changeState(new BaseState());
    }

    public void transitionInvincible(Player player) {
        player.changeState(new InvincibleState());
    }

    public void transitionInvisible(Player player) {
        player.changeState(new InvisibleState());
    }

    public void transitionBase(SnakeHead snake) {
        snake.changeState(new BaseState());
    }

    public void transitionInvincible(SnakeHead snake) {
        snake.changeState(new InvincibleState());
    }

    public void transitionInvisible(SnakeHead snake) {
        snake.changeState(new InvisibleState());
    }

    public void transitionInvincibleInvisible(SnakeHead snake) {
        snake.changeState(new InvincibleInvisibleState());
    }
}
