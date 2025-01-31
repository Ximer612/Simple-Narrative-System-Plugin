# Simple Narrative System Plugin
 
An easy-to-setup and use plugin for manage a narrative system made for Unreal Engine 5.
## Brain Storming Process

I've outlined the steps I took to create this plugin below.

- How to interact from the editor with C++?

```
EDITOR/BLUEPRINT → BLUEPRINT FUNCTION LIBRARY → DIALOGUE WORLD SUBSYSTEM → WIDGET IN BLUEPRINT

From the editor, you call a function to add a dialogue to the list of dialogues to be played through the blueprint function library.  
The blueprint function library passes the data to the world subsystem, which manages the widget present in the game through interfaces.
```

- Should it have multiple data tables for dialogues to manage each level and for better organization or just one for simplicity?

```
Better the first one.
```

- The speaker should only use one shared data table? Where do I save it?  
```
Only one data table for speakers avoid typos and it will save in the plugin folder.
```

- Where should I create the widget and audio?

```
From the world subsystem. It has been a nightmare process since I've never touched them.
```

- Does it work with multiple sublevels?

```
Yes.
```

- Change settings through saves or something else?

```
Through a Game Instance Subsytem.
```

- How to fix automatic size to content offset on top?

```
From a parameter inside the dialogue Data Table.
```

- Should I create a child class from UUserWidget in c++?

```
Yes, because in this way I can access more easily to it without some strange blueprint to c++ references.
```

- Should I create a custom data table interface?

```
Maybe for a more easy way to add of dialogues.
```

- Should I remove rich text?

```
No, because I want to use their cool effect that can be made using decorators.
If you don't want rich text you can still remove it by looking only into blueprints!
```


- Done and to-do stuff
```
DONE: Show/Hide subtitles and still audio plays
DONE: Speaker name ON/OFF
DONE: Background opacity and color
DONE: Change dialogue default text color
DONE: Fade in/out ON/OFF
DONE: Stack dialogue on top of each other (current max two)
DONE: Don't do the stack animation to the last dialogue
DONE: Add rich text
DONE: You can't add two identical dialogues if they are already in queue/playing
DONE: Button for skip current line working with audio
DONE: Customizable text separator between speaker's name and text
DONE: Dialogue that stops others (main story over NPC dialogue)
DONE: Add custom effects for rich text with tags: <wave>, <rainbow>
DONE: Add dialogues event system (OnStart,OnEnd,OnAllEnd)
DONE: Using Project Settings for get widget via C++
DONE: Added "FMessageLog("PIE").Error" for in-editor errors
DONE: Added "deletesns" console command to delete saved widget settings
DONE: Custom editor module for a more easy way to add data inside Data Tables

TO-DO: Increase dialogue stack-on-each-other dialogue number
TO-DO: Add custom per-dialogue-line fade-out duration
TO-DO: Audio and dialogue stops and continues with a button press (Like a visual novel)
TO-DO: Multithreading
TO-DO: Network replication
TO-DO: More text effects
TO-DO: Move more Widget Blueprint logic to C++

DISCARDED: Customizable text font,size and border (cannot be done with rich text)
```
