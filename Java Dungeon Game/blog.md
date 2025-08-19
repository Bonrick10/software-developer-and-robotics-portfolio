Assignment II Pair Blog Template
Task 1) Code Analysis and Refactoring ⛏️
a) From DRY to Design Patterns
https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/1 - Task 1a) Completed from BP/Individual_Blog
i. Look inside src/main/java/dungeonmania/entities/enemies. Where can you notice an instance of repeated code? Note down the particular offending lines/methods/fields.
Mercenary.java, ZombieToast.java: implementation under conditions set for players with invincibility potion
Mercenary.java, ZombieToast.java: implementation under conditions set for players with invisibility potion for Mercenary and default movement of ZombieToast.
Mercenary.java: under conditions set for players with invisibility potion, the repetition of the statement: map.moveTo(this, nextPos)
ii. What Design Pattern could be used to improve the quality of the code and avoid repetition? Justify your choice by relating the scenario to the key characteristics of your chosen Design Pattern.
https://refactoring.guru/design-patterns/strategy
Repeated code is being spotted in Mercenary and ZombieToast: in the move method implemented in both classes (abstract method specified in class enemy). Originally, our approach was to move the repeated code into the common abstract class of both classes (enemy), but since the method is not applicable to all subclasses, we later decided to implement the strategy pattern in our refactoring process.
By implementing the strategy pattern, we have broken down the movement of Mercenary and ZombieToast into several subparts (movement with invisibility potion, movement with invincibility potion, default movement etc.) and associated them with sub-objects. This extraction of overlapping logic in the two classes delegates the execution of specific functionalities to respective interfaces and reduces the variant behaviours within the same class.
iii. Using your chosen Design Pattern, refactor the code to remove the repetition.
We first created an interface for the movement of players with the invincibility potion, in which the method accepts a Game, Enemy object and returns the next position. The strategy between ZombieToast and Mercenary were identical in this case. Likewise, the default strategy of a ZombieToast and the case where the player is invisible for the Mercenary were the same and thus could be classified as a new Random strategy.
Additionally, there was also a fairly big code block where a different strategy is implemented when a Mercenary is allied with the Player. This was classed as its own strategy to remain consistent with this design pattern. Some attributes of Mercenary had to be accessed externally for this to work so getters and setters for isAdjacentToPlayer were added.
We also observed repeated lines of code in Mercenary.java that unnecessarily repeat the update of movement. The extra lines of code have been removed as a result.
b) Observer Pattern
Identify one place where the Observer Pattern is present in the codebase, and outline how the implementation relates to the key characteristics of the Observer Pattern.
List of files with Observer Pattern related methods (under the dungeonmania folder):
Game.java
map/GameMap.java
entities/Switch.java
entities/enemies/Enemy.java
entities/enemies/ZombieToastSpawner.java
entities/collectables/Bomb.java
Clarice:
GameMap exhibits characteristics of an observable in an observer pattern. The subject maintains a list of observables (has-a relationship) — a collection of entities in this case (one to many relationships), and triggers events to notify the entities of an update of movement.
Beonrik:
Switch and Bombs have characteristics of an Observer pattern where connected Bombs to switches become subscribed. The bombs are notified when a Switch becomes active. This is necessary in-game, as Bomb triggers should be instantaneous after a Switch activation. The game should ideally not have to wait for a game tick to complete to perform this action.
c) Inheritance Design
https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/2 - Task 1c) Completed from CF/Individual_Blog

