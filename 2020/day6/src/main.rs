use itertools::*;
use std::collections::*;

fn part1(contents: &str) -> usize {
    let groups = contents.trim().split("\n\n");
    let count = groups.map(|x| {
        let mut u: Vec<char> = x.chars().collect();
        u.sort_unstable();
        return u.iter().filter(|x| 'a' <= **x && **x <= 'z').unique().count();
    }).fold(0, |acc, x| acc + x);

    return count;
}

fn part2(contents: &str) -> usize {
    let groups = contents.trim().split("\n\n");

    let mut count: usize = 0;
    for g in groups {

        let mut everyone = g.split("\n")
            .map(|x| x.chars().filter(|x| 'a' <= *x && *x <= 'z')
                 .collect::<HashSet<char>>());

        let mut acc: HashSet<char> = everyone.next().unwrap().clone();
        for e in everyone {
            acc = acc.intersection(&e).map(|x| *x).collect();
        }

        count = count + acc.len();
//        println!("{:?}", acc);

//        let mut u: HashSet<char> = g.chars().collect();
//
//        let answers = u.iter().filter(|x| 'a' <= **x || **x >= 'z');
//
//        let stupid: String = answers.collect();


    }

    return count;

}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    println!("{}", part1(&contents));

    println!("{}", part2(&contents));
}
