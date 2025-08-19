package dungeonmania.mvp;

import dungeonmania.DungeonManiaController;
import dungeonmania.response.models.DungeonResponse;
import dungeonmania.util.Direction;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;

public class LogicTest {
    @Test
    @Tag("15-1")
    @DisplayName("Test And condition on Light bulbs")
    public void andRuleLightBulb() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_logicTest_basicAnd", "c_logicTest_basic");

        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        res = dmc.tick(Direction.RIGHT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(2, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(1, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.DOWN);
        assertEquals(2, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(1, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(2, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(1, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(1, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(2, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.DOWN);
        assertEquals(1, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(2, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(1, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(2, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(1, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(2, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.UP);
        assertEquals(3, TestUtils.countType(res, "light_bulb_on"));
    }

    @Test
    @Tag("15-2")
    @DisplayName("Test Or condition on Light bulbs")
    public void orRuleLightBulb() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_logicTest_basicOr", "c_logicTest_basic");

        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        res = dmc.tick(Direction.RIGHT);
        assertEquals(2, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(1, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(2, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(1, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.DOWN);
        assertEquals(1, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(2, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(1, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(2, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(1, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(2, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.DOWN);
        assertEquals(3, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.UP);
        assertEquals(3, TestUtils.countType(res, "light_bulb_on"));
    }

    @Test
    @Tag("15-3")
    @DisplayName("Test Xor condition on Light bulbs")
    public void xorRuleLightBulb() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_logicTest_basicXor", "c_logicTest_basic");

        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        res = dmc.tick(Direction.RIGHT);
        assertEquals(2, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(1, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.DOWN);
        assertEquals(2, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(1, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.DOWN);
        assertEquals(2, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(1, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.UP);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));
    }

    @Test
    @Tag("15-4")
    @DisplayName("Test dead Co And condition on Light bulbs")
    public void deadCoAndRuleLightBulb() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_logicTest_deadCoAnd", "c_logicTest_basic");

        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        res = dmc.tick(Direction.RIGHT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.DOWN);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.DOWN);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.UP);
        res = dmc.tick(Direction.LEFT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.LEFT);
        res = dmc.tick(Direction.UP);
        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));
    }

    @Test
    @Tag("15-5")
    @DisplayName("Test working Co-And condition on Light bulbs")
    public void workingCoAndRuleLightBulb() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_logicTest_workingCoAnd", "c_logicTest_basic");

        assertEquals(3, TestUtils.countType(res, "light_bulb_off"));

        res = dmc.tick(Direction.RIGHT);
        assertEquals(2, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(1, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(1, TestUtils.countType(res, "light_bulb_off"));
        assertEquals(2, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(3, TestUtils.countType(res, "light_bulb_on"));
    }

    @Test
    @Tag("15-6")
    @DisplayName("Test single wire case on all conditions of Light bulbs")
    public void singleWireLightBulb() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_logicTest_singleWire", "c_logicTest_basic");

        assertEquals(4, TestUtils.countType(res, "light_bulb_off"));

        res = dmc.tick(Direction.DOWN);
        assertEquals(4, TestUtils.countType(res, "light_bulb_on"));
    }

    @Test
    @Tag("15-7")
    @DisplayName("Test multiple switches linked to one cardinal position of Light bulbs and they turn off")
    public void multiSwitchOnePosLightBulb() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_logicTest_singleWire", "c_logicTest_basic");

        assertEquals(4, TestUtils.countType(res, "light_bulb_off"));

        res = dmc.tick(Direction.DOWN);
        assertEquals(4, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.UP);
        dmc.tick(Direction.RIGHT);
        dmc.tick(Direction.RIGHT);
        res = dmc.tick(Direction.DOWN);
        assertEquals(4, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.LEFT);
        dmc.tick(Direction.DOWN);
        res = dmc.tick(Direction.LEFT);
        assertEquals(4, TestUtils.countType(res, "light_bulb_on"));

        dmc.tick(Direction.RIGHT);
        res = dmc.tick(Direction.RIGHT);
        assertEquals(4, TestUtils.countType(res, "light_bulb_off"));
    }

    @Test
    @Tag("15-8")
    @DisplayName("Test switch door cases")
    public void multiSwitchDoors() {
        DungeonManiaController dmc;
        dmc = new DungeonManiaController();
        DungeonResponse res = dmc.newGame("d_logicTest_singleWire", "c_logicTest_basic");

    }
}
