package dungeonmania.mvp;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;

import dungeonmania.DungeonManiaController;
import dungeonmania.response.models.DungeonResponse;
import dungeonmania.util.Direction;
import dungeonmania.util.Position;

public class SnakesTest {
    @Test
    @Tag("17-1")
    @DisplayName("Test snake approach the closest food")
    public void shortestPath() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_shortestPath", "c_snakesTest_shortestPath");
        assertEquals(2, TestUtils.getEntities(res, "treasure").size());

        res = dmc.tick(Direction.UP);
        assertEquals(TestUtils.getEntities(res, "snake_head").get(0).getPosition(), new Position(5, 2));
        res = dmc.tick(Direction.UP);
        res = dmc.tick(Direction.UP);

        // assert that snake has consumed the closer treasure
        assertEquals(getSnakeHeadPos(res), new Position(4, 3));
        assertEquals(1, TestUtils.getEntities(res, "treasure").size());
    }

    @Test
    @Tag("17-2")
    @DisplayName("Test snake's body increase in size when consuming treasure")
    public void increaseBody() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_shortestPath", "c_snakesTest_shortestPath");
        assertEquals(2, TestUtils.getEntities(res, "treasure").size());
        // assert the snake does not has a body part
        assertEquals(0, TestUtils.getEntities(res, "snake_body").size());
        res = dmc.tick(Direction.UP);
        res = dmc.tick(Direction.UP);

        Position prevTail = new Position(4, 2);
        assertEquals(getSnakeHeadPos(res), prevTail);
        res = dmc.tick(Direction.UP);

        // assert a treasure is consumed
        assertEquals(1, TestUtils.getEntities(res, "treasure").size());
        assertEquals(getSnakeHeadPos(res), new Position(4, 3));
        // assert the snake grew a body part
        assertEquals(1, TestUtils.getEntities(res, "snake_body").size());
        // assert the position of the body part is at the previous position of the tail
        assertEquals(getSnakeBodyPos(res), prevTail);
    }

    @Test
    @Tag("17-3")
    @DisplayName("Test player can battle a snake body part")
    public void battleBody() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_battleBody", "c_snakesTest_shortestPath");

        assertEquals(1, TestUtils.countType(res, "snake_head"));

        // move player to right, collect sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());
        // assert snake consumes a treasure
        assertEquals(1, TestUtils.countType(res, "snake_body"));

        // encounter enemy
        res = dmc.tick(Direction.RIGHT);

        // assert a battle occured and snakeBody is defeated, but snakeHead remains
        assertEquals(res.getBattles().size(), 1);
        assertEquals(1, TestUtils.countType(res, "snake_head"));
        assertEquals(0, TestUtils.countType(res, "snake_body"));
    }

    @Test
    @Tag("17-4")
    @DisplayName("Test player can battle a snake head")
    public void battleHead() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_battleHead", "c_snakesTest_shortestPath");
        assertEquals(1, TestUtils.countType(res, "snake_head"));

        // move player to right, collect sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());

        // encounter enemy
        res = dmc.tick(Direction.RIGHT);

        // assert a battle occured and snakeHead is defeated
        assertEquals(res.getBattles().size(), 1);
        assertEquals(0, TestUtils.countType(res, "snake_head"));
    }

    @Test
    @Tag("17-5")
    @DisplayName("Test snake hibernates when there's no food")
    public void hibernation() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_hibernation", "c_snakesTest_shortestPath");
        Position pos = getSnakeHeadPos(res);

        for (int i = 0; i < 4; i++) {
            // assert that the snake is not moving
            res = dmc.tick(Direction.UP);
            assertEquals(pos, getSnakeHeadPos(res));
        }
    }

    @Test
    @Tag("17-6")
    @DisplayName("Test snake hibernates when there's no reachable food")
    public void hibernation2() {
        // the snake is trapped within walls, with a piece of food placed outside of the walls
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_hibernation2", "c_snakesTest_shortestPath");
        Position pos = getSnakeHeadPos(res);

        for (int i = 0; i < 4; i++) {
            // assert that the snake is not moving
            res = dmc.tick(Direction.UP);
            assertEquals(pos, getSnakeHeadPos(res));
        }
    }

    @Test
    @Tag("17-7")
    @DisplayName("Test snake consuming treasure does not contribute to treasure goal")
    public void treasureGoal() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_treasureGoal", "c_snakesTest_shortestPath");
        assertEquals(2, TestUtils.getEntities(res, "treasure").size());
        // assert the snake does not has a body part
        assertEquals(0, TestUtils.getEntities(res, "snake_body").size());
        res = dmc.tick(Direction.UP);
        res = dmc.tick(Direction.UP);
        res = dmc.tick(Direction.UP);

        // assert a treasure is consumed
        assertEquals(1, TestUtils.getEntities(res, "treasure").size());
        // assert player goal is not reached
        assertTrue(TestUtils.getGoals(res).contains("treasure"));
    }

    @Test
    @Tag("17-8")
    @DisplayName("Test player desfeat a body part of a snake, the subsequent body parts will be destroyed as well")
    public void destroyRestOfBodyPart() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_destroyRestOfBodyPart", "c_snakesTest_shortestPath");
        assertEquals(1, TestUtils.countType(res, "snake_head"));

        Position firstTail = getSnakeHeadPos(res);

        // move player to right, collect sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());
        // assert snake consumes a treasure
        assertEquals(1, TestUtils.countType(res, "snake_body"));
        assertTrue(TestUtils.entityAtPosition(res, "snake_body", firstTail));

        Position secondTail = getSnakeHeadPos(res);
        res = dmc.tick(Direction.RIGHT);
        // assert snake consumes the second treasure
        assertEquals(2, TestUtils.countType(res, "snake_body"));
        assertTrue(TestUtils.entityAtPosition(res, "snake_body", secondTail));

        // encount snakeBody
        res = dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.UP);
        // assert a battle occured and snakeBody is defeated, but snakeHead remains
        assertEquals(1, res.getBattles().size());
        assertEquals(1, TestUtils.countType(res, "snake_head"));
        assertEquals(0, TestUtils.countType(res, "snake_body"));
    }

    @Test
    @Tag("17-9")
    @DisplayName("Test player can battle an invincible snake head")
    public void battleInvincibleHead() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_battleInvincibleHead", "c_snakesTest_shortestPath");

        assertEquals(1, TestUtils.countType(res, "snake_head"));

        // move player to right, collect sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());
        // assert snake consumes an invinicibility potion
        assertEquals(1, TestUtils.countType(res, "snake_body"));

        // encounter enemy
        res = dmc.tick(Direction.UP);
        res = dmc.tick(Direction.RIGHT);
        res = dmc.tick(Direction.RIGHT);
        res = dmc.tick(Direction.DOWN);

        // assert a battle occured and snakeHead and its body remains
        assertEquals(res.getBattles().size(), 1);
        assertEquals(1, TestUtils.countType(res, "snake_head"));
        assertEquals(1, TestUtils.countType(res, "snake_body"));
    }

    @Test
    @Tag("17-10")
    @DisplayName("Test player can battle an invincible snake body")
    public void battleInvincibleBody() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_battleInvincibleHead", "c_snakesTest_shortestPath");

        assertEquals(1, TestUtils.countType(res, "snake_head"));

        // move player to right, collect sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());
        // assert snake consumes an invinicibility potion
        assertEquals(1, TestUtils.countType(res, "snake_body"));

        // encounter enemy
        res = dmc.tick(Direction.RIGHT);

        // assert a battle occured
        assertEquals(res.getBattles().size(), 1);
        assertEquals(1, TestUtils.countType(res, "snake_head"));
    }

    @Test
    @Tag("17-11")
    @DisplayName("Test player can battle an invincible snake body and a new head is created")
    public void battleInvincibleBodyNewHead() {
        DungeonManiaController dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_snakesTest_battleInvincibleBodyNewHead", "c_snakesTest_shortestPath");
        assertEquals(1, TestUtils.countType(res, "snake_head"));

        Position firstTail = getSnakeHeadPos(res);

        // move player to right, collect sword
        res = dmc.tick(Direction.RIGHT);
        // assert player picks up sword
        assertEquals(1, TestUtils.getInventory(res, "sword").size());
        // assert snake consumes an invincibility potion
        assertEquals(1, TestUtils.countType(res, "snake_body"));
        assertTrue(TestUtils.entityAtPosition(res, "snake_body", firstTail));

        Position secondTail = getSnakeHeadPos(res);
        res = dmc.tick(Direction.RIGHT);
        // assert snake consumes the second treasure
        assertEquals(2, TestUtils.countType(res, "snake_body"));
        assertTrue(TestUtils.entityAtPosition(res, "snake_body", secondTail));

        // encount snakeBody
        res = dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.UP);
        // assert a battle occured and snakeBody is defeated, but snakeHead remains
        assertEquals(1, res.getBattles().size());
        assertEquals(2, TestUtils.countType(res, "snake_head"));
        assertEquals(0, TestUtils.countType(res, "snake_body"));
    }

    private Position getSnakeHeadPos(DungeonResponse res) {
        return TestUtils.getEntities(res, "snake_head").get(0).getPosition();
    }

    private Position getSnakeBodyPos(DungeonResponse res) {
        return TestUtils.getEntities(res, "snake_body").get(0).getPosition();
    }

    private Position getPlayerPos(DungeonResponse res) {
        return TestUtils.getEntities(res, "player").get(0).getPosition();
    }
}