i. Name the code smell present in the above code. Identify all subclasses of Entity which have similar code smells that point towards the same root cause.
Unused implementations are present in the above code, which can cause unnecessary cluttering. There exist empty methods that do not serve a functional purpose in the code base — the three methods are abstract methods declared in the superclass Entity, but are not applicable to the Exit object.
Subclasses with similar code smells
entities/buildables/Buildable.java
entities/collectables/potions/Potion.java (except onOverlap)
entities/collectables/Arrow.java, Bomb.java, Key.java, Sword,java, Treasure.java, Wood.java (except onOverlap)
entites/enemies/Enemy.java (only onMovedAway)
entites/enemies/ZombieToastSpawner.java (except onDestroy)
entities/Boulder.java, Door.java, Player.java, Portal.java (except onOverlap)
entities/Exit.java, Wall.java
entities/Switch.java (only onDestroy)
ii. Redesign the inheritance structure to solve the problem, in doing so remove the smells.
Considering that the methods are being called in GameMap.java (at a higher level), refactoring was done by implementing the empty methods in the superclass Entity.java, where the empty methods are set as the defaults.
d) More Code Smells
https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/3 - Task 1d) Completed from BP/Individual_Blog
https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/10 - Task 1c) and d) Updated from BP/Individual_Blog
i. What design smell is present in the above description?
The engineers attempted to solve a Law of Demeter (LoD) violation but encountered that the overall problem required heavy shotgun surgery. LoD was violated as collectable entities were seemingly performing actions relevant to the Player; Ideally the action performed by collectable entities would logically be performed by Player. Engineers found that there were too many changes to be made and some features made it difficult to rectify which is a symptom of shotgun surgery.
ii. Refactor the code to resolve the smell and underlying problem causing it.
Reorganised the interface of InventoryItem into an abstract class extending Entity.
Taken common canMoveOnto() method out of InventoryItem subclasses and collapsed into InventoryItem, similar for onOverlap() except for Bomb class.
Moved functionality of InventoryItem onOverlap() to Player onOverlap() by placing it under instanceof condition and rearranging object method calls.
Converted Bomb onOverlap() as a helper method onPlayerOverlap() to call in Player onOverlap to perform side effects of Player overlapping Bomb.
Modified TriggerOverlapEvent() method in GameMap.java to collect Player.onOverlap(map, InventoryItem) Runnables as opposed to InventoryItem.onOverlap(map, Player) Runnables (which do nothing).
e) Open-Closed Goals
https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/4 - Task 1e) Completed from CF/Individual_Blog
https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/5 - Task 1e) Adjusted from BP/Individual_Blog
i. Do you think the design is of good quality here? Do you think it complies with the open-closed principle? Do you think the design should be changed?
The open-closed principle (OCP) proposes that the functionalities of a class should be open for extension without the need to modify the source code. The use of switch statements within Goal.java and GoalFactory.java violates the OCP. This is because introducing a new goal type would require modification to the existing switch statements (adding a new case), which contradicts the principle of “closed for modification”.
Repeating string literals are spotted in both Goal.java and GoalFactory.java; the magic strings can be represented by defined constants to improve the readability of the code.
ii. If you think the design is sufficient as it is, justify your decision. If you think the answer is no, pick a suitable Design Pattern that would improve the quality of the code and refactor the code accordingly.
Goal provides constructors to create both a composite node(object containing 2 other Goal objects) or a child node (object containing String, integer), hence refactoring through the composite pattern provides clarity in expressing the existing part-whole hierarchy within the class. The refactoring involves the creation of subclasses representing different types of goals. Among these GoalExit.java, GoalBoulders.java, and GoalTreasures.java function as classes for child nodes, while GoalAnd.java and GoalOr.java are classes for composite nodes.
The implementation of the checking conditions in both methods (achieved, toString) has been kept in the superclass Goal.java. While the subclass-specific implementations have been extracted into the checkAchieve and returnString helper methods.
Relevant code fragments to create different goal types are extracted from createGoal as separate methods(GoalFactory.java).
f) Open Refactoring

1. https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/8 - Task 1f): Build methods to complete EntityFactory, Completed from BF/Individual_Blog
   In entityfactory (extraction of methods)
2. https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/11 - Task 1f): Refactoring buildable class to remove hard code, Completed from CF/Individual_Blog
   Overlapping methods between the subclasses (use, getDurability) are being moved to the superclass. The overlapping variable durability is also placed in Buildable,java,
   The Buildable constructor sets the Position of the entity as null at default and accepts a durability parameter instead of a position.
3. https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/9 - Task 1f): BattleStatistics.java and Entity.java, Completed from CF/Individual_Blog
   Removing repeating code in the constructor of BattleStatistics.java, and in translate() method in Entity.java (call setPosition() method within method to replace duplicated code).
