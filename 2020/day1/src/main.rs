#[macro_use] extern crate itertools;

fn main() {
    let args: Vec<String> = std::env::args().collect();

    let filename = &args[1];

    println!("{}", filename);

    let contents = std::fs::read_to_string(filename).expect("file not found");

//    let values = contents.split('\n').filter(|x| !x.is_empty()).map(|x| x.parse::<i32>().unwrap());
    let values = contents.split('\n').filter_map(|x| x.parse::<i32>().ok());

    let values2 = values.clone();

    let values3 = values.clone();

    for (x, y, z) in iproduct!(values, values2, values3) {
        if x + y + z == 2020
        {
            println!("{0}, {1}, {2}, {3}", x, y, z, x * y * z);
        }
    }
        
}
