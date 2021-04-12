use std::collections::HashMap;

fn walk(nums: &Vec<u64>, iteration: u64) -> u64 {
    let mut map: HashMap<u64, u64> = HashMap::new();
    let mut step: u64 = (nums.len() - 1) as u64;
    let mut last: u64 = *nums.last().unwrap();

    let trunc = &nums[..nums.len()-1];
    for (k, v) in trunc.iter().enumerate() {
        map.insert(*v, k as u64);
    }


    let mut result = 0;
    while step < iteration {
//        println!("last={} step={}", last, step);
        let val = match map.get(&last) {
            Some(x) => step - x,
            None => 0
        };
        map.insert(last, step);
        result = last;
        last = val;
        step = step + 1;
    }

//    
//    last = ??
//
//        // check existence of target number
//        val = match map.get(last) {
//            Some(x) => step - x,
//            None => 0
//        }
//// add/update the last to the map
//    map.insert(last, step)
//        // next number to check
//        last = val 
//    
//
//

    return result;
}

fn step(nums: &Vec<u64>) -> u64 {
    let check = nums.last().unwrap();

    let sl = &nums[..nums.len()-1];
    match sl.iter().rposition(|x| x == check) {
        Some(x) => (sl.len() - x) as u64,
        None => 0
    }

//    match nums.iter().rev().enumerate().skip(1).find(|(_o, x)| *x == check) {
//        Some((o, _x)) => o as u64,
//        None => 0
//    }
}



fn main() {
//    let args: Vec<String> = std::env::args().collect();
//    let filename = &args[1];
//    let contents = std::fs::read_to_string(filename).expect("File not found");

    let mut nums = vec![16,11,15,0,1,7];
    while nums.len() < 30000000 {
        if nums.len() % 250000 == 0 {
            println!("{}", nums.len());
        }
//        println!("step={}", nums.len());
        nums.push(step(&nums));
    }

    println!("{}", *nums.last().unwrap())

//    let mut nums = vec![0,3,6];
//    println!("{}", walk(&nums, 30000000));

//    while nums.len() < 10 {
//        nums.push(step(&nums));
//    }
//    println!("{:?}", nums);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test1_2020() {
        let mut nums = vec![0,3,6];
        assert_eq!(walk(&nums, 2020), 436);
    }

    #[test]
    fn test2_2020() {
        let mut nums = vec![1,3,2];
        while nums.len() < 2020 {
            nums.push(step(&nums));
        }
        assert_eq!(*nums.last().unwrap(), 1);
    }

    #[test]
    fn test3_2020() {
        let mut nums = vec![2,1,3];
        while nums.len() < 2020 {
            nums.push(step(&nums));
        }
        assert_eq!(*nums.last().unwrap(), 10);
    }

    #[test]
    fn test4_2020() {
        let mut nums = vec![1,2,3];
        while nums.len() < 2020 {
            nums.push(step(&nums));
        }
        assert_eq!(*nums.last().unwrap(), 27);
    }

    #[test]
    fn test5_2020() {
        let mut nums = vec![2,3,1];
        while nums.len() < 2020 {
            nums.push(step(&nums));
        }
        assert_eq!(*nums.last().unwrap(), 78);
    }

    #[test]
    fn test6_2020() {
        let mut nums = vec![3,2,1];
        while nums.len() < 2020 {
            nums.push(step(&nums));
        }
        assert_eq!(*nums.last().unwrap(), 438);
    }
    
    #[test]
    fn test7_2020() {
        let mut nums = vec![3,1,2];
        while nums.len() < 2020 {
            nums.push(step(&nums));
        }
        assert_eq!(*nums.last().unwrap(), 1836);
    }


    #[test]
    fn test1_30000000() {
        let mut nums = vec![0,3,6];
        assert_eq!(walk(&nums, 30000000), 175594);
    }

    #[test]
    fn test2_30000000() {
        let mut nums = vec![1,3,2];
        assert_eq!(walk(&nums, 30000000), 2578);
    }

    #[test]
    fn test3_30000000() {
        let mut nums = vec![2,1,3];
        assert_eq!(walk(&nums, 30000000), 3544142);
    }

    #[test]
    fn test4_30000000() {
        let mut nums = vec![1,2,3];
        assert_eq!(walk(&nums, 30000000), 261214);
    }

    #[test]
    fn test5_30000000() {
        let mut nums = vec![2,3,1];
        assert_eq!(walk(&nums, 30000000), 6895259);
    }

    #[test]
    fn test6_30000000() {
        let mut nums = vec![3,2,1];
        assert_eq!(walk(&nums, 30000000), 18);
    }
    
    #[test]
    fn test7_30000000() {
        let mut nums = vec![3,1,2];
        assert_eq!(walk(&nums, 30000000), 362);
    }



}
