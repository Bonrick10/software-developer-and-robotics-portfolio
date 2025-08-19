package dungeonmania.entities.enemies;

import java.util.ArrayList;
import java.util.List;
import dungeonmania.Game;
import dungeonmania.battles.BattleStatistics;
import dungeonmania.entities.Entity;
import dungeonmania.entities.collectables.Treasure;
import dungeonmania.entities.collectables.potions.InvincibilityPotion;
import dungeonmania.entities.collectables.potions.InvisibilityPotion;
import dungeonmania.entities.collectables.potions.Potion;
import dungeonmania.entities.collectables.Key;
import dungeonmania.entities.collectables.Arrow;
import dungeonmania.entities.inventory.InventoryItem;
import dungeonmania.entities.playerState.BaseState;
import dungeonmania.entities.playerState.InvincibleInvisibleState;
import dungeonmania.entities.playerState.State;
import dungeonmania.entities.playerState.InvisibleState;
import dungeonmania.entities.playerState.InvincibleState;
import dungeonmania.map.GameMap;
import dungeonmania.util.Distance;
import dungeonmania.util.Position;
import dungeonmania.util.triggers.Overlapper;

// employ observer pattern to all snake body parts
public class SnakeHead extends Enemy implements Overlapper {
    public static final double DEFAULT_ATTACK = 5;
    public static final double DEFAULT_HEALTH = 10;
    public static final double DEFAULT_BUFF = 1;

    private double health;
    private double attack;
    private double attackArrowBuff;
    private double healthTreasureBuff;
    private double healthKeyBuff;

    private State state;
    private List<SnakeBody> body = null;
    private Position tail;

    public SnakeHead(Position position, double health, double attack, double attackArrowBuff,
        double healthTreasureBuff, double healthKeyBuff, State state, List<SnakeBody> body) {
        super(position, health, attack);
        this.health = health;
        this.attack = attack;
        this.attackArrowBuff = attackArrowBuff;
        this.healthTreasureBuff = healthTreasureBuff;
        this.healthKeyBuff = healthKeyBuff;
        this.state = state;
        this.body = body;
    }

    public void updateHealth(double update) {
        health += update;
        updateBattleStatistics(health, attack);
    }

    public void updateArrowAttack() {
        attack += attackArrowBuff;
        updateBattleStatistics(health, attack);
    }

    public void changeState(State snakeState) {
        state = snakeState;
    }

    @Override
    // all entites can move onto it besides other snakes and other parts of itself
    public boolean canMoveOnto(GameMap map, Entity entity) {
        // restriction to cross over another snake
        return !(entity instanceof SnakeHead || entity instanceof SnakeBody);
    }

    @Override
    public void onOverlap(GameMap map, Entity entity) {
        // battle response will be triggered from the player side
        if (entity instanceof InventoryItem) {
            // check if the item can be used or consumed
            if (use(entity) || consume(entity)) {
                map.destroyEntity(entity);
                growBody(map);
            }
        }
    }

    public boolean consume(Entity item) {
        if (item instanceof Treasure) {
            updateHealth(healthTreasureBuff);
        } else if (item instanceof Key) {
            updateHealth(healthKeyBuff);
        } else if (item instanceof Arrow) {
            updateArrowAttack();
        } else {
            return false;
        }

        // notify each bodyPart
        body.stream().forEach(b -> b.updateBattleStatistics());
        return true;
    }

    public boolean use(Entity potion) {
        if (!(potion instanceof Potion)) {
            return false;
        }

        if (potion instanceof InvincibilityPotion) {
            if (state instanceof BaseState) {
                state.transitionInvincible(this);
            } else if (state instanceof InvisibleState) {
                state.transitionInvincibleInvisible(this);
            }
        } else {
            if (state instanceof BaseState) {
                state.transitionInvisible(this);
            } else if (state instanceof InvincibleState) {
                state.transitionInvincibleInvisible(this);
            }
        }
        return true;
    }

