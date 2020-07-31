DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS robots;
DROP TABLE IF EXISTS maps;
DROP TABLE IF EXISTS stations;
DROP TABLE IF EXISTS tasks;

CREATE TABLE users (
	id INTEGER PRIMARY KEY NOT NULL,
	username TEXT NOT NULL,
    hash TEXT NOT NULL
);

CREATE TABLE robots (
    id INTEGER PRIMARY KEY NOT NULL,
    user_id INTEGER,
    robot_name TEXT,
    robot_type TEXT,
    ip_address TEXT,

    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE maps (
    id INTEGER PRIMARY KEY NOT NULL,
    mapfile_name TEXT,
    location TEXT,
    mapfile_location TEXT
);

CREATE TABLE stations (
    id INTEGER PRIMARY KEY NOT NULL,

    map INTEGER,
    location TEXT,
    station_type TEXT,
    postion TEXT,
    robot_at_station INTEGER,
    station_status INTEGER,

    FOREIGN KEY (map) REFERENCES maps(id),
    FOREIGN KEY (location) REFERENCES maps(location),
    FOREIGN KEY (robot_at_station) REFERENCES robots(id)
);

CREATE TABLE tasks (
    id INTEGER PRIMARY KEY NOT NULL,
    user_id INTEGER,
    robot_id INTEGER,
    map INTEGER,
    start_station INTEGER,
    goal_station INTEGER,
    priority INTEGER,
    task_type TEXT
    datetime DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (robot_id) REFERENCES robots(id),
    FOREIGN KEY (map) REFERENCES maps(id),
    FOREIGN KEY (start_station) REFERENCES stations(id),
    FOREIGN KEY (goal_station) REFERENCES stations(id)
);

INSERT INTO users (username, hash) VALUES ('admin', "pbkdf2:sha256:150000$RydnHB1s$4e8c13bb779b3403c3e7dbd3800ffabe4908b44e57578fa150108c83d84d2253");
