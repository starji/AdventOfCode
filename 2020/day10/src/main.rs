#[macro_use] extern crate itertools;

use petgraph::graphmap::*;
use petgraph::visit::*;
use petgraph::algo::*;
//use multimap::MultiMap;

fn generate_differences(sorted: &[i32], diff: i32) -> Vec<(i32, i32)> {
    return iproduct!(sorted, sorted)
        .map(|(x, y)| (*x, *y))
        .filter(|(x, y)| y - x > 0 && y - x <= diff)
        .collect();
}

fn generate_segments(sorted: &[i32], diff: i32) -> Vec<Vec<(i32, i32)> > {

    let mut result: Vec<Vec<(i32, i32)>> = Vec::new();
    let all = generate_differences(sorted, diff);

    let mut index = 0;
    let mut last = 0;
    
    while index + 1 < all.len() {
        if all[index + 1].1 - all[index + 1].0 == diff &&
            all[index].0 != all[index + 1].0
        {
            let to_push: Vec<(i32, i32)> = (&all[last..index+1]).iter().map(|x| *x).collect();
            result.push(to_push);
            last = index + 1;
        }
        index = index + 1;
    }
    
    return result;
}

fn count_differences(sorted: &[i32], diff: i32) -> usize {

    let mut count = 0;
    let mut index = 0;

    while sorted.get(index+1).is_some() {
        if sorted[index+1] - sorted[index] == diff {
            count = count + 1;
        }
        index = index + 1;
    }

    return count;
}

//fn count_paths(map: &MultiMap<i32, i32>, node: i32) -> usize {
//    let opt = map.get_vec(&node);
//    if opt.is_some() {
//        let vec = opt.unwrap();
//        let mut count = vec.len();
//        for n in vec {
//            count = count * count_paths(map, *n);
//        }
//        return count;
//    }
//    else {
//        return 1;
//    }
//
//}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let mut numbers: Vec<i32> = contents.trim().lines().map(|x| x.parse::<i32>().unwrap()).collect();

    let socket: i32 = 0;
    numbers.push(socket); // plug
    numbers.sort();
    let device = numbers.last().unwrap() + 3;
    numbers.push(device); // device

    let segments = generate_segments(&numbers, 3);
    println!("{:?}", segments);

    let path_count = segments.iter().map(|x| {
        let graph = DiGraphMap::<i32, i32>::from_edges(x);
        let paths = all_simple_paths::<Vec<_>, _>(&graph, x.first().unwrap().0, x.last().unwrap().1, 0, None);
        paths.count()
    }).fold(1, |acc, c| acc * c);
    println!("path count={}", path_count);

    //    println!("{:?}", path_counts);

//    let edges = generate_differences(&numbers, 3);
//    println!("{:?}", edges);
//    
//    let graph = DiGraphMap::<i32, i32>::from_edges(edges);
//    let paths = all_simple_paths::<Vec<_>, _>(&graph, socket, device, 0, None);
//    println!("number of simple paths= {}", paths.count());
}