    @Override
    public void move(Game game) {
        GameMap map = game.getMap();

        Position last = this.getPosition();
        if (body.size() > 0) {
            last = body.get(body.size() - 1).getPosition();
        }
        // store the tail position of the snake before moving
        this.tail = last;

        // record the position of the head before moving
        Position prev = this.getPosition();
        // redesign the path finder, so that the distance is known and can be used
        List<List<? extends Entity>> entityLists = new ArrayList<>();
        entityLists.add(map.getEntities(Treasure.class));
        entityLists.add(map.getEntities(Key.class));
        entityLists.add(map.getEntities(Arrow.class));
        entityLists.add(map.getEntities(InvisibilityPotion.class));
        entityLists.add(map.getEntities(InvincibilityPotion.class));

        Distance closest = null;
        for (List<? extends Entity> entities : entityLists) {
            for (Entity e : entities) {
                Distance curr = map.dijkstraPathFind(getPosition(), e.getPosition(), this);
                if (closest == null) {
                    if (curr.getDist() > 0) {
                        closest = curr;
                    }
                } else if (curr.getDist() < closest.getDist() && curr.getDist() > 0) {
                    closest = curr;
                }
            }
        }

        // if there is no reachable food, the snake goes into hibernation and stays still
        if (closest == null) {
            map.moveTo(this, this.getPosition());
        } else {
            map.moveTo(this, closest.getPosition());
            // trigger a movement for all snake body
            for (SnakeBody s: body) {
                Position prevBody = s.getPosition();
                map.moveTo(s, prev);
                prev = prevBody;
            }
        }
    }

    // add snakeBody subscription
    public void growBody(GameMap map) {
        // check previous position of tail and grow accordingly
        SnakeBody newPart = new SnakeBody(tail, this);
        body.add(newPart);
        map.addEntity(newPart);
    }

    public void growNewHead(GameMap map, SnakeBody bodyPart) {
        int index = body.indexOf(bodyPart);
        SnakeBody killedPart = body.get(index);
        if (index != body.size() - 1) {
            SnakeBody newHead = body.get(index + 1);
            List<SnakeBody> origin = body;

            // seperate the current bodyParts into 2, exlcuding the bodyPart transform to a head,
            // and the defeated body part
            body = origin.subList(0, index);
            List<SnakeBody> newBody = origin.subList(index + 1, origin.size());

            // create new head with the rest of the body parts, and add to the map
            map.addEntity(new SnakeHead(newHead.getPosition(), health, attack, attackArrowBuff, healthTreasureBuff,
            healthKeyBuff, state, newBody));
            // and destroy the body part from the map (now transformed to a head)
            map.destroyEntity(newHead);
        }

        map.destroyEntity(killedPart);
    }

    public double getHealth() {
        return health;
    }

    public double getAttack() {
        return attack;
    }

    public State getState() {
        return state;
    }

    public List<SnakeBody> getBody() {
        return body;
    }

    // returns a list of snake body parts following the given part (including the given part)
    // while truncated the body list stored in this snakeHead
    public List<SnakeBody> getBody(SnakeBody bodyPart) {
        int index = body.indexOf(bodyPart);
        if (index == -1) {
            index = 0;
        }
        List<SnakeBody> origin = body;
        body = new ArrayList<SnakeBody>(origin.subList(0, index));
        return origin.subList(index, origin.size());
    }

    public BattleStatistics applyBuff(BattleStatistics origin) {
        if (state instanceof InvincibleState) {
            return BattleStatistics.applyBuff(origin, new BattleStatistics(0, 0, 0, 1, 1, true, true));
        } else if (state instanceof InvisibleState) {
            return BattleStatistics.applyBuff(origin, new BattleStatistics(0, 0, 0, 1, 1, false, false));
        } else if (state instanceof InvincibleInvisibleState) {
            return BattleStatistics.applyBuff(origin, new BattleStatistics(0, 0, 0, 1, 1, true, false));
        }
        return origin;
    }

}