4. https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/13 Task1f): BattleFacade extraction of method (5 steps contained in 1)
5. https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/6 - Task 1f): Moving player relevant actions to Player.java, Completed from CF/Individual_Blog
   The onOverlap functionality within Enemy.java and Boulder.java is being moved to Player.java
   Boulder.java: similar to part d, where LoD was violated as the entities were performing actions that are initiated by players: boulders pushed by player
   Enemy.java: has duplicated code within both classes that trigger a battle response onOverlap with one another
   Within triggerOverlapEvent in GameMap.java(after part d refactoring): Initially the player.onOverlap was not triggered when overLap with Enemy but enemy.onOverlap(), so despite of duplicated code only the battle response is triggered through Enemy.java. Refactoring is done by the removal of overridden method onOverLap from Enemy.java as logically, only the Player instantiates battles with an Enemy.
6. https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/7 - Task 1f): Moving abstract methods to superclass PlayerState.java, Completed frrom CF/Individual_Blog
   Subclasses of PlayerState.java (BaseState.java, InvincibleState.jave, InvisibleState.java) have identical implementations (duplicated code) for abstract methods of the class: transitionBase, transitionInvincible, transitionInvisible. Refactoring is done by placing the method implementation within the superclass.

Task 2) Evolution of Requirements 👽
a) Microevolution - Enemy Goal
https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/12 - Task 2a): Implementation of microevolution, Completed from microevolution
Assumptions
When allies are destroyed by a player-placed bomb, the kill is still counted towards the enemy goal (from forum)
If the only remaining goal is to kill one more enemy, and the player engages in a battle which leaves both entities dead, the killed enemy is still counted towards the goal (from forum)
Design
Creation of a GoalEnemy subclass (a child node)
Modify GoalFactory to create methods for creating an enemy goal
Creation of a variable within Player.java to keep track of enemy kill count and another for spawner destroy count. Getters of the variables within Player.java is also created.
Creation of getter methods in Game.java to fetch the 2 variables within the player object
Creation of a variable that stores the number of all spawners within Game.java, and creates corresponding getter and setter
Modify initRegisterSpawners() in GameMap.java to pass in the total number of spawners to Game.java
Modify onDestroy() in Enemy.java to add to enemyKillCount variable when an enemy is killed
Modify onDestroy() in zombieToastSpawner.java to add to spawnerDestroyCount variable
Critical problem: Looking through the coverage reports and the given codebase, onDestroy() method in zombieToastSpawner is never called.
Solution: Modify interact() in zombieToastSpawner.java to call onDestroy(). The precondition of a player owning a weapon is ensured in Game.java interact(String entityId).
Changes after review
SpawnerCount
Current approach: create variables to track destroyed spawners and total spawners
Original designed approach: Pass through the list of entities into a method and check for the existence of spawners. If none, then it is satisfied. However this approach is falsified, as spawners are not removed from the entities list after being destroyed.
Test list
Creation of 1 GoalEnemy: Test for all spawners destroyed and kill count is satisfied (sword)
Creation of 1 GoalEnemy: Test for all spawners destroyed and kill count is satisfied (bomb)
Creation of 1 GoalEnemy: Test for no spawner exists and kill count is satisfied (sword)
Creation of 1 GoalEnemy: Test for enemy goal = 0 and all spawners are destroyed (enemy goal = 0) (sword)
Creation of 1 Goal Enemy: Test for enemy goal = 0 and no spawner exists
Creation of 1 GoalAnd(GoalEnemy, GoalExit): Test completion of GoalEnemy but not GoalExit (sword)
Choice 1 Snakes
https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/15
Assumptions
Whether other snakes can move onto an invisible snake is undefined
When food are placed at an equal distance, the snake does not have preference in terms of which food gives a greater health boast.
The closest distance to food is calculated with consideration of the wall, and snake relevant entities.
The snake comes out of hibernation state when there’s reachable food
Battle is still triggered when a player overlaps with a hibernated snake
Players cannot fight invisible snakes
Destorying a snake head or body is both counted towards an enemy goal
Killing an invincible snake body part or snake head is not counted towards enemy goal
When a snake goes through a portal, its head teleport immediately, or just the head?
When movement occurs, new position <- position of head, position of head <- b1, b1 <- b2
Design
Creation of enemy subclass snakeHead (employing observer pattern for snakeHead and snakeBody)
Containing a list of snakeBody
Creation of enemy subclass snakeBody
Containing a snakeHead object
Removal of PlayerState, and creation of a class State, to improve code reusability (so it’s the state classes are usable by both Snakehead and Player class)
Creation of a new state: InvincibileInvisibleState account for stacked buffs
Creation of method in EntityFactory to create snakeHead
Modify BattleFacade to account for the snake’s state when a battle response is triggered
Modify GameMap, so that when a snakeHead is destroyed, its body parts will as well, and when a snakeBody is destroyed, any subsequent parts will be destroyed
Modify Portal.java, Door.java, Boulder.java to allow Snakes to move Onto
Repurpose the pathFind algorithm in GameMap
Changes after review
Original Approach: Creation of snakeState
Problem: snakeState has overlapping subclasses and methods with playerState
Test list
Test snake approaches the closest food
Test snake’s body increase in size when consuming treasure
Test player can battle snakeHead
Test player can battle snakeBody
Test snake hibernation when there is no food
Test snake hibernation when there is no reachable food
Test snake consuming treasure does not contribute to treasure goal
Test player defeat a body part of a snake, the subsequent body parts will be destroyed as well
Test player battle invincible SnakeHead
Test player battle invincible SnakeBody
Test player battle invincible SnakeBody and a new head is created
Other notes
It can travel through portals, non-snake enemies
It cannot travel through itself
When there is no reachable food, the snake immediately stops moving. And the snake will not continue to move even if there is reachable food when it’s hibernating.
Snake movement accounts for its own body part movements

