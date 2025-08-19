package dungeonmania.battles;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import dungeonmania.Game;
import dungeonmania.entities.BattleItem;
import dungeonmania.entities.Entity;
import dungeonmania.entities.Player;
import dungeonmania.entities.collectables.potions.Potion;
import dungeonmania.entities.enemies.Enemy;
import dungeonmania.entities.enemies.Mercenary;
import dungeonmania.entities.enemies.SnakeHead;
import dungeonmania.entities.enemies.SnakeBody;
import dungeonmania.entities.inventory.InventoryItem;
import dungeonmania.response.models.BattleResponse;
import dungeonmania.response.models.ResponseBuilder;
import dungeonmania.util.NameConverter;

public class BattleFacade {
    private List<BattleResponse> battleResponses = new ArrayList<>();

    public void battle(Game game, Player player, Enemy enemy) {
        // 0. init
        double initialPlayerHealth = player.getBattleStatistics().getHealth();
        double initialEnemyHealth = enemy.getBattleStatistics().getHealth();
        String enemyString = NameConverter.toSnakeCase(enemy);

        // 1. apply buff provided by the game and player's inventory
        // getting buffing amount
        List<BattleItem> battleItems = new ArrayList<>();
        BattleStatistics playerBuff = new BattleStatistics(0, 0, 0, 1, 1);
        playerBuff = applyAllBuffs(game, player, battleItems, playerBuff);

        // 2. Battle the two stats
        BattleStatistics playerBaseStatistics = player.getBattleStatistics();
        BattleStatistics enemyBattleStatistics = enemy.getBattleStatistics();
        // applying buff to enemy if it is a snake
        enemyBattleStatistics = applyAllBuffs(game, enemy, enemyBattleStatistics);

        BattleStatistics playerBattleStatistics = BattleStatistics.applyBuff(playerBaseStatistics, playerBuff);
        if (!playerBattleStatistics.isEnabled() || !enemyBattleStatistics.isEnabled())
            return;
        List<BattleRound> rounds = BattleStatistics.battle(playerBattleStatistics, enemyBattleStatistics);

        // 3. update health to the actual statistics
        updateHealth(player, enemy, playerBattleStatistics, enemyBattleStatistics);

        // 4. call to decrease durability of items
        updateDurability(game, battleItems);

        // 5. Log the battle - solidate it to be a battle response
        updateBattleResponse(enemyString, rounds, battleItems, initialPlayerHealth, initialEnemyHealth);
    }

    public List<BattleResponse> getBattleResponses() {
        return battleResponses;
    }

    private BattleStatistics applyAllBuffs(Game game, Player player, List<BattleItem> battleItems,
            BattleStatistics playerBuff) {
        Potion effectivePotion = player.getEffectivePotion();
        if (effectivePotion != null) {
            playerBuff = player.applyBuff(playerBuff);
        } else {
            for (BattleItem item : player.getInventory().getEntities(BattleItem.class)) {
                if (item instanceof Potion)
                    continue;
                playerBuff = item.applyBuff(playerBuff);
                battleItems.add(item);
            }
        }

        List<Mercenary> mercs = game.getMap().getEntities(Mercenary.class);
        for (Mercenary merc : mercs) {
            if (!merc.isAllied())
                continue;
            playerBuff = BattleStatistics.applyBuff(playerBuff, merc.getBattleStatistics());
        }
        return playerBuff;
    }

    // check if the snake's state
    private BattleStatistics applyAllBuffs(Game game, Enemy enemy, BattleStatistics buff) {
        if (enemy instanceof SnakeHead) {
            buff = ((SnakeHead) enemy).applyBuff(buff);
        } else if (enemy instanceof SnakeBody) {
            buff = ((SnakeBody) enemy).getHead().applyBuff(buff);
        }
        return buff;
    }

    private void updateHealth(Player player, Enemy enemy, BattleStatistics playerBattleStatistics,
            BattleStatistics enemyBattleStatistics) {
        player.getBattleStatistics().setHealth(playerBattleStatistics.getHealth());
        enemy.getBattleStatistics().setHealth(enemyBattleStatistics.getHealth());
    }

    private void updateDurability(Game game, List<BattleItem> battleItems) {
        for (BattleItem item : battleItems) {
            if (item instanceof InventoryItem)
                item.use(game);
        }
    }

    private void updateBattleResponse(String enemyString, List<BattleRound> rounds, List<BattleItem> battleItems,
            double initialPlayerHealth, double initialEnemyHealth) {
        battleResponses.add(new BattleResponse(enemyString,
                rounds.stream().map(ResponseBuilder::getRoundResponse).collect(Collectors.toList()),
                battleItems.stream().map(Entity.class::cast).map(ResponseBuilder::getItemResponse)
                        .collect(Collectors.toList()),
                initialPlayerHealth, initialEnemyHealth));
    }
}
