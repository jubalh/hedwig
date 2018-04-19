# hedwig

hedwig is a small XMPP bot written in C.

It understands the following commands:
 - !run : run a command via popen()
 - !quit : quit hedwig

Only people in the roster are allowed to run commands.
All messages to hedwig will be printed out on stdio.

```
qmake
make
./hedwig -j hedwig@hogwarts.co.uk -p ooohooh
```
