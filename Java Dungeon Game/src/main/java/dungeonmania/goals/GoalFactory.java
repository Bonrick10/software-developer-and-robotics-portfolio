package dungeonmania.goals;

import org.json.JSONArray;
import org.json.JSONObject;

public class GoalFactory {
    public static Goal createGoal(JSONObject jsonGoal, JSONObject config) {
        switch (jsonGoal.getString("goal")) {
        case "AND":
            return createGoalAnd(jsonGoal.getJSONArray("subgoals"), config);
        case "OR":
            return createGoalOr(jsonGoal.getJSONArray("subgoals"), config);
        case "exit":
            return createGoalExit();
        case "boulders":
            return createGoalBoulders();
        case "treasure":
            return createGoalTreasure(config);
        case "enemies":
            return createGoalEnemy(config);
        default:
            return null;
        }
    }

    public static Goal createGoalEnemy(JSONObject config) {
        int enemyGoal = config.optInt("enemy_goal", 1);
        return new GoalEnemy(enemyGoal);
    }

    public static Goal createGoalAnd(JSONArray subgoals, JSONObject config) {
        return new GoalAnd(createGoal(subgoals.getJSONObject(0), config),
                createGoal(subgoals.getJSONObject(1), config));
    }

    private static Goal createGoalOr(JSONArray subgoals, JSONObject config) {
        return new GoalOr(createGoal(subgoals.getJSONObject(0), config), createGoal(subgoals.getJSONObject(1), config));
    }

    private static Goal createGoalExit() {
        return new GoalExit();
    }

    private static Goal createGoalBoulders() {
        return new GoalBoulders();
    }

    private static Goal createGoalTreasure(JSONObject config) {
        int treasureGoal = config.optInt("treasure_goal", 1);
        return new GoalTreasure(treasureGoal);
    }
}
