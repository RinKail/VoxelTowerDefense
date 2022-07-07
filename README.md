# Voxel Tower-Defense

# How the project works
This is a UE5 Tower-Defense project designed to be extendable.

## Core Classes

The classes listed bellow are where the core gameplay primarily takes place.


### [GameMode](https://github.com/RinKail/VoxelTowerDefense/blob/main/Source/BlockDefense/BlockDefenseGameMode.h)
This class is responsible for AI management and Wave Control.
It also manages the tiles and their visual events / Lock States.

### [PlayerController](https://github.com/RinKail/VoxelTowerDefense/blob/main/Source/BlockDefense/BlockDefensePlayerController.h)
The player Controller class handles all gameplay inputs. It determines what tile is selected and what Blocks to place.
It contains the Score and initial Recipe unlocks.

### [Construction Grid](https://github.com/RinKail/VoxelTowerDefense/blob/main/Source/BlockDefense/Map/Map_ConstructionGrid.h)
The Construction grid handles all building and is seperate from the tiles. 
This only cares about what Blocks are currently placed and what tiles currently have a Building. 

### [Buildable Base](https://github.com/RinKail/VoxelTowerDefense/blob/main/Source/BlockDefense/Buildables/BuildableBase.h)
This is the base class for Buildings and primarily contains Blueprint events or virtual functions shared between all Buildables.

#### [Towers](https://github.com/RinKail/VoxelTowerDefense/blob/main/Source/BlockDefense/Buildables/Buildable_Turret.h)
This contains all turret logic and inherits from Buildable Base

#### [Structures](https://github.com/RinKail/VoxelTowerDefense/blob/main/Source/BlockDefense/Buildables/Buildable_Structure.h)
This contains all structure logic and inherits from Buildable Base.


## Creating new levels.
Creating new levels and making them playable is rather straight forward. 
The following section will go over the tools provided and how to add your new level to the game.

### [Tiles](https://github.com/RinKail/VoxelTowerDefense/blob/main/Source/BlockDefense/Map/Map_Tile.cpp)
There are two types of Tiles that the developer needs to worry about, Game Tiles and Editor Tiles. 
#### [Game Tiles](https://github.com/RinKail/VoxelTowerDefense/blob/main/Source/BlockDefense/Map/Map_Tile_Game.h)
Game tiles contain all of the important gameplay elements, if you wish to create a new set of tiles, this is the one you must inherit from.
#### [Editor Tiles](https://github.com/RinKail/VoxelTowerDefense/blob/main/Source/BlockDefense/Map/Map_Tile_Editor.h)
Editor tiles instead are specifically for creating the maps in the editor. These contain no gameplay elements and are instead more akin to a tool. Once gameplay starts they will automatically generate their coresponding Gameplay counterpart, but these exact classes must be set by the designer. 

By Default, there is a setup already available in the project.

### Chunk Builder
The Chunk Builder classes is similar to the Editor Tile in that is has no gameplay elements and is purely an editor tool. 
This actor simply allows the designer to designate a tile scale and map size and it will automatically add the requested tiles to the level in a square grid. The number input is the size of both axis.

Do note that the tile system implemented does in fact support unique tile shapes, and as such it is possible to arrange the tiles in any sort of shape just as long as the spacing is uniform.
Level 3 is an example of this.

### Map Creation

Using the previously mentioned tools, you can create your levels with ease. The fastest method to creating a new level is to copy an existing level and delete all the tiles on the map. Do this by selecting them all in the Outliner.
Once all tiles are deleted you can generate a new Grid using the Chunk Builder.

While this is recomended, there is nothing stopping a designer from simply editing existing maps either.

To lay out your AI Pathways, Hold ctrl + alt to select and drag on the grid.
Select all the tiles you wish to form the passages and the  in their Details panel change the TransformTo property under "Tile Transformation" to passage.
This will set these selected tiles to the passage type, to add build platforms repeat the previous step, but instead set them to platform.
This also works for the Build Points.

Build Points should be sparse, they are also not required but they do help players get started on levels. Harder maps will have less build maps available at the start.

### Adding your level to level select.

To add a level to Level select, simply navigate to BlockDefense/Core/Data and open DT_LevelSelect.
Create a new row and fill in the properties and make sure to add your levels file name to the table.
Your level is now inside level select!
Be careful when packaging the game, as you will have to make sure your level is set to be included in the package.





## Building
Buildings are constructed using "Construction Blocks". These come in 3 variants, Red, Green and Blue.

Using these Blocks you can construct a Tower or structure by arranging them as specified by the recipe bar.
Only unlocked Recipes will take affect.

### Developing new Buildings
There are 3 steps to adding a new building. 
To add a new building, no C++ is required and can all be done through blueprints making this template incredible designer friendly.
#### Creating the actor
To create the buildable actor, create a child Blueprint of either BP_Build_Turret_MASTER or BP_Build_Structure_MASTER. These Blueprint classes contain all of the additional visual effects that all turrets contain. 

if you wish to create your own VFX and ignore the default, you can directly create a new Turret or Structure from the base C++ classes ABuildable_Turret or ABuildable_Structure. The only logic contained without the Master Blueprints are purely for visuals, so no features will be lost if you choose to go this route.

#### Add it to the Build Table
To make your newly created Buildable useable in the game is quick and simple. 
Navigate to BlockDefense/Core/Data and open DT_StructureTable. 
Inside the table you will find all the available Buildables and their corresponding data. 
To create the recipe, assign the blocks to the composition section, keep in mind that regardless of type, Coordinates [0|0|0] correspond to the root position.
Make sure to name the row, this is used for accessing the recipe.

The recipe is now available to the game, now we need a way for the player to unlock the recipe.

#### Unlock requirement
For the player to gain access to your newly created building, you must add it to an Unlock-List. 
There are two choices for this. 
- Adding the ID to an existing unlockable Structure
- Adding the ID to the player controller and have it unlocked from the start.

Regardless of your choice, the method of making it unlockable is the same, take the name you gave your building inside the Table and add it to the UnlockRecipes array inside your 
chosen destination.

If you've followed these 3 steps correctly, you have now successfully added a new Building to the game!













