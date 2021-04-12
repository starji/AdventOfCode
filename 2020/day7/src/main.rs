
use std::collections::HashSet;
use multimap::MultiMap;

fn part1_containers(map: &MultiMap<String, String>, color: &str, mut set: &mut HashSet<String>) {
    match map.get_vec(color) {
        Some(vec) => {
            for c in vec {
                set.insert(String::from(c));
                part1_containers(&map, &c, &mut set);
            }
        },
        None => {}
    }
}

fn part1(contents: &str) {

    let mut map = MultiMap::new();
    let mut set = HashSet::new();

    for l in contents.trim().lines() {
        let mut words = l.split_whitespace().peekable();
        let color = format!("{} {}", words.next().unwrap(), words.next().unwrap());

        println!("{}", color);

        words.next(); // bags
        words.next(); // contain

        while words.peek().is_some() && words.peek().unwrap() != &"no" {
            let count: usize = words.next().unwrap().parse().unwrap();
            let inside_color = format!("{} {}", words.next().unwrap(), words.next().unwrap());
            words.next();

            println!("\t{} {}", count, inside_color);
            map.insert(inside_color, color.clone());
        }
    }

    part1_containers(&map, &"shiny gold", &mut set);
    println!("shiny gold = {}", set.len());
}

fn part2_contents(map: &MultiMap<String, String>, color: &str) -> usize {
    let mut count = 0;
    match map.get_vec(color) {
        Some(vec) => {
            for c in vec {
                count = count + 1 + part2_contents(&map, &c);
            }
        },
        None => {}
    }
    return count;
}

fn part2(contents: &str) {
    let mut map = MultiMap::new();

    for l in contents.trim().lines() {
        let mut words = l.split_whitespace().peekable();
        let color = format!("{} {}", words.next().unwrap(), words.next().unwrap());

        println!("{}", color);

        words.next(); // bags
        words.next(); // contain

        while words.peek().is_some() && words.peek().unwrap() != &"no" {
            let mut count: usize = words.next().unwrap().parse().unwrap();
            let inside_color = format!("{} {}", words.next().unwrap(), words.next().unwrap());
            words.next();

            println!("\t{} {}", count, inside_color);

            while count > 0 {
                map.insert(color.clone(), inside_color.clone());
                count = count - 1;
            }
        }
    }

//    println!("{:?}", map);

    println!("shiny gold contains = {}", part2_contents(&map, &"shiny gold"));
}


fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

//    let mut map = HashMap::new();

    part2(&contents);
        

}
