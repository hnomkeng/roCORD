# Script Commands

## discord(string, string)

    - The first string is the message
    - The second string is the channel. The channel has to have a mapping in the configuration file.
    
    
Example:
```cpp
gonryun,34,179,11	script	NPC1	721,{
  discord("Hello World", "global");
  end;
}
```
Sends "Hello World" to the discord channel which is mapped to "global".

## others (added later)
