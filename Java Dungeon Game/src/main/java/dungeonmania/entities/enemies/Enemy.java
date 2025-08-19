package dungeonmania.entities.enemies;

import dungeonmania.Game;
import dungeonmania.battles.BattleStatistics;
import dungeonmania.battles.Battleable;
import dungeonmania.entities.Entity;
import dungeonmania.entities.Player;
import dungeonmania.entities.enemies.enemyMoveStrategy.EnemyMoveStrategy;
import dungeonmania.map.GameMap;
import dungeonmania.util.Position;
import dungeonmania.util.triggers.Destroyer;

public abstract class Enemy extends Entity implements Battleable, Destroyer {
    private BattleStatistics battleStatistics;
    private EnemyMoveStrategy moveStrat;

    public Enemy(Position position, double health, double attack) {
        super(position.asLayer(Entity.CHARACTER_LAYER));
        battleStatistics = new BattleStatistics(health, attack, 0, BattleStatistics.DEFAULT_DAMAGE_MAGNIFIER,
                BattleStatistics.DEFAULT_ENEMY_DAMAGE_REDUCER);
    }

    public void setMoveStrat(EnemyMoveStrategy moveStrat) {
        this.moveStrat = moveStrat;
    }

    public Position executeStrat(Game game) {
        return moveStrat.nextMove(game, this);
    }

    @Override
    public boolean canMoveOnto(GameMap map, Entity entity) {
        return entity instanceof Player || entity instanceof SnakeHead || entity instanceof SnakeBody;
    }

    @Override
    public BattleStatistics getBattleStatistics() {
        return battleStatistics;
    }

    public void updateBattleStatistics(double health, double attack) {
        setBattleStatistics(new BattleStatistics(health, attack, 0, BattleStatistics.DEFAULT_DAMAGE_MAGNIFIER,
                BattleStatistics.DEFAULT_ENEMY_DAMAGE_REDUCER));
    }

    public void setBattleStatistics(BattleStatistics newStat) {
        this.battleStatistics = newStat;
    }

    @Override
    public void onDestroy(GameMap map) {
        Game g = map.getGame();
        g.unsubscribe(getId());
        map.getPlayer().addEnemyKillCount();
    }

    public abstract void move(Game game);
}
