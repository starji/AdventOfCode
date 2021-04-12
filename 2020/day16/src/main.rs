use unicode_segmentation::UnicodeSegmentation;
use std::collections::HashMap;

fn check_rules(rules: &HashMap<String, Vec<(u64, u64)>>, value: u64) -> bool {
    rules.values().any(|v| v.iter().any(|(a,b)| a <= &value && b >= &value))
}

fn all_comply(individual_rules: &Vec<(u64, u64)>, values: &Vec<u64>) -> bool {
    values.iter().all(|x| individual_rules.iter().any(|(a,b)| a <= x && b >= x))
}    

fn scanning_rate(rules: &HashMap<String, Vec<(u64, u64)>>, tickets: &Vec<Vec<u64>>) -> u64 {
    let mut invalid_fields: Vec<u64> = Vec::new();
    for t in tickets {
        for f in t {
            if !check_rules(rules, *f) {
                invalid_fields.push(*f);
            }
        }
    }
    invalid_fields.iter().fold(0, |acc, x| acc + x)    
}

fn rotate(tickets: &Vec<Vec<u64>>) -> Vec<Vec<u64>> {

    let mut result: Vec<Vec<u64>> = Vec::new();
    result.resize(tickets[0].len(), Vec::new());

    for v in tickets {
        for (i,x) in v.iter().enumerate() {
            result[i].push(*x);
        }
    }

    return result;
}

fn filter(candidates: &HashMap<String, Vec<usize>>) -> HashMap<String, Vec<usize> > {
    let (name, fields) = candidates.iter().find(|(n, f)| f.len() == 1).unwrap();
    candidates
        .iter()
        .filter(|(n,_f)| *n != name)
        .map(|(n,f)| (String::from(n), f.iter().filter(|v| **v != fields[0]).cloned().collect()))
        .collect()
}

fn field_order(rules: &HashMap<String, Vec<(u64, u64)>>, tickets: &Vec<Vec<u64>>) -> HashMap<String, usize> {
    let fields = rotate(tickets);
    let mut candidates: HashMap<String, Vec<usize> > = HashMap::new();

    for (n, r) in rules {
        candidates.insert(String::from(n),
                          fields
                          .iter()
                          .enumerate()
                          .filter(|(i, x)| all_comply(r, x))
                          .map(|(i, _x)| i)
                          .collect());
//        println!("{} has matching {:?}", n, fields.iter().enumerate().filter(|(i, x)| all_comply(r, x)).cloned().collect());
    }

    let mut result: HashMap<String, usize> = HashMap::new();

    while candidates.values().any(|x| x.len() > 0) {
        let (name, fields) = candidates.iter().find(|(n,x)| x.len() == 1).unwrap();
        result.insert(String::from(name), fields[0]);
        candidates = filter(&candidates);
    }

    return result;
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let blocks: Vec<&str> = contents.split("\n\n").collect();

    let rules_str: Vec<Vec<&str>> = blocks[0].trim().lines().map(|x| x.unicode_words().collect()).collect();
    let mut rules: HashMap<String, Vec<(u64, u64)>> = HashMap::new();
    for r in &rules_str {
        rules.insert(String::from(r[0]), Vec::new());
        let rs = rules.get_mut(r[0]).unwrap();
        rs.push((r[1].parse().unwrap(), r[2].parse().unwrap()));
        rs.push((r[4].parse().unwrap(), r[5].parse().unwrap()));
    }

    let tickets: Vec<Vec<u64>> = blocks[2].trim().lines().skip(1).map(|x| x.split(',').map(|x| x.parse().unwrap()).collect()).collect();

    let filtered_tickets: Vec<Vec<u64>> = tickets
        .iter()
        .filter(|x| x.iter().all(|f| check_rules(&rules, *f)))
        .cloned()
        .collect();


    let order = field_order(&rules, &filtered_tickets);
    println!("{:?}", order);

    let my_ticket: Vec<Vec<u64>> = blocks[1].trim().lines().skip(1).map(|x| x.split(',').map(|x| x.parse().unwrap()).collect()).collect();

    println!("my ticket={:?}", my_ticket[0]);

    let mut acc = 1;
    for (n, i) in order.iter().filter(|(n, i)| n.len() >= 9 && &n[0..9] == "departure") {
        println!("{} {} = {}", n, i, my_ticket[0][*i]);
        acc = acc * my_ticket[0][*i];
    }

    println!("mult = {}", acc);
}
