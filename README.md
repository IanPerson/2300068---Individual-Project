# CMP105 2024 Final Submission 

## Game 

**_Abyssal Village_** 

_Abyssal Village is a game where you, a decrepit peasant living in a ruined village overrun with monsters, must harvest crops and make enough money to pay for your family's entry into a safer town. This requires you to survive against the monsters of the woods while also tending to your family's hunger. Due to your desperation, searching the abandoned homes of your neighbours for loot is another possible method for earning the coin._

_NOTE: Some unfortunate circumstances forced me into spending two weeks looking for laptop repairs as well as redoing the darts project from scratch. Because of this, I lost a lot of time to work on this project and could not implement/polish things to the degree I would've liked. However, I am satisfied with what I was able to accomplish despite the setbacks._

**Controls:** 

WASD to move,
mouse to navigate menus,
f to interact/use shop,
escape to pause,
q and e to move items in inventory,
i to open full inventory
shift to sprint
r to attack monsters (only works if you're holding a lantern)

## Student Details

**Student Name:** Ian Person

**Student Number:** 2300068

**Course:** _CGAD_
## Changelist
_The changes you have made since the group submission._

* Enemies - Two types of enemies have been added which spawn at night. The chaser will run towards you relentlessly unless driven away or killed by a lantern, while the grabber is a stationary enemy that will try to trap you if you get too close.
* Day/night cycle - The environment will transition between a daytime state and a night-time state. During the day, you can freely trade with the shopkeeper and safely farm. During the night, however, enemies will spawn that will threaten you while you harvest your crops.
* Lanterns/combat - The player has been given lanterns to ward off any enemies which may pose a threat. Chasers will run away when coming into contact with the light, and the player can permanently remove a lantern in order to kill a monster with it.
* New map - The previous map has been expanded to include several houses, all of which include abandoned boxes that may provide some loot for the player to make use of.
* Prompt text - Some text has been added to notify the player when they can interact with something. This was a small quality of life change to make the control scheme feel less awkward.

## Known Bugs
_List any issues you are aware of and how to avoid them_

* The grabber's arm will not appear when attempting to grab the player. This would not go away despite everything I did in an attempt to fix it. Focus on keeping any grabber's as far away as possible, as their difficult to escape even when their arm is visible. (FIXED)
* The program would crash when picking up a lantern due to attempting to access data of a deleted object (FIXED)=
* All chickens are incorporeal. They may wander somewhere you cannot reach them, but they come back eventually. It's impossible for them to leave the map borders.
---
---

# Proposal from March

## Brief Overview of game 

I plan to extend the project by leaning heavily into the horror genre, with enemies to avoid while the player tends to their crops. The village the player currently lives in has been overrun by monsters, and they must earn enough money for their family to join another town. The game will cycle between day and night, and the player will have access to different actions during each stage. 

During the day, the player can safely grow crops, collect resources, and trade with the shopkeeper. The family will also need to be fed during this time if they are hungry. During the night, however, the player will only be able to tend to the fields and must fend off any monsters that may appear while doing so. Vision will be impaired by the darkness and the player must depend on lanterns scattered across the farmland to survive. These lanterns will prevent the monsters from attacking the player while standing in their radius, but only last a certain time before needing to be relit.

As a base, there will be two enemy types: the chaser and the nest. When the player enters the chaser's field of vision, it will relentlessly chase them until enough time has passed or until the player uses a lantern. The nest, on the other hand, will be a stationary enemy that spawns primarily near fields and waits for the player to get close. Once the player is in range, the nest will reach an arm out in an attempt to trap the player, and will try to pull them in if successful.

In addition to these core features, I aim to implement a form of combat the player can utilize to escape dangerous situations. They would be able to throw one of the lanterns available to kill a monster, with the added risk of losing that lantern permanently.

(311 words)


## Must Have Features


- **Enemies** At least two types of enemies who will threaten the player while they try to grow crops and feed their family. One will be a basic enemy that chases the player and the other will be stationary and attempt to grab the player when they get too close.


- **Day/Night Cycle** The player will have the day to plant any crops safely, while enemies spawn during the night. A complete day/night cycle will only last around 2.5 minutes to quicken the pace of the game.


- **Lanterns/Light System** The player's main method of fending off enemies will be through lanterns, which will cause them to lose aggro when entering the light's radius. They will fizzle out after a certain amount of time and the player will need to relight them, although they can only be relit a few times each day.


## Should Have Features


- **Exploration** Give the player access to the abandoned village to explore for potential items. Enemies will only spawn in the village during the night.


- **Combat** The player can take a lantern to throw at and kill a monster, but that lantern will be permanently gone afterwards. Will give the player the ability to deal with a particularly annoying enemy, but requires a massive sacrifice.


## Could Have Features


- **Pickups** Allow items to drop on the ground to be picked up for a small time. Enemies could drop special items if the player


- **Text boxes** Text boxes may appear when interacting with certain objects. They will appear to inform the player of any items they may have just found, or to provide some short dialogue when interacting with characters like the shopkeeper.


- **Extra Enemy Types** Extra types of enemies to add some difficulty to the nights. My ideas for enemy types include ones that move in quick bursts, ones that only move when the player isn't looking at them (Doctor Who's Weeping Angels), and ones that attempt to steal/sabotage crops


## Wish to Have Features


- **Spoiling food** Food can spoil if not sold or eaten after a certain number of days. Could force the player to think carefully about when they use or sell their food, but might also lead to a game where the difficulty is focused more toward the resource management rather than the monsters, which I want to avoid.


- **Random encounters** Once per day, there is a chance that a serf will run through your land and ask for a certain resource. If you're able to provide it before they leave, then you'll earn some extra coin compared to what you would have gotten by selling it normally.

