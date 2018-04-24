# hedwig

hedwig is a small XMPP bot written in C.

It understands the following commands:
 - !exec : run any program via popen()
 - !cmd  : run defined commands
 - !quit : quit hedwig

Only people in the roster are allowed to run commands.
All messages to hedwig will be printed out on stdio.

The `!exec` option is disabled by default for security reasons. To enable it start hedwig with `-e`.

`!cmd` is used to extend the bot. It runs files that have the executable bit set and are in a certain directory.
By default that is the current working directory, but you can define it with `-d`.

## Example

```
cd ~/hedwigs-plugins
echo "echo 'hello harry'" > greet
echo "ls" > list-commands
chmod u+x greet list-commands
hedwig -j hedwig@hogwarts.co.uk -p ooohooh -d ~/hedwigs-plugins
```

## Building

```
qmake
make
./hedwig -j hedwig@hogwarts.co.uk -p ooohooh
```
