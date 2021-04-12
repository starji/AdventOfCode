#[derive(Debug, Copy, Clone)]
enum Command {
    North,
    South,
    East,
    West,
    Left,
    Right,
    Forward
}

#[derive(Debug)]
struct Ship {
    x_pos: i32,
    y_pos: i32,
    facing: Command
}

fn rotate_left(cmd: Command) -> Command {
    match cmd {
        Command::North => Command::West,
        Command::West => Command::South,
        Command::South => Command::East,
        Command::East => Command::North,
        _ => panic!("unsupported rotate left")
    }
}

fn rotate_right(cmd: Command) -> Command {
    match cmd {
        Command::North => Command::East,
        Command::West => Command::North,
        Command::South => Command::West,
        Command::East => Command::South,
        _ => panic!("unsupported rotate right")
    }
}

impl Ship {
    fn new() -> Self {
        Self {
            x_pos: 0,
            y_pos: 0,
            facing: Command::East
        }
    }

    fn move_ship(&mut self, cmd: Command, amt: i32) {
        match cmd {
            Command::North => { self.y_pos = self.y_pos + amt },
            Command::South => { self.y_pos = self.y_pos - amt },
            Command::East => { self.x_pos = self.x_pos + amt },
            Command::West => { self.x_pos = self.x_pos - amt },
            Command::Left => {
                let mut times = amt / 90;
                while times > 0 {
                    self.facing = rotate_left(self.facing);
                    times = times - 1;
                }
            }
            Command::Right => {
                let mut times = amt / 90;
                while times > 0 {
                    self.facing = rotate_right(self.facing);
                    times = times - 1;
                }
            }
            Command::Forward => { self.move_ship(self.facing, amt); }
            _ => panic!("Unknown command")
        }
    }

    fn manhattan_distance(&self) -> i32 {
        return self.x_pos.abs() + self.y_pos.abs();
    }
}

fn wp_cmd(cmd: (Command, i32), ship: (i32, i32), waypoint: (i32, i32)) -> ((i32, i32), (i32, i32)) {
    match cmd.0 {
        Command::North => (ship, (waypoint.0, waypoint.1 + cmd.1)),
        Command::South => (ship, (waypoint.0, waypoint.1 - cmd.1)),
        Command::East => (ship, (waypoint.0 + cmd.1, waypoint.1)),
        Command::West => (ship, (waypoint.0 - cmd.1, waypoint.1)),
        Command::Left => {
            let mut times = cmd.1 / 90;
            let (mut x, mut y) = waypoint;
            while times > 0 {
                let x_copy = x;
                let y_copy = y;
                y = x_copy;
                x = -y_copy;
                times = times - 1;
            }
            (ship, (x, y))
        }
        Command::Right => {
            let mut times = cmd.1 / 90;
            let (mut x, mut y) = waypoint;
            while times > 0 {
                let x_copy = x;
                let y_copy = y;
                y = -x_copy;
                x = y_copy;
                times = times - 1;
            }
            (ship, (x, y))
        }
        Command::Forward => ((ship.0 + cmd.1 * waypoint.0, ship.1 + cmd.1 * waypoint.1),
                             waypoint)
    }
}

fn manhattan_distance(x: i32, y: i32) -> i32 {
    return x.abs() + y.abs();
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let commands: Vec<(Command, i32)> = contents.trim().lines().map(|x| (match &x[..1] {
        "N" => Command::North,
        "S" => Command::South,
        "E" => Command::East,
        "W" => Command::West,
        "L" => Command::Left,
        "R" => Command::Right,
        "F" => Command::Forward,
        _ => panic!("unprocessed command")
    }, *(&x[1..].parse::<i32>().unwrap()))).collect();

    let mut ship = Ship::new();
    for c in &commands {
        println!("{:?}", c);
        ship.move_ship(c.0, c.1);
        println!("{:?}", ship);
    }

    println!("manhattan distance = {}", ship.manhattan_distance());

    let mut ship2 = (0, 0);
    let mut waypoint = (10, 1);

    for c in &commands {
        let (s, w) = wp_cmd(*c, ship2, waypoint);
        ship2 = s;
        waypoint = w;
        println!("{:?} {:?} {:?}", c, ship2, waypoint);
    }

    println!("manhattan distance = {}", manhattan_distance(ship2.0, ship2.1));

}
