package dungeonmania.entities.logicals.condtions;

public class LogicFactory {
    public static LogicRule createLogic(String type) {
        switch (type) {
        case "and":
            return new AndRule();
        case "or":
            return new OrRule();
        case "xor":
            return new XorRule();
        case "co_and":
            return new CoAndRule();
        default:
            return null;
        }
    }
}
