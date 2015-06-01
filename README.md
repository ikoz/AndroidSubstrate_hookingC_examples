This project contains code used in the examples of this blogpost: https://koz.io/android-substrate-c-hooking

targetApp1
-> The original target application, with exported getAge().

targetApp2
-> A modified version of the target application where getAge() is static (symbol not visible).

nativeHook1
-> A substrate module hooking arc4random() (bionic).

nativeHook2
-> A substrate module hooking arc4random() (bionic) and getAge() (internal) - getAge symbol needs to be visible.

nativeHook3
-> A substrate module hooking arc4random() (bionic) and getAget() (internal) - getAge symbol does not need to be visible, but you have to edit the source and hardcode the correct prefix.

John Kozyrakis
