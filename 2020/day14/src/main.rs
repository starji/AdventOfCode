
use unicode_segmentation::UnicodeSegmentation;
use std::collections::{HashMap, HashSet};


fn duplicate_address(address: u64, offset: u64) -> (u64, u64) {
    let mask = 1 << offset;
    return (address | mask, address & !mask);
}

fn create_addresses(mask: &Vec<(usize, char)>, address: u64) -> HashSet<u64> {
    let mut set: u64 = 0;
    for (offset, bit) in mask {
        if *bit == '1' {
            let v = 1 << (35 - offset);
            set = set | v;
        }
    }

    let mut results: HashSet<u64> = HashSet::new();

    println!("Set bits");

    for (offset, bit) in mask {
        if *bit == 'X' {
            let mut more: Vec<u64> = Vec::new();
            if results.len() == 0 {
                let (s, u) = duplicate_address(address | set, (35 - (*offset) as u64));
                more.push(s);
                more.push(u);
            }
            else {
                for r in &results {
                    let (s, u) = duplicate_address(*r, (35 - (*offset) as u64));
                    more.push(s);
                    more.push(u);
                }
            }
            for m in &more {
                results.insert(*m);
            }
        }
    }

    return results;
}

fn create_mask(parse: &str) -> Box<dyn Fn(u64) -> u64> {
    let mask: Vec<(usize, u64)> = parse
        .chars()
        .enumerate()
        .map(|(i, x)| (i, x.to_digit(10)))
        .filter(|(_i, x)| x.is_some())
        .map(|(i, x)| (i, x.unwrap() as u64))
        .collect();

    let mut set: u64 = 0;
    let mut unset: u64 = 0;

    for (offset, bit) in &mask {
        if *bit == 1 {
            let v = 1 << (35 - offset);
            set = set | v;
        }
        else if *bit == 0 {
            let v =  1 << (35 - offset);
            unset = unset | v;
        }
    }

    unset = !unset;

    Box::new(move |x: u64| { (x | set) & unset })
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let commands: Vec<Vec<&str> > = contents.trim().lines().map(|x| x.unicode_words().collect()).collect();

//    let mut apply_mask = create_mask(&"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

    let mut mask: Vec<(usize, char)> = Vec::new();

    let mut memory: HashMap<u64, u64> = HashMap::new();
    
    for c in &commands {
//        println!("{:?}", c);

        if c[0] == "mask" {
            //            apply_mask = create_mask(c[1]);
            mask = c[1].chars().enumerate().collect();
        }
        if c[0] == "mem" {
            //            memory.insert(c[1].parse().unwrap(), (&apply_mask)(c[2].parse().unwrap()));
            let addresses = create_addresses(&mask, c[1].parse().unwrap());
            for a in &addresses {
                memory.insert(*a, c[2].parse().unwrap());
            }
        }
    }

    let mut sum: u128 = 0;
    for (_k, v) in &memory {
        sum = sum + (*v) as u128;
    }
    println!("sum={}", sum);

}
