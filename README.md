# Simple Narrative System Plugin
 
An easy to setup and use plugin for manage a narrative system made for Unreal Engine 5.
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
Only one data table for speakers avoid typo and it will save in the plugin folder.
```

- Where should I create the widget and audio?

```
From the world subsystem. It has been a nightmare but I succeed!
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
Idk
```

- Should I create a child class from UUserWidget in c++?

```
Yes, because with this way I can access more easily to it without some strange blueprint to c++ things.
```

- Should I create a custom data table interface?

```
Maybe for a more easily add of dialogues.
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
DONE: Add rich text (maybe will be removed)
DONE: You can't add two identical dialogues if they are already in queue/playing
DONE: Button for skip current line working with audio
DONE: Customizable text separator between speaker's name and text
DONE: Dialogue that stops others (main story over NPC dialogue)
DONE: Add custom effects for rich text with tags: <wave>, <rainbow>
DONE: Add dialogues event system (OnStart,OnEnd,OnAllEnd)

TO-DO: Increase dialogue stack-on-each-other dialogue number
TO-DO: Add custom duration to fade-out
TO-DO: Change font to every font easily (cannot be done with rich text)
TO-DO: Change text size and border (cannot be done with rich text)

TO-DO: Audio and dialogue stops and continues with a button press
```