Choice 2 Logic Switches
https://nw-syd-gitlab.cseunsw.tech/COMP2511/23T3/teams/F09B_MAGIKARP/assignment-ii/-/merge_requests/16
Assumptions
Switches are labelled as Conductors but act more as a power source
Wires can go through walls.
Co_And entities with no viable way to trigger all cardinal conductors simultaneously are effectively dead (unless they are a placed bomb).
Logical entities are only bound by a single logical rule.
If a Logical entity is only cardinally connected to one wire, then it doesn’t matter which logical rule is applied, it will only be active if that single wire is active.
Regarding Bombs:
Whether they have been placed or spawned, they can be subject to exploding.
When they are picked up and placed again, it must re-evaluate which wires are subscribed to align with the condition.
Placing a Co_And bomb in a position where all cardinally adjacent conductors are active satisfies the logical rule and explodes even if those wires are activated separately.
This can lead to cases where a Co_And Bomb is placed around four wires which are first switched one by one (not detonating the bomb) followed by picking up and immediately placing the bomb which will detonate the bomb.
Since wires and switched aren’t exempt from entities that can be destroyed, their destruction can change up the subscription of wires to switches, which means this needs to be reviewed every time an explosion occurs.
Bombs can be placed on top of wires, however it will only be conductively connected to its adjacent wires and not the wire it overlaps.
If a Co_And bomb is placed on the middle wire of 3 connected wires, it explodes whenever they’re activated since technically those wires are activated simultaneously.
X_Or bombs will NOT explode in this case since more than one wire is cardinally active.
Design
LogicRule interface for logical rules
Logical entities contain their logic conditions
LightBulb.
Bypass NameConverter to make the new LogicBomb class which extends Bomb and implements Logical use “bomb” type.
Changes after review
N/A
Test list
Basic Tests:
And, Or, XOr, Light Bulb
1 map each condition, containing a set of 3 light bulbs cardinally connected to 2, 3 then 4 switches activated by boulders. Turn on each switch one by one and check state of relevant lightbulb
Co_And LightBulb
1 map with 3 light bulbs, two can be activated with one switch which are connected to their respective light bulb in 2 and 4 cardinal positions. 3rd bulb is a dead case, check state is off in all instances of a switch being moved.
Single wire
One switch connected to four bulbs with each condition. SInce they all only have one wire, they all will turn on
Door blocking path
Open Switch door using And Or Xor Co_and test dead Co_and case
Logic Bombs:
All the assumptions are satisfied.
