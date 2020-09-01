# CS50 Final Project - Robot Manager
Robot Manager is a web app that allows users to manage a fleet of robots. 

## Installation
Python 3, Flask, sqlite3

```
pip3 install Flask
pip3 install db-sqlite3
```

## Usage
```
$ flask run
```

## JSON format for tasklist
```
{
    "tasks_name": "test_task",

    "map_name": "test",
    "map_file": "test.pgm",
    "yaml_file": "test.yaml",

    "tasks": [
        {"id": "0", "waypoint": [0, 0, 0]},
        {"id": "1", "waypoint": [1, 1, 1]},
        {"id": "2", "waypoint": [2, 2, 2]},
        {"id": "3", "waypoint": [3, 3, 3]}
    ]
}
```