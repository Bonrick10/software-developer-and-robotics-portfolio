package dungeonmania.goals;

import dungeonmania.Game;

public abstract class Goal {
    public static final boolean DEFAULT_BOOLEAN = false;
    public static final String EMPTY = "";

    public boolean achieved(Game game) {
        if (checkPlayer(game)) {
            return DEFAULT_BOOLEAN;
        }
        return checkAchieve(game);
    }

    public String toString(Game game) {
        if (checkAchieve(game)) {
            return EMPTY;
        }
        return returnString(game);
    }

    // default behaviour
    protected abstract String returnString(Game game);

    // default behaviour: check if the goal is achieved
    protected abstract boolean checkAchieve(Game game);

    // check if an active player exists
    private boolean checkPlayer(Game game) {
        return (game.getPlayer() == null);
    }
}
