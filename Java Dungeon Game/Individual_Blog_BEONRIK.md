Week 7:
- Started assignment in group F09B_MAGIKARP with Ling Gi Fong (Clarice).
- Implemented relevant refactoring for Task 1(a) using a Strategy Pattern.
- Helped make some corrections with shared blog (Clarice did most of it however).
- Made merge request for Task a.

Week 8:
- Added my input for Task 1(b) which involved listing all files where an Observer Pattern was implemented, and my discussion of a specific area where it was implemented.
- To be safe, I left Clarice's input in the shared blog too.

- Pulled Clarice's progress on Task 1(c) and approved it's work.
- Began work on Task 1(d).
    - Plan to reorganise the interface of InventoryItem into an abstract class extending Entity.
    - Taken common canMoveOnto() method out of InventoryItem subclasses and collapsed into InventoryItem, similar for onOverlap() except for Bomb class.
    - Move functionality of InventoryItem onOverlap() to Player onOverlap() by placing it under instanceof condition and rearranging object method calls.
    - Convert Bomb onOverlap() as a helper method onPlayerOverlap() to call in Player onOverlap to perform side effects of Player overlapping Bomb.
    - Modify TriggerOverlapEvent() method in GameMap.java to collect Player.onOverlap(map, InventoryItem) Runnables as opposed to InventoryItem.onOverlap(map, Player) Runnables (which do nothing).
- Made some small improvements to Goal, after revisiting.
- Fixed EntityFactory such that all necessary object builds have their own build methods.
- Asked about triggerOnOverlap modifications; they agree that its not good.
    - Mentioned that Task (c) could be improved further.
    - Current thoughts; dead code should be safely removed, needs to implement a way to prevent an attempt to run dead code.
    - New interface classes for each of the on... methods in util.
    - These can function as marker classes so their instance can be checked first before to then call their implemented function.

Week 9:
- Refactored large method battle() in BattleFacade.