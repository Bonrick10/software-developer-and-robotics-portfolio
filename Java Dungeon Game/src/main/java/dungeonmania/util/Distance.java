package dungeonmania.util;

// returns the next position, and the distance to travel to the destination
public class Distance {
    public static final int DEFAULT_DISTANCE = -5;
    private Position position;
    private int dist = DEFAULT_DISTANCE;

    public Distance(Position position, int dist) {
        this.position = position;
        this.dist = dist;
    }

    public Distance(Position position) {
        this.position = position;
    }

    public Position getPosition() {
        return position;
    }

    public int getDist() {
        return dist;
    }
}
