package dungeonmania.mvp;

import dungeonmania.DungeonManiaController;
import dungeonmania.response.models.DungeonResponse;
import dungeonmania.util.Direction;
import dungeonmania.exceptions.InvalidActionException;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;
public class GoalEnemyTest {
    @Test
    @Tag("16-1")
    @DisplayName("Test achieving a basic enemy goal, enemy kill count fulfilled and all spawners destroyed (sword)")
    public void enemy() throws InvalidActionException {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_basicGoalsTest_enemies", "c_basicGoalsTest_enemies");
        String spawnerId = getSpawnerId(res);

        // move player to right, collect sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());

        // cardinally adjacent: true, has sword: true
        // assert the player can interact with the spawner
        res = assertDoesNotThrow(() -> dmc.interact(spawnerId));
        assertEquals(1, TestUtils.countType(res, "zombie_toast_spawner"));
        // assert goal not met
        assertTrue(TestUtils.getGoals(res).contains(":enemies"));

        // encounter enemy
        res = dmc.tick(Direction.UP);
        res = dmc.tick(Direction.RIGHT);
        // assert enemy is killed
        assertTrue(TestUtils.countEntityOfType(TestUtils.getEntities(res), "mercenary") == 0);

        // assert goal met
        assertEquals("", TestUtils.getGoals(res));
    }

    @Test
    @Tag("16-2")
    @DisplayName("Test achieving a basic enemy goal, enemy kill count fulfilled and no spawner exists (sword)")
    public void enemyNoSpawner() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_basicGoalsTest_enemiesNoSpawner", "c_basicGoalsTest_enemiesNoSpawner");
        // assert no spawner exists
        assertEquals(0, TestUtils.countType(res, "zombie_toast_spawner"));
        // assert 2 enemies exist
        assertEquals(2, TestUtils.countType(res, "mercenary"));

        // move player to right, collect sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());
        // assert goal not met
        assertTrue(TestUtils.getGoals(res).contains(":enemies"));

        // encounter enemy 1
        res = dmc.tick(Direction.RIGHT);
        assertEquals(1, TestUtils.countType(res, "mercenary"));
        // assert goal not met
        assertTrue(TestUtils.getGoals(res).contains(":enemies"));

         // encounter enemy 2
        res = dmc.tick(Direction.RIGHT);
        assertEquals(0, TestUtils.countType(res, "mercenary"));
        // assert goal met
        assertEquals("", TestUtils.getGoals(res));
    }

    @Test
    @Tag("16-3")
    @DisplayName("Test achieving a basic enemy goal, all spawners destroyed and enemies goal = 0 (sword)")
    public void enemyNoEnemies() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_basicGoalsTest_enemiesNoEnemies", "c_basicGoalsTest_enemiesNoEnemies");
        String spawnerId = getSpawnerId(res);

        // move to the right, pick up sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());
        // assert the player can interact with the spawner
        res = assertDoesNotThrow(() -> dmc.interact(spawnerId));
        assertEquals(1, TestUtils.countType(res, "zombie_toast_spawner"));

        // assert goal met
        assertEquals("", TestUtils.getGoals(res));
    }

    @Test
    @Tag("16-4")
    @DisplayName("Test achieving a basic enemy goal, no spawners and enemies goal = 0")
    public void enemyNoEnemiesNoSpawner() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_basicGoalsTest_enemiesNoEnemiesNoSpawner",
            "c_basicGoalsTest_enemiesNoEnemies");
        assertEquals(0, TestUtils.countType(res, "zombie_toast_spawner"));

        // assert goal met
        assertEquals("", TestUtils.getGoals(res));
    }

    @Test
    @Tag("16-5")
    @DisplayName("Testing that the exit goal must be achieved last in EXIT and ENEMY (sword)")
    public void exitAndEnemyOrder() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_complexGoalsTest_exitAndEnemyOrder",
                "c_basicGoalsTest_enemies");

        assertTrue(TestUtils.getGoals(res).contains(":exit"));
        assertTrue(TestUtils.getGoals(res).contains(":enemies"));

        // player collects sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());

        // move player towards exit
        res = dmc.tick(Direction.RIGHT);
        // don't check state of exit goal in string
        assertTrue(TestUtils.getGoals(res).contains(":enemies"));

        // move player to encounter enemy
        res = dmc.tick(Direction.RIGHT);
        // assert enemy is killed
        assertEquals(0, TestUtils.countType(res, "mercenary"));

        // assert enemies goal met, but goal string is not empty
        assertFalse(TestUtils.getGoals(res).contains(":enemies"));
        assertNotEquals("", TestUtils.getGoals(res));

        // move player back onto exit
        res = dmc.tick(Direction.LEFT);

        // assert goal met
        assertEquals("", TestUtils.getGoals(res));
    }

    @Tag("16-6")
    @DisplayName("Test achieving a basic enemy goal, enemy kill count fulfilled and all spawners destroyed (bomb)")
    public void enemy2() throws InvalidActionException {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_basicGoalsTest_enemies2", "c_basicGoalsTest_enemies");

        // activate switch
        res = dmc.tick(Direction.RIGHT);

        // pick up Bomb
        res = dmc.tick(Direction.DOWN);
        assertEquals(1, TestUtils.getInventory(res, "bomb").size());

         // place bomb cardinally adjacent
        res = dmc.tick(Direction.RIGHT);
        res = dmc.tick(TestUtils.getInventory(res, "bomb").get(0).getId());
        assertEquals(1, TestUtils.countType(res, "zombie_toast_spawner"));

        // assert enemy is killed
        assertTrue(TestUtils.countEntityOfType(TestUtils.getEntities(res), "mercenary") == 0);

        // assert goal met
        assertEquals("", TestUtils.getGoals(res));
    }

    private String getSpawnerId(DungeonResponse res) {
        return TestUtils.getEntitiesStream(res, "zombie_toast_spawner").findFirst().get().getId();
    }
}
