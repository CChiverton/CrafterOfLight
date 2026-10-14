This crafting solver was inspired by many of the other crafting solutions out there. Giving greater control over parameters so that rather than crafting a specific item, the stat baseline is used and applicable for multiple crafts.

UI developed using QT, with C++20 for the logic behind it, and implements multithreading for faster solving.

Use the Brute Craft/Smart Craft buttons on the bottom right to choose which way to solve. When a solution is found a scrollable window will be have buttons to click and copy a macro to your clipboard to paste in the game.

<p align=center>
<img width="597" height="434" alt="image" src="https://github.com/user-attachments/assets/a8f7a770-6f03-4f79-9fd0-304dca8b928e" />
</p>

All of the displayed stats on the left-hand side are found in-game.<br>
<b>Max Progress:</b> This is the amount of progress required to complete of a craft.<br>
<b>Max Quality:</b> This is the value to get a 100% chance of a high quality item. Any less than the maximum will give a % chance to get a high quality item on completion.<br>
<b>Max Durability:</b> When this value reaches 0 the craft fails unless progress has reached its maximum value.

<b>Max CP:</b> The Crafter Points that the character uses to perform special actions.<br>
<b>Progress Efficiency:</b> This is the character stat relating to how much progress is generated in one progress action. This amount will very based on disparity between this and the maximum value of the craft.<br>
<b>Quality Efficiency:</b> This is the character stat relating to how much quality is generated in one quality action. This amount will very based on disparity between this and the maximum value of the craft.

All of the displayed stats on the right-hand side are the solution-finding parameters<br>
<b>Maximum Steps:</b> This is the maximum number of steps the solver will reach finding a solution.<br>
<b>Enforce Maximum Time:</b> This option is used to allow a mix between maximum numbers of steps and time to craft.<br>
<b>Maximum Time:</b> This is the maximum time of a crafting solution the solver will use to determine if a craft is valid or not.<br>
<b>Max Quality:</b> This option enables or disables icons to the left relating to quality and disables quality finding in the craft and will only search for solutions with 100% quality.<br>
<b>Force Turn One:</b> This option forces the start of a solution to begin with actions that can only be performed on turn one.<br>

<b>Brute Craft:</b> This is a slower but more exhaustive search through all crafting possibilities.<br>
<b>Smart Craft:</b> This is a faster solver that uses some logic to help navigate all possible options and reduce solving time. As such there is always a chance that it misses out possible solutions, but can be useful in finding upper limits to use with the brute crafter.<br>


This tool is completely standalone and does not interact with FFXIV files in any way and as such does not violate any ToS. 
