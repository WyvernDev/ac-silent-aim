# Assault cube silent aim.
## Works for version V1.2.0.2
### How is work?

This works by hooking the shoot function which is present in most weapon vtables in the game. It changes the position of where the bullet will spawn, to an enemy player's head of course.

Below is some screenshots of the assembly and the psuedocode I reversed to create a better understanding.

![alt text](https://github.com/WyvernDev/ac-silent-aim/blob/main/ida_XQUKOCDSL7.png)
![alt text](https://github.com/WyvernDev/ac-silent-aim/blob/main/ida_jbAN3cQdqi.png)
