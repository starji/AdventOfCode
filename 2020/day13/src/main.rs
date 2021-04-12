
fn nearest_bus(estimate: i32, available: &Vec<i32>) -> (i32, i32) {
//    let value: Vec<(usize, i32)> = available.iter().enumerate().map(|(idx, x)| (idx, ((estimate / x) + 1) * x)).collect();
    
//    println!("{:?}", value);

    let (idx, bus) = available.iter().enumerate().min_by_key(|(idx, x)| ((estimate / **x) + 1) * **x).unwrap();
    println!("{:?}", bus);

    (*bus, ((((estimate / *bus) + 1) * *bus) - estimate))
}

fn day2() {
    let test_data: Vec<(usize, i64)> = r"
7,13,x,x,59,x,31,19".to_string().trim().split(",").enumerate().map(|(i, x)| (i, x.parse())).filter(|(i, x)| x.is_ok()).map(|(i, x)| (i, x.unwrap())).collect();
    
    println!("len={}", test_data.len());
    println!("{:?}", test_data);

    let mut count = 5;
    let mut check = 1;
    while count > 0 {
        while !(check % 7 == 0 && (check + 1) % 13 == 0) {
            check = check + 1;
        }
        println!("check={}", check);
        count = count - 1;
        check = check + 1;
    }
    
    

//    let mut prev = 7;
//    let mut check = 7;
//    for (i, x) in &test_data {
//        println!("{} {} {}", i, x, prev);
//        while !((check + (*i as i64)) % x == 0) {
//            check = check + prev;
//        }
//        println!("check={}", check);
//        prev = *x;
//    }
//    
    println!("check={}", check);
}

fn main() {
    let estimate: i32 = 1001796;
    let test_data: Vec<i32> = r"37,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,41,x,x,x,x,x,x,x,x,x,457,x,x,x,x,x,x,x,x,x,x,x,x,13,17,x,x,x,x,x,x,x,x,23,x,x,x,x,x,29,x,431,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,19".to_string().trim().split(",").filter_map(|x| x.parse().ok()).collect();

    let (bus, wait) = nearest_bus(estimate, &test_data);

    println!("part1 = {}", bus * wait);

    day2();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn initial_test() {
        let estimate: i32 = 939;
        let test_data: Vec<i32> = r"
7,13,x,x,59,x,31,19".to_string().trim().split(",").filter_map(|x| x.parse().ok()).collect();

        let (bus, wait) = nearest_bus(estimate, &test_data);

        assert_eq!(wait * bus, 295);
    }

    #[test]
    fn day2_test() {
        let test_data: Vec<(usize, i64)> = r"
7,13,x,x,59,x,31,19".to_string().trim().split(",").enumerate().map(|(i, x)| (i, x.parse())).filter(|(i, x)| x.is_ok()).map(|(i, x)| (i, x.unwrap())).collect();

        println!("len={}", test_data.len());
        println!("{:?}", test_data);

        

        let mut check = 7;
        for (i, x) in &test_data {
            println!("{} {}", i, x);
            while !(check % x == 0) {
                check = check + x;
                println!("{}", check);
            }
        }

        println!("check={}", check);

        let mut value: i64 = 1;

        for (i, x) in &test_data {
            value = value * x;
        }

        println!("mult={}", value);

        assert_eq!(true, false);

    }
}
