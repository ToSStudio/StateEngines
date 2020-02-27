# StateEngines
while getting beyond the delay()-command within Arduino I came across the concept of the **StateEngine** 
as covered in several Tutorials. It allows for a sort of Multi-Tasking while leaving CPU-pwr for other
tasks. A logical next step would be to write everything in OOP.

A LED&Key-Shield was lying around and there was intel online about its useage. I ended up using the LEDs
as the eight _engines_ which have completely independent _states_ (on- and off-Times).

![TM1638](https://user-images.githubusercontent.com/61556064/75483577-71178a00-59a7-11ea-9c4e-777b15dd0931.png)



serves as an example.
