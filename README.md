# Mini database
## ACK
### This is a mini database, using B-tree as the index and .db file as presistence, original version(see link below) was writtern in C, this is a re-write version in C++ following the RAII rules, and also using the CMAKE as a building and testing tool
## 

## Example
```
miniDB << "insert 1 user1 person1@example.com",
miniDB << "select",
```
```
miniDB >> (1 user1 person1@example.com)
```

## Insert many
### you can find this in the tests folder
```
std::vector<std::string> scripts4{
        "insert 18 user18 person18@example.com",
        "insert 1 user1 person1@example.com",
        "insert 7 user7 person7@example.com",
        "insert 10 user10 person10@example.com",
        "insert 29 user29 person29@example.com",
        .....
        "insert 13 user13 person13@example.com",
        "insert 24 user24 person24@example.com",
        "insert 25 user25 person25@example.com",
        "insert 28 user28 person28@example.com",
        ".btree",
        ....
    };

```

```
 Tree:
 - internal (size 3)
   - leaf (size 7)
     - 1
     - 2
     - 3
     - 4
     - 5
     - 6
     - 7
   - key 1
   - leaf (size 8)
     - 8
     - 9
     - 10
     - 11
     - 12
     - 13
     - 14
     - 15
   - key 15
   - leaf (size 7)
     - 16
     - 17
     - 18
     - 19
     - 20
     - 21
     - 22
   - key 22
   - leaf (size 8)
     - 23
     - 24
     - 25
     - 26
     - 27
     - 28
     - 29
     - 30
```

# How to use it?
## requirements:
### -Linux
### -GCC or LLVM
### -CMAKE
## Instruction:
### download the code and open a terminal:
```
cd code_path/miniDB/src
```

### create a folder call build
```
mkdir build
```

### set up CMAKE
```
cmake ..
```

### make and test the code

```
make
ctest
```

### play with it
```
./miniDB <nameWhateverYouWant>.db
```
### here are the commands
for insertion:
```
insert <key> <name> <email>
```

for selection
```
select
```

see the tree

```
.btree
```

see configuration
```
.constants
```

quit and write to the db
```
.exit
```
# Orginal post, respect to cstack
## click-> https://github.com/cstack/db_tutorial